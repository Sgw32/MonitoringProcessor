#include "RadarReader.h"
#include "RadarUtils.h"

RadarReader::RadarReader()
{

}

RadarReader::~RadarReader()
{

}

void RadarReader::init(string radarPrefix, string radarZIP, string rawPrefix, string radarName)
{
	mRadarPrefix = radarPrefix;
	mRadarZip = radarZIP;
	mRawPrefix = rawPrefix;
	mRadarName = radarName;
}

void RadarReader::initFromFile(string infile, string infile2)
{
	string test_pr = infile;
	if (!fileExists(infile + mRadarZip))
		test_pr = infile2;
	//file = zip_file(test_pr + mRadarZip);
	file.load(test_pr + mRadarZip);
	base = file.get_first_filename();
	if (base.find_last_of('/') != string::npos)
		base = base.substr(0, base.find_last_of('/')) + "/";
	else
		base = "";
	//file = fil;
}

void RadarReader::setSettings(vector<bool> stngs)
{
	mSettings = stngs;
}

void RadarReader::setRadarNumber(int radar)
{
	mRadarNumber = radar;
}

string RadarReader::radar_readFormat(zip_file &file, string base, int dayornight, string format)
{
	string data;
	string filename;
	char prefix[50];
#ifdef FIND_ALL_FILES
	if (!dayornight)
	{
		for (int j = 0; j != 24; j++)
		{
			for (int i = 0; i != 60; i++)
			{
				data = try_readFormat(file, base, dayornight, format, "-%02d.%02d%s", j, i);
				if (!data.empty())
					return data;
				data = try_readFormat(file, base, dayornight, format, "-%02d%02d%s", j, i);
				if (!data.empty())
					return data;
			}
		}
	}
	else
	{
		if (morning_hour || morning_min)
		{
			//WARNING!!!
			// ���� ���� ������ ����� ���� �� ����� ���������� ������, �� 
			// ����� ����� ����� ������, �������� ���� �������. 
			for (int j = morning_hour + 1; j != 24; j++)
			{
				for (int i = 0; i != 60; i++)
				{
					data = try_readFormat(file, base, dayornight, format, "-%02d.%02d%s", j, i);
					if (!data.empty())
						return data;
					data = try_readFormat(file, base, dayornight, format, "-%02d%02d%s", j, i);
					if (!data.empty())
						return data;
				}
			}
		}

	}
#else
	for (int i = 30; i != 60; i++)
	{
		if (!dayornight)
		{
			sprintf_s(prefix, "-11.%02d%s", i, format.c_str());
			morning_min = i;
		}
		else
		{
			sprintf_s(prefix, "-23.%02d%s", i, format.c_str());
			night_min = i;
		}

		filename = base + string(prefix);
		try
		{
			data = file.read(filename);
			return data;
		}
		catch (...)
		{

		}
	}
	//Vector-M radar
	for (int i = 30; i != 60; i++)
	{
		if (!dayornight)
		{
			sprintf_s(prefix, "-11%02d%s", i, format.c_str());
			morning_min = i;
		}
		else
		{
			sprintf_s(prefix, "-23%02d%s", i, format.c_str());
			night_min = i;
		}

		filename = base + string(prefix);
		try
		{
			data = file.read(filename);
			return data;
		}
		catch (...)
		{

		}
	}
#endif

	throw 1;
}

void RadarReader::setCSVWorker(CSVWorker* csvw)
{
	mCsvw = csvw;
}

void RadarReader::radar_processFormats(Sounding &s)
{
	int i;
	for (i = 0; i != N; i++)
	{
		s.setDayOrNight(0);
		try
		{
			data = radar_readFormat(file, morningstr, 0, formats[i]);//file.read(morningstr + formats[i]);
			datam[i] = data;
			s.addData(data, formats[i]);
		}
		catch (...)
		{
			cout << "#2 Error reading format " << formats[i] << endl;
			datam[i] = "";
			s.addData("", formats[i]);
		}
		s.setDayOrNight(1);
		try
		{
			data = radar_readFormat(file, nightstr, 1, formats[i]);
			datan[i] = data;
			s.addData(data, formats[i]);
		}
		catch (...)
		{
			cout << "#3 Error reading format " << formats[i] << endl;
			datan[i] = "";
			s.addData("", formats[i]);
		}
	}

	file.get_first_filename
}

void RadarReader::radar_processLaunch(Sounding &s, LaunchTime &lt1, LaunchTime &lt2,int cnt)
{
	s.setDayOrNight(cnt);
	s.processRAWFile();
	s.processINFOFile();
	if (mSettings[NUMPARAMETERS_STR + 10])
		s.processTAE3File();
	if (mSettings[NUMPARAMETERS_STR + 2])
		s.processKN04File();
	//yyyy.mm.dd hh:mm
	LaunchTime lt;
	lt1.tm_year = year;
	lt1.tm_mon = month;
	lt1.tm_day = day;
	lt2.tm_year = year;
	lt2.tm_mon = month;
	lt2.tm_day = day;
	if (cnt == 0)
	{
		lt = lt1;
	}
	else
	{
		lt = lt2;
	}

	MarkGen& mg = MarkGen::Instance();

	LaunchParameters l;
	l.radarCode = mRadarPrefix;

	int t1 = s.getRAWSoundingTime();
	int t2 = s.getSoundingTime();

	mg.setSoundingTime(t2);

	push_back_params(l, s, mSettings, error_str);

	if (t1 || t2)
		mCsvw->addLaunch(cnt, lt, l);
	printSoundingInformation(cnt, s);
	mg.clearCalcs();
}

void RadarReader::radar_makeDayNightStrings()
{
	ss.clear();
	ss = stringstream();

	if (mRadarNumber != 2)
	{
		ss << base << day << '.' << month << '.' << year;// << "-11.30";
	}
	else
	{
		char date[10];
		sprintf_s(date, "%04d%02d%02d", year, month, day);
		ss << base << string(date);
	}

	morningstr = ss.str();
	nightstr = morningstr;

	cout << morningstr << endl;
}

void RadarReader::processMonth(int m, int y)
{
	month = m;
	year = y;
	int i;
	for (day = 1; day <= daysInMonth(month,year); day++)
	{
		Sounding s; // ����� "������������" �� ���� ����.
		s.setRAWDataIdentifier(mRawPrefix); //������������� ������� ����� RAW ��� ��� �� ����������������
		
		radar_makeDayNightStrings(); //���������� ������ 11:30 � 22:30
		radar_processFormats(s); //������� ��� ������� �� ����

		LaunchTime lt1;
		lt1.tm_hour = 11;
		lt1.tm_min = 30;
		LaunchTime lt2;
		lt2.tm_hour = 23;
		lt2.tm_min = 30;

		for (int cnt = 0; cnt <= 1; cnt++)
		{
			radar_processLaunch(s, lt1, lt2, cnt); //���������� ����
		}
	}
}

void RadarReader::processMonthInd(int m, int y)
{
	month = m;
	year = y;
	int i;
	for (day = 1; day <= daysInMonth(month, year); day++)
	{
		Sounding s; // ����� "������������" �� ���� ����.
		s.setRAWDataIdentifier(mRawPrefix); //������������� ������� ����� RAW ��� ��� �� ����������������

		
	}
}
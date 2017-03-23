#ifndef SOUNDING_H
#define SOUNDING_H

#include "stdafx.h"
#include "RAWData.h"

class Sounding
{
public:
	Sounding()
	{
		mRawData = ".RAW";
		infozond1 = infozond2 = 0;
	};
	~Sounding()
	{

	};
	
	void setRAWDataIdentifier(string rdata);
	void setDayOrNight(char dayornight);
	void addData(string data, string format);
	bool hasFormat(string format);
	string getFormat(string format);
	int checkFormats();
	string getFormatsTelegram();
	int getSoundingTime();
	int getRAWSoundingTime();
	int getMaxAltitude();
	int getMaxDistance();
	int getKN04Code();
	int getInfoZondType();
	float getMinElevation();
	float getGroundTemperature();
	float getMinTemperature();
	float getMinTemperature(float H);
	int getAlt10Elevation();
	void processRAWFile();
	void processKN04File();
	void processTAE3File();
	void processINFOFile();
private:
	RAWData rDaM;  /*morning*/
	RAWData rDaN; /* night */
	int dayornight;
	string mRawData;
	string getRAWfile();
	string getCRDfile();
	string getKN04file();
	string getINFOfile();
	int KN04Code1;
	int KN04Code2;
	int infozond1;
	int infozond2;
	map<string, string> mDataDay;
	map<string, string> mDataNight;
};

#endif
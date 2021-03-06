/*

����� � ������� �� �������� ������������. �� ���� ����!!!

*/

#ifndef DAYFORMATS
#define DAYFORMATS
#include "stdafx.h"
#include "LaunchParameters.h"
#include "RAWData.h"
#include "TAE3.h"

class DayFormats
{
public:
	DayFormats()
	{
		KN04Code = 0;
		infozond = 0;
	};
	~DayFormats(){};
	void addData(string data, string format);
	void setLaunchTime(LaunchTime lt);
	bool hasFormat(string format);
	string getFormat(string format);
	int checkFormats();
	string getFormatsTelegram();
	RAWData rDa;
	TAE3 tDa;
	int KN04Code;
	int infozond;
	int longitude;
private:
	LaunchTime mLt;
	map<string, string> mData;
};
#endif
#pragma once

#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

class RewardData
{
private:
	RewardData();
	~RewardData();

	static RewardData* _instance;

public:
	static RewardData* getInstance();
	static bool purgeInstance(); //Çå³ýÊµÀý

	rapidjson::Document preparePrase();
	void copyFileToPath(std::string path);

	void praseFinish(rapidjson::Document &document);
	void praseJsonData();

public:
	int getPower(int index);
	int getLove(int index);
	int getCoins(int index);
	int getOffsetX(int index);
	int getOffsetY(int index);
	int getRefresh(int index);
	int getBoom(int index);

	int getBlue(int index);
	int getPurple(int index);
	int getGreen(int index);
	int getRad(int index);
	int getYellor(int index);

	int getIsFristPlay(int index);
	void setIsFristPlay(int level);

private:
	string path;

	vector <int> m_power;
	vector <int> m_love;
	vector <int> m_coins;
	vector <int> m_offsetX;
	vector <int> m_offsetY;
	vector <int> m_refresh;
	vector <int> m_boom;
	//
	vector <int> m_blue;
	vector <int> m_purple;
	vector <int> m_green;
	vector <int> m_rad;
	vector <int> m_yellor;
	vector <int> m_isFristPlay;
};
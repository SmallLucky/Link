#pragma once

#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

/*
	set��get�ؿ�����������Ŀ���������ʷ�߷֣���ʯ������������������������

*/
class GameData
{
public:
	static GameData* getInstance();
	static bool purgeInstance(); //���ʵ��

	rapidjson::Document preparePrase();
	void copyFileToPath(std::string path);

	void praseFinish(rapidjson::Document &document);
	void praseJsonData();

public:
	void setCurLevel(int _level);
	int getCurLevel(){ return cur_level; }

	void setCurScore(int _score);
	int getCurSocre() { return cur_score; }

	void setHistoryScore(int _socre);
	int getHistoryScore(){ return history_score; }

	void setDiamondNum(int _num);
	int getDiamondNum(){ return diamond_num; }

	void setLoveNum(int _num);
	int	 getLoveNum(){ return love_num; };

	void setPowerNum(int _num);
	int	 getPowerNum(){ return power_num; };

	void setMoneyNum(int _num);
	int	 getMoneyNum(){ return money_num; };

	void setLock(int level_index);
	int  getLock(int level);

	int getTargetScore(int level);
	int getCount(int level);

	//���x��y��ƫ����
	int getOffsetX(int level);
	int getOffsetY(int level);

private:
	GameData();
	~GameData();

	static GameData* _instance;

public:
	vector <int> m_lock;
private:
	int cur_level;		//��ǰ�ؿ�
	int cur_score;		//��ǰ����
	int history_score;	//��ʷ��߷�
	int diamond_num;	//��ʯ����
	int love_num;
	int money_num;
	int power_num;
	int refresh_num;
	int boom_num;


	string path;

	vector <int> m_targetScore;
	vector <int> m_offsetX;
	vector <int> m_offsetY;
	vector <int> m_count;
};


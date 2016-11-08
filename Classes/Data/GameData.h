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
	set、get关卡数，分数，目标分数，历史高分，钻石数，步数，体力，道具数量

*/
class GameData
{
public:
	static GameData* getInstance();
	static bool purgeInstance(); //清除实例

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
	int getShopCoins(int index);
	//获得x，y的偏移量
	int getOffsetX(int level);
	int getOffsetY(int level);

	int getRefresh(){ return refresh_num; };
	void setrefresh(int num);
	int getBoomb(){ return boom_num; }
	void setBoomb(int num);

private:
	GameData();
	~GameData();

	static GameData* _instance;

public:
	vector <int> m_lock;
	vector <int> m_coins;
private:
	int cur_level;		//当前关卡
	int cur_score;		//当前分数
	int history_score;	//历史最高分
	int diamond_num;	//钻石数量
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
	//vector <int> m_coins;
};


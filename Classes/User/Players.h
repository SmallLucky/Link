#pragma once

#include "Global.h"
#include "Unit/PlayerUnit.h"

class Players : public Node
{
protected:
	Map<int, PlayerUnit*> allies;
	Map<int, PlayerUnit*> enemies;
	PlayerUnit* master; //主控玩家
	int masterId; //主控玩家编号

	PlayerUnit* createPlayer(int id, int score = 0); //添加玩家

public:
	CREATE_FUNC(Players);
	bool init();

	PlayerUnit* createAlly(int id, int score = 0); //添加队友
	PlayerUnit* createEnemy(int id, int score = 0); //添加对手

	PlayerUnit* findPlayer(int id); //通过编号获得玩家指针

	bool setMaster(int id); //设置主控玩家

	void setScore(int score); //设置玩家分数
	void setScore(int id, int score); //设置玩家分数
	int getScore(); //返回玩家分数
	int getScore(int id); //返回玩家分数
	int scoreUp(int delta); //玩家分数提高，返回提高后的分数
	int scoreUp(int id, int delta); //玩家分数提高，返回提高后的分数
};
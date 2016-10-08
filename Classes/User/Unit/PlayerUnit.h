#pragma once

#include "Global.h"
#include "Data/Data.h"

class PlayerUnit : public Node
{
protected:
	int id; //玩家编号

	int score; //玩家分数

public:
	CREATE_FUNC(PlayerUnit);
	bool init();

	void setId(int i); //设置玩家编号

	void setScore(int s = 0); //设置玩家分数
	int getScore(); //返回玩家分数
	int scoreUp(int delta); //玩家分数提高，返回提高后的分数
};
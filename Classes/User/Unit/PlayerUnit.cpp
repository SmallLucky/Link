#include "PlayerUnit.h"

bool PlayerUnit::init()
{
	if (!Node::init())
	{
		return false;
	}
	id = -1;
	score = 0;
	return true;
}

//设置玩家编号
void PlayerUnit::setId(int i)
{
	id = i;
}

//设置玩家分数
void PlayerUnit::setScore(int s /* = 0 */)
{
	score = s;
}

//返回玩家分数
int PlayerUnit::getScore()
{
	return score;
}

//玩家分数提高，返回提高后的分数
int PlayerUnit::scoreUp(int delta)
{
	score += delta;
	return score;
}
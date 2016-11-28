#include "Players.h"

bool Players::init()
{
	if (!Node::init())
	{
		return false;
	}
	master = nullptr;
	return	true;
}

//添加玩家
PlayerUnit* Players::createPlayer(int id, int score /* = 0 */)
{
	PlayerUnit* p = PlayerUnit::create();
	p->setId(id);
	p->setScore(score);
	addChild(p);

	return p;
}

//添加队友
PlayerUnit* Players::createAlly(int id, int score /* = 0 */)
{
	PlayerUnit* p = createPlayer(id, score);
	allies.insert(id, p);

	return p;
}

//添加对手
PlayerUnit* Players::createEnemy(int id, int score /* = 0 */)
{
	PlayerUnit* p = createPlayer(id, score);
	enemies.insert(id, p);

	return p;
}

//通过编号获得玩家指针
PlayerUnit* Players::findPlayer(int id)
{
	PlayerUnit* p = allies.at(id);

	if (!p)
		p = enemies.at(id);

	if (!p)
		return nullptr;

	return p;
}

//设置主控玩家
bool Players::setMaster(int id)
{
	PlayerUnit* p = findPlayer(id);
	if (!p)
		return false;

	master = p;
	masterId = id;
	return true;
}

//设置玩家分数
void Players::setScore(int score)
{
	setScore(masterId, score); //默认设置主控玩家分数
}
void Players::setScore(int id, int score)
{
	PlayerUnit* p = findPlayer(id);
	if (!p)
		return;

	p->setScore(score);
}

//返回玩家分数
int Players::getScore()
{
	return getScore(masterId); //默认获取主控玩家分数
}
int Players::getScore(int id)
{
	PlayerUnit* p = findPlayer(id);
	if (!p)
		return -1;

	return p->getScore();
}

//玩家分数提高，返回提高后的分数
int Players::scoreUp(int delta)
{
	return scoreUp(masterId, delta); //默认设置主控玩家分数
}
int Players::scoreUp(int id, int delta)
{
	PlayerUnit* p = findPlayer(id);
	if (!p)
		return -1;

	return p->scoreUp(delta);
}
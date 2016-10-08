#include "StateList.h"

//游戏结束状态

bool GameOverState::checkStateChange() //状态改变返回true，状态未改变返回false，
{
	return false;
}

bool GameOverState::entryState()
{
	scene->removeLink(); //将当前触摸连接的元素消除
	scene->showScoreUp();
	scene->forbiddenLink(); //不再允许触摸

	scene->gameOver(); //游戏结束

	return true;
}

void GameOverState::onUpdate(float dt)
{
	time += dt;
}

bool GameOverState::exitState()
{
	return true;
}
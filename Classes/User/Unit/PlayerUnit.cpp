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

//������ұ��
void PlayerUnit::setId(int i)
{
	id = i;
}

//������ҷ���
void PlayerUnit::setScore(int s /* = 0 */)
{
	score = s;
}

//������ҷ���
int PlayerUnit::getScore()
{
	return score;
}

//��ҷ�����ߣ�������ߺ�ķ���
int PlayerUnit::scoreUp(int delta)
{
	score += delta;
	return score;
}
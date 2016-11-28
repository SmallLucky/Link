#include "PlayerData.h" //������йص���Ϣ

const int MASTER_ID = 0; //������ұ��

const char* HIGH_SCORE_UD = "high_score"; //��߷���UserDefault�еı�ǩ

//ͨ��һ�����������ĸ���ȷ���÷�
int getScoreByLinkCount(int count, int ele)
{
	int ratio = 1;
	if (count <= 5)
	{
		ratio = 1;
	}
	if (count >5 && count <= 9)
	{
		ratio = 1.2;
	}
	if (count > 9 && count <= 14)
	{
		ratio = 1.3;
	}
	if (count > 14 && count <= 19)
	{
		ratio = 1.4;
	}
	if (count > 19)
	{
		ratio = 1.5;
	}
	int score = 0;
	switch (ele)
	{
	case 0:

		score = count * 50 * ratio;
		break;
	case 1:
		score = count * 55 * ratio;
		break;
	case 2:
		score = count * 60 * ratio;
		break;
	case 3:
		score = count * 65 * ratio;
		break;
	case 4:
		score = count * 70 * ratio;
		break;
	default:
		break;
	}
	//log("elemntType:%d,count:%d,score:%d", ele, count, score);
	return score ; //count * count * 10
}
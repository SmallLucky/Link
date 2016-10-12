#include "PlayerData.h" //与玩家有关的信息

const int MASTER_ID = 0; //主控玩家编号

const char* HIGH_SCORE_UD = "high_score"; //最高分在UserDefault中的标签

//通过一次连接消除的个数确定得分
int getScoreByLinkCount(int count, int ele)
{
	//log(" getScoreByLinkCount:elemntType:%d",ele);
	int score;
	switch (ele)
	{
	case 0:
		score = count * 50 * 1;
		break;
	case 1:
		score = count * 55 * 1.2;
		break;
	case 2:
		score = count * 60 * 1.5;
		break;
	case 3:
		score = count * 65 * 2;
		break;
	case 4:
		score = count * 70 * 2.5;
		break;
	default:
		score = count * 100 * 2.5;
		break;
	}
	//log("elemntType:%d,count:%d,score:%d", ele, count, score);
	return score ; //count * count * 10
}
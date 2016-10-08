#pragma once

#include "Global.h"


//与玩家有关的信息

extern const int MASTER_ID; //主控玩家编号

extern const char* HIGH_SCORE_UD; //最高分在UserDefault中的标签

int getScoreByLinkCount(int count, int ele); //通过一次连接消除的个数确定得分
#pragma once

#include "Global.h"

//与游戏信息有关的游戏数据

extern const string INFORMATION_FONT; //游戏信息字体
extern const int INFORMATION_FONTSIZE;//游戏信息字体大小
extern const Size INFORMATION_SHADOW_OFFSET;//游戏信息阴影偏移量

extern const Point TIME_POSITION; //时间信息位置
extern const Point TIME_ANCHOR; //时间信息锚点
extern const float TIME_SIZE; //时间信息大小
extern const Color4B TIME_NORMAL_COLOR; //剩余时间充裕时的颜色
extern const Color4B TIME_LITTLE_COLOR; //剩余时间较少时的颜色

extern const Point SCORE_POSITION; //分数信息位置
extern const Point SCORE_ANCHOR; //分数信息锚点
extern const float SCORE_SIZE; //分数信息大小
extern const Color4B SCORE_COLOR; //分数信息颜色

extern const Point SCORE_UP_POSITION; //分数增长信息位置
extern const Point SCORE_UP_ANCHOR; //分数增长信息锚点
extern const float SCORE_UP_SIZE; //分数增长信息大小
extern const Color4B SCORE_UP_COLOR; //分数增长信息颜色
extern const float SCORE_UP_TIME; //分数增长信息持续时间

extern const Color4B GAME_OVER_COLOR; //游戏结束标签颜色
extern const int GAME_OVER_SIZE; //游戏结束信息大小（像素）
extern const Size GAME_OVER_SHADOW_OFFSET; //游戏信息阴影偏移量

extern const Point COUNT_POSITION; //步数信息位置
extern const Point COUNT_ANCHOR;	//步数信息锚点
extern const float COUNT_SIZE;	//步数信息的大小
extern const  Color4B COUNT_NORMAL_COLOR; //步数充裕的颜色
extern const  Color4B COUNT_LITTLE_COLOR; //步数较少的颜色

extern const Point POWER_POSITION; //体力信息的位置
extern const Point POWER_ANCHOR; //体力信息的锚点
extern const float POWER_SIZE; //体力信息的大小
extern const Color4B POWER_COLOR; //体力信息的颜色
#include "InformationData.h" //与游戏信息有关的游戏数据

const string INFORMATION_FONT = "fonts/Marker Felt.ttf"; //游戏信息字体
const int INFORMATION_FONTSIZE = 100; //游戏信息字体大小
const Size INFORMATION_SHADOW_OFFSET = Size(3, 3); //游戏信息阴影偏移量

const Point TIME_POSITION = Point(0, HEIGHT * 0.95); //时间信息位置
const Point TIME_ANCHOR = Point(0, 0.5); //时间信息锚点
const float TIME_SIZE = 0.05f; //时间信息大小
const Color4B TIME_NORMAL_COLOR = Color4B(0, 255, 0, 255); //剩余时间充裕时的颜色
const Color4B TIME_LITTLE_COLOR = Color4B(255, 0, 0, 255); //剩余时间较少时的颜色

const Point SCORE_POSITION = Point(WIDTH / 2, HEIGHT * 0.88f); //分数信息位置
const Point SCORE_ANCHOR = Point(0.5, 0.5); //分数信息锚点
const float SCORE_SIZE = 0.06f; //分数信息大小
const Color4B SCORE_COLOR = Color4B(0, 0, 255, 255); //分数信息颜色

const Point SCORE_UP_POSITION = Point(WIDTH / 2, HEIGHT / 2); //分数增长信息位置
const Point SCORE_UP_ANCHOR = Point(0.5, 0.5); //分数增长信息锚点
const float SCORE_UP_SIZE = 0.1f; //分数增长信息大小
const Color4B SCORE_UP_COLOR = Color4B(0, 255, 0, 255); //分数增长信息颜色
const float SCORE_UP_TIME = 1.0f; //分数增长信息持续时间

const Color4B GAME_OVER_COLOR = Color4B(0, 255, 255, 255); //游戏结束标签颜色
const int GAME_OVER_SIZE = 60; //游戏结束信息大小（像素）
const Size GAME_OVER_SHADOW_OFFSET = Size(5, 5); //游戏信息阴影偏移量

const Point COUNT_POSITION = Point(0, HEIGHT * 0.95); //步数信息位置
const Point COUNT_ANCHOR = Point(0,0.5);	//步数信息锚点
const float COUNT_SIZE = 0.05f;	//步数信息的大小
const  Color4B COUNT_NORMAL_COLOR = Color4B(0, 255, 0, 255); //步数充裕的颜色
const  Color4B COUNT_LITTLE_COLOR = Color4B(255, 0, 0, 255); //步数较少的颜色

const Point POWER_POSITION = Point(WIDTH * 0.50, HEIGHT * 0.95); //体力信息的位置
const Point POWER_ANCHOR = Point(0, 0.5); //体力信息的锚点
const float POWER_SIZE = 0.05f; //体力信息的大小
const Color4B POWER_COLOR = Color4B(255, 0, 0, 255); //体力信息的颜色

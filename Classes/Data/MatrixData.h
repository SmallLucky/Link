#pragma once

#include "Global.h"

//与元素矩阵有关的游戏数据


extern const int MATRIX_LINE; //矩阵行数
extern const int MATRIX_ROW;  //矩阵列数
extern const int MATRIX_MLINE; //矩阵行数
extern const int BLOCK_NUM;		//矩阵格子数

extern const float BLOCK_WIDTH; //格子宽度（像素）
extern const float BLOCK_HEIGHT; //格子高度
extern const float TOUCH_RADIUS; //可触控区域半径相对格子大小

extern const Point MATRIX_CENTER; //矩阵中心
extern const float MATRIX_LEFT;  //矩阵左边缘
extern const float MATRIX_RIGHT;	//矩阵右边缘
extern const float MATRIX_BOTTOM; //矩阵底边缘
extern const float MATRIX_TOP; //矩阵上边缘

extern const Point MATRIX_BOTTOM_LEFT;  //矩阵左下角
extern const Point MATRIX_BOTTOM_LEFT_CENTER; //左下角格子中心
extern const Point MATRIX_TOP_RIGHT; //矩阵右上角
extern const Point MATRIX_TOP_RIGHT_CENTER; //右上角格子中心

extern const Rect MATRIX_RECT; //矩阵区域

extern const float GUIDELINE_WIDTH; //矩阵外围辅助线宽度
extern const Color4F GUIDELINE_COLOR; //矩阵外围辅助线颜色

extern const int MAX_GAP; //元素类型数量最多元素与最少元素的最大差距

Point getCenterByCoord(int line, int row); //通过坐标获得格子中心
Point getCenterByCoord(Coord c); //通过坐标获得格子中心

//遍历所有的格子

//#define ERGODIC_BLOCK(line,row)			for(int (line)=0 ; (line)<MATRIX_LINE; ++(line)) for(int (row) = 0; (row)<MATRIX_ROW;++(row))

#define ERGODIC_MBLOCK(row,line)  for(int (row) = 0; (row)<MATRIX_ROW; (row)++ )  for(int (line) = MATRIX_LINE ; (line) >= 0 ;--(line))  //line -- row ++

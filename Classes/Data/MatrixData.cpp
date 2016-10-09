
#include "MatrixData.h"

 const int MATRIX_LINE =5; //矩阵行数
 const int MATRIX_ROW = 6;  //矩阵列数
 const int MATRIX_MLINE = 6; //矩阵行数
 const int BLOCK_NUM = MATRIX_MLINE * MATRIX_ROW;		//矩阵格子数

 const float BLOCK_WIDTH = DATUM*0.055f; //格子宽度（像素）
 const float BLOCK_HEIGHT = DATUM*0.055f; //格子高度
 const float TOUCH_RADIUS = 0.8f; //可触控区域半径相对格子大小

 const Point MATRIX_CENTER = visibleSize/2; //矩阵中心
 const float MATRIX_LEFT = MATRIX_CENTER.x - BLOCK_WIDTH*(float(MATRIX_ROW) / 2);  //矩阵左边缘
 const float MATRIX_RIGHT = MATRIX_CENTER.x + BLOCK_WIDTH*(float(MATRIX_ROW) / 2);	//矩阵右边缘
 const float MATRIX_BOTTOM = MATRIX_CENTER.y - BLOCK_HEIGHT*(float(MATRIX_LINE) / 2); //矩阵底边缘
 const float MATRIX_TOP = MATRIX_CENTER.y + BLOCK_HEIGHT* (float(MATRIX_LINE) / 2); //矩阵上边缘

 const Point MATRIX_BOTTOM_LEFT = Point(MATRIX_LEFT,MATRIX_BOTTOM);  //矩阵左下角
 const Point MATRIX_BOTTOM_LEFT_CENTER = MATRIX_BOTTOM_LEFT + Point(BLOCK_WIDTH / 2, BLOCK_HEIGHT / 2); //左下角格子中心
 const Point MATRIX_TOP_RIGHT = Vec2(MATRIX_RIGHT, MATRIX_TOP); //矩阵右上角
 const Point MATRIX_TOP_RIGHT_CENTER = MATRIX_TOP_RIGHT - Point(BLOCK_WIDTH / 2, BLOCK_HEIGHT / 2); //右上角格子中心

 const Rect MATRIX_RECT = Rect(MATRIX_BOTTOM_LEFT.x, MATRIX_BOTTOM_LEFT.y, BLOCK_WIDTH*MATRIX_ROW, BLOCK_HEIGHT*MATRIX_LINE); //矩阵区域

 const float GUIDELINE_WIDTH = 2.5f; //矩阵外围辅助线宽度
 const Color4F GUIDELINE_COLOR = Color4F(0, 1, 0, 0.16f); //矩阵外围辅助线颜色

const int MAX_GAP = 10; //元素类型数量最多元素与最少元素的最大差距

Point getCenterByCoord(int line, int row) //通过坐标获得格子中心70
{
	Point center = MATRIX_BOTTOM_LEFT_CENTER;
	center += Point(BLOCK_WIDTH*row, BLOCK_HEIGHT*line);
	return center;
}
Point getCenterByCoord(Coord c)//通过坐标获得格子中心
{
	return getCenterByCoord(c.line, c.row);
}
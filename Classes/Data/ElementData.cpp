#include "ElementData.h" //与单个元素有关的游戏数据
//单个元素有关的游戏数据

 const int ELEMENT_COUNT = 5;			//元素类型数量, "element/star5.png"
 const string ELEMENT_NORMAL_IMG[] = { "element/blue_1.png", "element/green_0.png", "element/purple_0.png", "element/rad_0.png", "element/yellor_0.png", "element/star5.png" };	//各元素未连接状态图片，万能元素编号的是万能元素图片
 const float ELEMENT_NORMAL_SIZE = 0.075f;		//图片尺寸
 const int WILD_NUM = ELEMENT_COUNT;					//万能元素的编号
 const int WILD_PROBABILITY = 20;			//出现万能元素的概率

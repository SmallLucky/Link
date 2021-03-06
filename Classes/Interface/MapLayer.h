#pragma once

#include "Global.h"
#include "CommonFunction.h"
#include "InformationLayer.h"

#include "Expand/TouchableLayer.h"
#include "Unit/ElementUnit.h"
#include "Unit/MyEffect.h"
#include "DrawLine.h"

class MapLayer : public TouchableLayer
{
public:
	MapLayer();
	~MapLayer();
	bool init();
	CREATE_FUNC(MapLayer);

	virtual bool onTouchBegan(Touch *touch, Event *unused_event); //开始触摸
	virtual void onTouchMoved(Touch *touch, Event *unused_event); //触摸点移动
	virtual void onTouchEnded(Touch *touch, Event *unused_event); //触摸结束

	Vec2  positionForTileCoord(const Vec2& tileCoord);
	Vec2  MypositionForTileCoord(const Vec2& tileCoord);

	Point getMCenterByCoord(int row, int line); //通过坐标获得格子中心
	Point getMCenterByCoord(Coord c);//通过坐标获得格子中心

	bool checkIsnullptr(int _row,int _line);

	int getElementType();
	void setElementType(int ele);
	int elementType;

	Coord lastPoint;
	bool isLine;
	bool getIsLine(){ return isLine; };

	//bool isCanAddCountLayer;
	//vector<int> special_row;
	//vector<Coord> special_round;

	void removeBeEliminate(int r, int l);//检测并消除可以被消除的元素，改变type值
	void changeElement(int ele,int r, int l);  //修改周围三乘三的元素
	void undoChangeElement(); //撤销修改的三乘三元素
	vector<int> temp_element; // 暂时存放替换前元素
	vector<Coord> temp_elementPos;

	bool checkIsNeedRefresh();  //检测是否需要进行刷新位置
	void refreshAllElement();	//刷新所有基础元素的位置

	bool checkIsDrop(int r,int l);
	int green;
	int blue;
	int red;
	int yellor;
	int purple;

	//是否是炸弹
	bool isBoom;
	bool isTargetElementFinish;
	bool getIsBoom() { return isBoom; }
	void setIsBoom( bool b) { isBoom = b; }
	//炸弹炸掉的元素
	void isBoomElement(int _row,int _line);
	bool isCount;
	int  getSpecialScore(){  int s = specialScore; specialScore = 0;  return s; };
private:
	int  specialSttlement( int ele); // 
	int  specialScore;
	void  changeTiledType(int r,int l); // 修改指定位置上瓦片的属性值

	Vector<Sprite*> m_effects;
	//Vector<ParticleSystem*> m_effects;
	void showEffect(Point p);
	bool removeEffect();
	void showLineEffect(Point p);
	void showRowEffect(Point p);
	void showBoomEffect(Point p);
	void showCloudEffect(Point p);
	void showLoveEffect(Point p);
	void showRemoveEleEffect(Point p);
	void showQQEffect(Node* node);
	void showRemoveEffect(Point p);

	void eventTargetElement(int ele,int count);

private:

	TMXTiledMap* _tileMap;
	
	TMXLayer* mapLayer;
	TMXLayer* typeLayer;
	Value getType;

	//MyEffect* myeffect;

	Point mapPoint;

	//int line; //行
	//int row; //列

	
	/*vector<Coord> spritenullptr;*/
	
private:
	//ElementUnit* **elements; //格子元素
	//Point **blocksCenter;	//格子中心

	//bool **signFlag;		//是否被标记
	//bool **signFlagSpecial;		//是否被特殊元素标记的
	//bool **signBeRemove;	//标记被消除是否是同一次连线被改变
	bool signBeRemove[6][6];	//标记被消除是否是同一次连线被改变
	bool signFlag[6][6];		//是否被标记
	bool signFlagSpecial[6][6];		//是否被特殊元素标记的
	ElementUnit* elements[6][6]; //格子元素
	Point blocksCenter[6][6];	//格子中心


	void initArray();

	float containsDis;		//触点包含于格子内的最大距离

	//--int *elementCount;		//矩阵中各元素的数量
	//--int *removeCount;		//消除各元素的数量

	vector<Coord> linkIndex; //一次连线中的连线顺序
	vector<DrawLine*> linkBrush;	//连接两个元素的线

	int removeAllCount; //一次连线消除总数
	
	bool touchedFlag;	//是否已经有效的触摸
	bool linkFinishFlag;	//是否有有效的连接结束

	Sprite* Btaskbg;
	Sprite* Ptaskbg;
	Sprite* Gtaskbg;
	Sprite* Rtaskbg;
	Sprite* Ytaskbg;
	int m_blue, m_purple, m_green, m_rad, m_yellor;
	LabelAtlas* targetElementBNum; //目标数标签
	LabelAtlas* targetElementPNum; //目标数标签
	LabelAtlas* targetElementGNum; //目标数标签
	LabelAtlas* targetElementRNum; //目标数标签
	LabelAtlas* targetElementYNum; //目标数标签

public:
	void removeLink();	//符合条件则消除被连接的元素，否则连接失败

	
	void specialSignElement(int ele ,Coord c); //给特殊元素需要消除的元素加上标记

	int getRemoveCount();	//获取上次连接中消除个数，并清零为下次准备


	bool elementsFall();	//元素下落，填补被消除的元素。返回是否填补了空位，每列填补最下一个空位
	bool isLinkFinish();	//是否有效的连接结束
	void prepareLink();		//准备下一次连接

public:
	int m_count = 0;
	int removeMyCount();


	//特效
	void  jitterEffect(Node* node, float delay);
	int		getTargetElementCount(int level);
	void	addTargetElement();
	void	showTargetElementNum(int ele, int n);
	void	initTargetEleNum();
	bool	enoughTargetElement();
	void    finishEffect(Node* node);
	void	effectAction(Node* node, float delay);
public:
	//void drawGuideLine(Point leftBottom, Point rightTop);		//绘制矩形辅助线
	void drawLine(Coord from, Coord to);			//绘制连接两个元素的线
	bool removeLatestLine();					//删除最后一条连接线，返回是否删除成功
	void removeAllLine();						//删除所有线

	bool checkLink(Coord from, Coord to);		//检查是否能连接两个元素，，通过修改该函数即可实现多样化规则

	void linkStart(int row, int line);	//设置连线的起点
	void linkElement(Coord from, Coord to);		//连接两个元素
	void undoLink(); //撤销最后一次连接

	void initBlocks();	//初始化格子，确定格子区域，初始状态矩阵为空
	void clearElement();	//清空矩阵元素

public:
	int createElement(int element, int row, int line);	//在指定位置创建给定类型的元素
private:

	int removeElement(int row, int line);	//消除指定位置的元素，返回元素类型
	int removeSignedElement();	//消除被标记的元素，翻消除个数
	int getElement(int line, int row);		//获取指定位置的元素类型
	int getElement(Coord c);	//获取指定位置元素类型

	void rowFall(int _row, int bottom);		//指定列的全部空位上方元素下落，顶端出现新元素

	/**/
	void rowMyFall(int _row, int bottom);
	void rightAndLeft(int _row, int bottom);

	void appear(int row);		//指定列的顶端出现新元素

public:
	int randElement();		//通过现有元素类型分布，随机获得一个元素类型
private:
	void initSign(); //初始化标记
	int signElement(int row, int line);	//标记元素
	int signElement(Coord c);	//标记元素
	int unsignElement(int row,int line); // 取消标记元素
	int unsignElement(Coord c); //取消标记元素
	void signClear();	//清除所有标记

	int signSpecialElement(Coord c); //标记特殊元素涉及的元素
	int unsignSpecialElement(Coord c); // 取消特殊元素涉及元素的标记
	void removeSignSpecial(); //取消撤销关于这特殊元素涉及的所有标识
	void removeCountCleaar();  //清空消除个数
	void signOnlyBlock(int row, int line);		//只标记一个元素（其他标记被取消）


	void	 initSignBeRemove(); //初始被同一次被消标记
	void	 clearSignBeRemove(); //清除被消标记
};
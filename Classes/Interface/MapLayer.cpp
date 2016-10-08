
#include "MapLayer.h"
//#include <iostream>
//#include <vector>
#include "Unit/ElementUnit.h"
#include "Data/ElementData.h"
 

using namespace cocos2d;
using namespace std;

#define RATIO   0.8

MapLayer::MapLayer() :isLine(false)
{
	//分配格子元素空间
	elements = new ElementUnit* *[MATRIX_ROW];
	for (int i = 0; i < MATRIX_ROW; i++)
	{
		elements[i] = new ElementUnit*[MATRIX_MLINE];
	}

	//分配标记空间
	signFlag = new bool*[MATRIX_ROW];
	for (int i = 0; i < MATRIX_ROW; i++)
	{
		signFlag[i] = new bool[MATRIX_MLINE];
	}

	//分配格子中心空间
	blocksCenter = new Point*[MATRIX_ROW];
	for (int i = 0; i < MATRIX_ROW; i++)
	{
		blocksCenter[i] = new Point[MATRIX_MLINE];
	}
	//分配各元素数量空间
	elementCount = new int[ELEMENT_COUNT + 1]; //注意万能元素空间

	//分配消除个元素的数量空间
	removeCount = new int[ELEMENT_COUNT + 1]; //注意万能元素空间

	//linkBrush.clear();
}
MapLayer::~MapLayer()
{
	//回收格子元素空间
	for (int i = 0; i < MATRIX_ROW; i++)
		delete[] elements[i];
	delete[] elements;
	//回收标记空间
	for (int i = 0; i < MATRIX_ROW; i++)
		delete[] signFlag[i];
	delete[] signFlag;
	//回收格子中心空间
	for (int i = 0; i < MATRIX_ROW; i++)
		delete[] blocksCenter[i];
	delete[] blocksCenter;

	//回收各元素数量空间
	delete[] elementCount;

	//回收消除各元素的数量空间
	delete[] removeCount;
}

bool MapLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Size visibleSize = Director::getInstance()->getVisibleSize();
	mapPoint = { visibleSize.width / 2, visibleSize.height/2 - 50 };

	initBlocks();//初始化格子，确定格子区域，初始状态矩阵为空
	initSign(); //初始化标记

	forbiddenResponse(); //暂时禁止触摸，在进入等待连接状态后通过allowResponse函数允许触摸（继承自TouchableLayer）
	touchedFlag = false; //没有有效的触摸
	linkFinishFlag = false; //没有连接结束

	return true;
}

//符合条件则消除被连接的元素，否则连接失败
void MapLayer::removeLink()
{
	if (linkIndex.size() >= MIN_ROMOVE_COUNT) //如果连接的元素个数达到要求
	{
		removeSignedElement(); //消除被连接的元素
		removeAllLine(); //删除连接线
		linkFinishFlag = true; //完成一次消除
		//if (UserDefault::getInstance()->getBoolForKey(EFFECT_UD, EFFECT_DEFAULT_FLAG))
		//	SimpleAudioEngine::getInstance()->playEffect(REMOVE_EFFECT); //播放音效
		//if (UserDefault::getInstance()->getBoolForKey(VIBRATE_UD, VIBRATE_DEFAULT_FLAG))
		//	Vibrator::vibrate(REMOVE_VIBRATOR_TIME); //震动
		if (getIsLine())
		{
			removeSignedElement(); //消除被连接的元素
		}
		else
		{
			if (linkIndex.size() >= 4)
			{
				lastPoint = linkIndex.back();
				createSecial(lastPoint, getElementType());
			}
		}
		isLine = false;
		signClear();
		//成功消除一次，步数减少一次
		//removeMyCount();
	}
	else //如果连接的元素个数未达到要求
	{
		signClear(); //取消对元素的标记
		removeAllLine(); //删除连接线
		//if (UserDefault::getInstance()->getBoolForKey(EFFECT_UD, EFFECT_DEFAULT_FLAG))
		//	SimpleAudioEngine::getInstance()->playEffect(WRONG_REMOVE_EFFECT); //播放音效
		//if (UserDefault::getInstance()->getBoolForKey(VIBRATE_UD, VIBRATE_DEFAULT_FLAG))
		//	Vibrator::vibrate(WRONG_REMOVE_VIBRATOR_TIME); //震动
	}

	linkIndex.clear(); //清空连线序列
}

//消除横向或者竖向特殊元素。
void  MapLayer::removeLineOrRow()
{
	if (getIsLine())
	{
		for (int i = 0; i < 6; i++)
		{
			if (signElement({ i, special_Line }))
			{
				continue;
			}
			signElement({i,special_Line});
			if (getElement({ i, line }) == 5)
			{
				unsignElement({ i, line });
				//typeLayer->setTileGID(1, Vec2(i, line));
			}
		}
	}//标记需要消除的元素

	removeSignedElement();

}

//消除被标记的元素，返回消除个数
int  MapLayer::removeSignedElement()
{
	removeAllCount = 0;
	ERGODIC_MBLOCK(row,line)
	{
		if (signFlag[row][line])
		{
			if (getElement({ row, line }) >= 10)//只走一次
			{
				isLine = true;
				log("removeSignedElement:%d", getElement({ row, line }));
				if (getElement({ row, line }) < 20)
				{
					for (int i = 0; i < 6; i++)
					{
						if (signElement({ i, line }))
						{
							continue;
						}
						signElement({ i, line });
						if (getElement({ i, line }) == 5)
						{
							unsignElement({ i, line });
						}
					}
				}if (getElement({ row, line }) >= 20)
				{
					if (getElement({ row, line }) < 30)
					{
						for (int i = 0; i < 8; i++)
						{
							if (signElement({ row, i }))
							{
								continue;
							}
							signElement({ row, i });
							if (getElement({ row, i }) == 5)
							{
								unsignElement({ row, i });
							}
						}
					}
					if (getElement({ row, line }) >= 30)
					{
						ERGODIC_MBLOCK(row, line)
						{
							//switch (getElement({}))
							//{
							//default:
							//	break;
							//}
						}
					}
				}

			}
			removeCount[removeElement(row, line)]++; //消除标记的元素
			removeAllCount++;
		}
	}
	//signClear();
	return removeAllCount;
}

//删除所有线
void MapLayer::removeAllLine()
{
	while (removeLatestLine());  //不断删除线序列中最后一条连接线，直至为空
}

//消除指定位置的元素，返回元素类型
int MapLayer::removeElement(int row,int line)
{
	ElementUnit* &temp = elements[row][line]; //临时变量。简化代码
	if (!temp)
	{
		return -1; //指定位置五元素，则不操作
	}

	int ele = temp->getElement(); //获得元素类型
	elementType = ele;//获得元素类型
	elementCount[ele]--; //对应元素类型计数自增
	temp->disappear(FALL_TIME);//元素缩小至消失
	temp = nullptr;
	signFlag[row][line] = false;
	return ele;
}

int MapLayer::getElementType()
{
	return elementType;
}

int MapLayer::removeMyCount()
{
	log("m_count:%d", m_count);
	return ++m_count;
}

//获取上次连接中消除个数，并清零为下次准备
int MapLayer::getRemoveCount()
{
	int r = removeAllCount;
	removeAllCount = 0;
	return r;
}

//元素下落，填补被消除的元素。返回是否填补了空位，每列填补最下一个空位
bool MapLayer::elementsFall()
{
	bool flag = false; //是否填补了空位
	for (int i = 0; i < MATRIX_ROW; ++i)//对每一列单独处理 0
	{
		for (int j = MATRIX_LINE; j >= 0; --j)//7
		{
			if (elements[i][j] == nullptr)//07
			{
				//log("MapLayer::elementsFall:%i,%i",i,j);
				rowFall(i, j);//从最下方开始，一旦出现空位，所有上方元素下落，填补空位
				flag = true;//填补了空位
				break;//每一列只填补最下一个空位
			}
		}
	}
	return flag;
}

bool MapLayer::checkIsNull(int _row, int _line)
{
	unsigned int num;
	num = typeLayer->getTileGIDAt(Vec2(_row, _line));
	getType = _tileMap->getPropertiesForGID(num);
	int _type = getType.asValueMap()["type"].asInt();
	if (_type == 2)
	{
		log("_type == 2");
		return true;
	}
	return false;
}

//是否有效的连接结束
bool MapLayer::isLinkFinish()
{
	return linkFinishFlag;
}

//准备下一次连接
void MapLayer::prepareLink()
{
	linkFinishFlag = false;
}

//指定列的全部空位上方元素下落，顶端出现新元素i.j
void MapLayer::rowFall(int _row, int bottom)//0,7
{
	for (int i = bottom; i > 0; )//7--i
	{
		int j = i - 1;//6
		if (checkIsNull(_row,i))
		{
			--i;
			continue;
		}
		while (checkIsNull(_row, j))//06
		{
			//log("while (checkSpriteNull({ _row, j })):%i",j);
			j--;
		}
		elements[_row][i] = elements[_row][j]; //null

		if (elements[_row][i])
		{
			Point pop = getMCenterByCoord(_row, i);
			elements[_row][i]->moveToPosition(pop, FALL_TIME);//上方元素掉落一个
		}
		i = j;
	}
	appear(_row);
} 

//初始化格子，确定格子区域，初始状态矩阵为空
void  MapLayer::initBlocks()
{
	string str = StringUtils::format("tiledmap/map_%d.tmx", GAMEDATA->getCurLevel());
	_tileMap = TMXTiledMap::create(str);//("TileMaps/hexa-test.tmx");
	//_tileMap = TMXTiledMap::create("tiledmap/map_10.tmx");//("TileMaps/hexa-test.tmx");
	_tileMap->setAnchorPoint(Vec2(0.5, 0.5));
	_tileMap->setPosition(mapPoint);
	_tileMap->setScale(RATIO);
	addChild(_tileMap, 0);

	mapLayer = _tileMap->getLayer("map");
	typeLayer = _tileMap->getLayer("type");
	typeLayer->setVisible(false);
	
	line = _tileMap->getMapSize().height; // 10
	row = _tileMap->getMapSize().width; // 8
	log("***line:%i ,row:%i",line,row);

	////mei shi xian
	containsDis =(typeLayer->getMapTileSize().width*RATIO)/2 - 5;//(_tileMap->getTileSize().width + _tileMap->getTileSize().height) / 4 * TOUCH_RADIUS; // 48*48
	log("%f", containsDis);

	ERGODIC_MBLOCK(row,line)
	{
		log("*********************row : %d, line : %d", row, line);
		blocksCenter[row][line] = getMCenterByCoord(row, line); //确定格子中心位置
			/*	int  num = (uint32_t)typeLayer->getTileGIDAt(Vec2(i, j));*/
				//getType = _tileMap->getPropertiesForGID(num);
				//int _type = getType.asValueMap()["type"].asInt();
		if (checkIsNull(row, line))
		{
			//这块瓦片是不显示 
			auto s = ElementUnit::create();
			s->createElement(5, getMCenterByCoord(row, line));
			elements[row][line] = s;
			elementCount[5]++;
			addChild(elements[row][line], 1);
		}
		else
		{
			elements[row][line] = nullptr;
		}
	}

	//初始化数量
	for (int i = 0; i < 6; ++i)
	{
		log("elementCount[i]:%i",i);
		elementCount[i] = 0;
		removeCount[i] = 0;
	}
	removeAllCount = 0;
}

// tile坐标转成瓦片格子中心的OpenGL坐标  //获得瓦片锚点的坐标
Vec2 MapLayer::positionForTileCoord(const Vec2& tileCoord) //0,0
{
	Size mapSize = _tileMap->getMapSize(); // w:8 h:10
	Size tileSize = _tileMap->getTileSize();//w:100 h:100
	int x = ((tileCoord.x )* tileSize.width*RATIO) + (mapPoint.x - mapSize.width / 2 * tileSize.width*RATIO); //0+(240-4*48)=48
	int y = ((mapSize.height - tileCoord.y - 1) * tileSize.height*RATIO) + (mapPoint.y - mapSize.height / 2 * tileSize.height*RATIO); // 9*48 + 160=592
	//log("positionForTileCoord:x:%i,y:%i",x,y);
	return Vec2(x, y); 
}

// tile坐标转成瓦片格子中心的OpenGL坐标  
Vec2 MapLayer::MypositionForTileCoord(const Vec2& tileCoord)
{
	Size mapSize = _tileMap->getMapSize();
	Size tileSize = _tileMap->getTileSize();
	int x = tileCoord.x * tileSize.width + tileSize.width / 2;
	int y = (mapSize.height - tileCoord.y)*tileSize.height - tileSize.height / 2;
	return Vec2(x, y);
}

//初始化标记
void MapLayer::initSign()
{
	signClear();
}

//清除所有标记
void  MapLayer::signClear()
{
	ERGODIC_MBLOCK(row,line)
	{
		unsignElement(row, line);
	}
}

// 取消标记元素
int MapLayer::unsignElement(int row,int line)
{
	if (!elements[row][line])
	{
		return -1;  //如果指定位置无元素，则不操作
	}
	signFlag[row][line] = false;
	return 0;
}

//取消标记元素
int MapLayer::unsignElement(Coord c)
{
	return unsignElement(c.row, c.line);
}

//标记元素
int MapLayer::signElement(int row, int line)
{
	if (!elements[row][line])
	{
		return -1;
	}
	signFlag[row][line] = true;

	return 0;
}

//标记元素
int MapLayer::signElement(Coord c)
{
	return signElement(c.row, c.line);//调用重载函数
}

bool MapLayer::onTouchBegan(Touch *touch, Event *unused_event) //开始触摸
{
	if (isResponse())//如果允许触摸
	{
		if (touchedFlag)
		{
			return false;//若已有有效触摸，则忽略此次触摸
		}
		Point touchPoint = Director::getInstance()->convertToGL(touch->getLocationInView());//获得触摸点

		ERGODIC_MBLOCK(row,line)
		{
			if (blocksCenter[row][line].getDistance(touchPoint) < containsDis)//如果触摸点在格子有效半径内
			{
				log("element:%i", getElement({ row, line }));
				if (getElement({ row, line }) == 5)
				{
					log("wild");
					touchedFlag = false;//成为有效触摸
					return false;
				}
				else
				{
					linkStart(row, line);//该格子作为连线起点
					touchedFlag = true;//成为有效触摸
					return true; //对该次触摸的后续操作做出反映
				}
			}
		}

	}

	return true;//若不允许触摸或未触摸任何格子，则为无效触摸，忽略后续操作
}
void MapLayer::onTouchMoved(Touch *touch, Event *unused_event) //触摸点移动
{
	if (isResponse()) //如果允许触摸
	{
		Point touchPoint = Director::getInstance()->convertToGL(touch->getLocationInView()); //获得触摸点
		if (!linkIndex.empty())
		{
			Coord latestPos = linkIndex.back(); //连线的最后一个元素
			int _line = latestPos.line;
			int _row = latestPos.row;
			if (blocksCenter[_row][_line].getDistance(touchPoint) < containsDis)
			{
				log("return");
				return; //若触摸点仍在同一个格子里，则不操作
			}

			if (linkIndex.size() > 1) //若已经形成连线（有两个或以上元素连接）
			{
				log("linkIndex.size() > 1");
				Coord linkPos = *(linkIndex.end() - 2); //连线的倒数第二个元素
				if (blocksCenter[linkPos.row][linkPos.line].getDistance(touchPoint) < containsDis)
				{
					undoLink(); //若触摸点回到上一个连接的元素，则撤销最后一次连接
					return;
				}
			}

			//遍历连线最后一个元素周围的所有元素 // 1,1
			for (int i = max(_row - 1, 0); i <= min(_row + 1, MATRIX_ROW - 1); i++) // 0,2
			{
				for (int j = max(_line - 1, 0); j <= min(_line + 1, MATRIX_MLINE - 1); j++) //0,2
				{
					if (blocksCenter[i][j].getDistance(touchPoint) < containsDis) //如果触摸点在格子有效半径内
					{
						if (!signFlag[i][j]) //如果该元素未被连接过
						{
							if (checkLink(latestPos, { i, j })) //如果两个元素符合连接的条件（颜色或数字相同，或其中一个是万能元素）
							{
								linkElement(latestPos, { i, j }); //连接两个元素
							}
						}
					}
				}
			}

		}

	}
}
void MapLayer::onTouchEnded(Touch *touch, Event *unused_event) //触摸结束
{
	if (isResponse())//如果允许触摸
	{
		Point touchPoint = Director::getInstance()->convertToGL(touch->getLocationInView());

		removeLink();//进行一次消除，判断符合条件后消除连线的元素
		
	}
	touchedFlag = false;  //触摸结束
}

void  MapLayer::createSecial(Coord c,int ele)
{
	auto s = ElementUnit::create();
	int num;
	if (linkIndex.size() > 8)
	{
		 num = ele + 30;
	}
	switch (linkIndex.size())
	{
	case 4:
		num = ele + 10;
		break;
	case 5:
		num = ele + 10;
		break;
	case 6:
		num = ele + 20;
		break;
	case 7:
		num = ele + 20;
		break;
	case 8:
		num = ele + 20;
		break;
	default:
		break;
	}
	log("num:%d",num);
	s->createElement(num, getMCenterByCoord(c.row, c.line));
	elements[c.row][c.line] = s;
	elementCount[num]++;
	addChild(elements[c.row][c.line]);
}

//撤销最后一次连接
void MapLayer::undoLink()
{
	Coord latest = linkIndex.back();
	linkIndex.pop_back(); //清除连线序列的最后一个元素
	removeLatestLine(); //删除最后一条连接线
	unsignElement(latest); //取消最后一个元素的标记
}

//绘制矩形辅助线
void MapLayer::drawGuideLine(Point leftBottom, Point rightTop)
{
	DrawNode* b = DrawNode::create(); //创建画刷
	addChild(b);

	//通过四角绘制矩形辅助线
	Point leftTop = Point(leftBottom.x, rightTop.y);
	Point rightBottom = Point(rightTop.x, leftBottom.y);
	b->drawSegment(leftBottom, leftTop, GUIDELINE_WIDTH, GUIDELINE_COLOR);
	b->drawSegment(rightBottom, rightTop, GUIDELINE_WIDTH, GUIDELINE_COLOR);
	b->drawSegment(leftBottom, rightBottom, GUIDELINE_WIDTH, GUIDELINE_COLOR);
	b->drawSegment(leftTop, rightTop, GUIDELINE_WIDTH, GUIDELINE_COLOR);
}

//绘制连接两个元素的线
void MapLayer::drawLine(Coord from, Coord to)
{
	DrawNode* brush = DrawNode::create();
	addChild(brush, -1);
	brush->drawSegment(getMCenterByCoord(from), getMCenterByCoord(to), LINKLINE_WIDTH, LINKLINE_COLOR);
	linkBrush.push_back(brush); //将该连接线添加到线序列中
}

//删除最后一条连接线，返回是否删除成功
bool MapLayer::removeLatestLine()
{
	if (linkBrush.empty())
	{
		return false;
	}
	DrawNode* brush = linkBrush.back();
	removeChild(brush);
	linkBrush.pop_back();//删除线序列中的最后一条
	return true;
}

//检查是否能连接两个元素，，通过修改该函数即可实现多样化规则
bool MapLayer::checkLink(Coord from, Coord to)
{
	if (getElement(from) == getElement(to))  //元素类型相同
	{
		return true;
	}
	int f = getElement(from);
	int t = getElement(to);
	log("getElement(to): f:%d,t:%d", f, t);
	//其中一个是万能元素
	if (getElement(to) == (getElement(from) + 10) || getElement(to) == (getElement(from) + 20) || getElement(to) == (getElement(from) + 30))
	{
		return true;
	}
	if (getElement(to) == (getElement(from) - 10) || getElement(to) == (getElement(from) - 20) || getElement(to) == (getElement(from) - 30))
		return true;

	return false; //不能连接

}

//获取指定位置元素类型
int MapLayer::getElement(Coord c)
{
	return elements[c.row][c.line]->getElement();
}

//指定列的顶端出现新元素
void  MapLayer::appear(int row)
{
	int top =  0;//-1
	int ele = randElement();
	createElement(ele, row, top);
	if (elements[row][top])
	{
		this->addChild(elements[row][top]);
	}
	if (elements[row][top])
	{
		elements[row][top]->appear(FALL_TIME);
	}
}

//通过现有元素类型分布，随机获得一个元素类型
int MapLayer::randElement()
{
	//bool  wildFlag = randFlag(WILD_PROBABILITY - 1, 1); //出现万能元素的概率是1/WILD_PROBABILITY
	//if (wildFlag)
	//{
	//	return WILD_NUM; //抽中则出现万能元素
	//}

	int *num = new int[ELEMENT_COUNT];//用于存放各种元素出现的概率

	//获取元素类型数量的最大最小值
	int max = 0;
	int min = 0x7FFFFFFF;
	for (int i = 0; i < ELEMENT_COUNT; ++i)
	{
		int temp = elementCount[i];
		if (temp >max)
		{
			max = temp;
		}
		if (temp < min)
		{
			min = temp;
		}
	}
	if (max - min <= MAX_GAP)
	{
		max++;//如果最多元素与最少元素差距没有过大，则最多元素仍有可能出现
	}
	for (int i = 0; i < ELEMENT_COUNT; ++i)
	{
		num[i] = max - elementCount[i];//根据元素类型数量确定概率，已存在数量越多，概率越小
	}

	int re = randNum(ELEMENT_COUNT, num);//根据概率随机获得一个结果
	delete[] num;
	return re;
}



//设置连线的起点
void MapLayer::linkStart(int row, int line)
{
	linkIndex.clear(); //清空连线序列
	signOnlyBlock(row, line); //标记唯一元素
	linkIndex.push_back({ row, line });//添加到连线序列
	//音效，震动
	/*	if (UserDefault::getInstance()->getBoolForKey(EFFECT_UD, EFFECT_DEFAULT_FLAG))
	SimpleAudioEngine::getInstance()->playEffect(START_LINK_EFFECT); //播放音效
	if (UserDefault::getInstance()->getBoolForKey(VIBRATE_UD, VIBRATE_DEFAULT_FLAG))
	Vibrator::vibrate(START_LINK_VIBRATOR_TIME); //震动*/
}

//连接两个元素
void MapLayer::linkElement(Coord from, Coord to)
{
	log("linkElement");
	signElement(to);  //标记元素
	drawLine(from, to); //绘制连线
	linkIndex.push_back(to); //添加到连线序列

}

//清空矩阵元素
void MapLayer::clearElement()
{
	//mei shi xian
	ERGODIC_MBLOCK(row,line)
	{
		removeElement(row, line); //清除每个格子的元素
	}
	for (int i = 0; i < ELEMENT_COUNT; ++i)
	{
		elementCount[i] = 0;
	}
}

//在指定位置创建给定类型的元素
int  MapLayer::createElement(int element, int row, int line)
{
		ElementUnit* &temp = elements[row][line]; //定义临时变量，简化代码
		temp = ElementUnit::create();
		elementCount[element]++; // 对应元素类型基数自增
		temp->createElement(element, getMCenterByCoord(row, line));//创建一个元素并添加到屏幕上

		return element;
}

//通过坐标获得格子中心
Point MapLayer::getMCenterByCoord(int row, int line)
{
	//Size mapSize = _tileMap->getMapSize();// 获取以tiles数量为单位的地图尺寸
	Size tileSize = _tileMap->getTileSize();// 获取以像素点为单位的tile尺寸属性 // 100
	//log("size:%i,%i", tileSize.width, tileSize.height);

	Point c = positionForTileCoord(Vec2(row,line));
	//log("row:%i,line:%i",row,line);
	///log("getMCenterByCoord//:c.x:%f,c.y:%f", c.x, c.y);
	int x = c.x + (tileSize.width * RATIO)/ 2;
	int y = c.y + (tileSize.height *RATIO)/ 2;
	//log("getMCenterByCoord//: x:%i,y:%i", x, y);
	return Vec2(x, y);
}

//通过坐标获得格子中心
Point  MapLayer::getMCenterByCoord(Coord c)
{
	return getMCenterByCoord(c.row, c.line);
}

//清空消除个数
void MapLayer::removeCountCleaar()
{
	for (int i = 0; i < ELEMENT_COUNT; ++i)
	{
		removeCount[i] = 0;
	}
}

//只标记一个元素（其他标记被取消）
void MapLayer::signOnlyBlock(int row, int line)
{
	signClear();
	signElement(row, line);
}

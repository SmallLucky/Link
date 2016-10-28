
#include "MapLayer.h"
//#include <iostream>
//#include <vector>
#include "Unit/ElementUnit.h"
#include "Data/ElementData.h"
#include "Data/Data.h"


using namespace cocos2d;
using namespace std;

#define RATIO   1.0

MapLayer::MapLayer() :isLine(false), green(0), red(0), yellor(0), blue(0), purple(0), elementType(5), line(0), row(0), lastPoint({0,0}),
isBoom(false), isCount(false), specialScore(0)
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

	signFlagSpecial = new bool *[MATRIX_ROW];
	for (int i = 0; i < MATRIX_ROW; i++)
	{
		signFlagSpecial[i] = new bool[MATRIX_MLINE];
	}

	//分配格子中心空间
	blocksCenter = new Point*[MATRIX_ROW];
	for (int i = 0; i < MATRIX_ROW; i++)
	{
		blocksCenter[i] = new Point[MATRIX_MLINE];
	}
	////分配各元素数量空间
	elementCount = new int[40]; //注意万能元素空间

	////分配消除个元素的数量空间
	removeCount = new int[40]; //注意万能元素空间

	linkBrush.clear();
}
MapLayer::~MapLayer()
{
	CC_SAFE_DELETE_ARRAY(elements);
	//回收格子元素空间
	//for (int i = 0; i < MATRIX_ROW; i++)
	//	delete[] elements[i];
	//delete[] elements;
	//回收标记空间
	CC_SAFE_DELETE_ARRAY(signFlag);
	//for (int i = 0; i < MATRIX_ROW; i++)
	//	delete[] signFlag[i];
	//delete[] signFlag;

	CC_SAFE_DELETE_ARRAY(signFlagSpecial);
	//for (int i = 0; i < MATRIX_ROW; i++)
	//	delete[] signFlagSpecial[i];
	//delete[] signFlagSpecial;
	//回收格子中心空间
	CC_SAFE_DELETE_ARRAY(blocksCenter);
	//for (int i = 0; i < MATRIX_ROW; i++)
	//	delete[] blocksCenter[i];
	//delete[] blocksCenter;

	//回收各元素数量空间

	CC_SAFE_DELETE_ARRAY(elementCount);

	/*delete[] elementCount;
	elementCount = nullptr;*/
	CC_SAFE_DELETE_ARRAY(removeCount);
	//回收消除各元素的数量空间
	//delete[] removeCount;
	//removeCount = nullptr;

	//linkFinishFlag = false;

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

//检测是否需要进行刷新位置
bool  MapLayer::checkIsNeedRefresh()
{
	ERGODIC_MBLOCK(row, line)
	{
		//遍历一个元素周围的所有元素 // 1,1
		int eleCount = 0;
		if (getElement({row,line}) == 5)
		{
			continue;
		}
		for (int i = max(row - 1, 0); i <= min(row + 1, MATRIX_ROW - 1); i++) // 0,2
		{
			for (int j = max(line - 1, 0); j <= min(line + 1, MATRIX_MLINE - 1); j++) //0,2
			{
				if (getElement({i,j}) == 5)
				{
					continue;
				}
				if (checkLink({row,line},{i,j}))
				{
					eleCount++;
				}
			}
		}
		if (eleCount >= 3 )
		{
			eleCount = 0;
			return false;
		}
		eleCount = 0;
	}
	return true;
}

//刷新所有基础元素的位置
void  MapLayer::refreshAllElement()
{
	ERGODIC_MBLOCK(row, line)
	{
		if (elements[row][line] != nullptr)
		{
			if (getElement({row,line}) < 5)
			{
				elements[row][line]->removeFromParent(); ///按理是不能移除，只能换位置
				auto s = ElementUnit::create();
				if (s)
				{
					s->createElement(randElement(), blocksCenter[row][line]);
					elements[row][line] = s;
					addChild(elements[row][line]);
				}
			}
		}
		
	}
}

//符合条件则消除被连接的元素，否则连接失败
void MapLayer::removeLink()
{
	if (linkIndex.size() >= MIN_ROMOVE_COUNT) //如果连接的元素个数达到要求
	{
		removeSignedElement(); //消除被连接的元素
		removeAllLine(); //删除连接线	
		if (getIsLine())
		{
			removeSignSpecial();
			isLine = false;
		}
		signClear();  
		linkFinishFlag = true; //完成一次消除
		if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT",true))
			SimpleAudioEngine::getInstance()->playEffect(REMOVE_EFFECT); //播放音效
		if (UserDefault::getInstance()->getBoolForKey("IS_VIBRATE",true))
			Vibrator::vibrate(REMOVE_VIBRATOR_TIME); //震动 

		//成功消除一次，步数减少一次
		//removeMyCount();
	}
	else //如果连接的元素个数未达到要求
	{
		signClear(); //取消对元素的标记
		removeAllLine(); //删除连接线
		if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT",true))
			SimpleAudioEngine::getInstance()->playEffect(WRONG_REMOVE_EFFECT); //播放音效
		if (UserDefault::getInstance()->getBoolForKey("IS_VIBRATE",true))
			Vibrator::vibrate(WRONG_REMOVE_VIBRATOR_TIME); //震动
	}

	linkIndex.clear(); //清空连线序列
	//log("removeLink()");
}

//消除被标记的元素，返回消除个数
int		MapLayer::removeSignedElement()
{
//#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//	LOGD("MapLayer::removeSignedElement()");
//#endif
	removeAllCount = 0;
	ERGODIC_MBLOCK(row,line)
	{
		if (signFlag[row][line])
		{
			if (elements[row][line])
			{
				if (elements[row][line]->getElement() >= 10)
				{
					isLine = true;
					specialSignElement(elements[row][line]->getElement(), { row, line });
				}
			}
			int ele = removeElement(row, line);
			removeCount[ele]++; //消除标记的元素
			removeAllCount++;
			if (ele >= 10 && ele < 20)
			{
				showLineEffect(blocksCenter[row][line]);
			}
			if (ele >= 20 && ele < 30)
			{
				showRowEffect(blocksCenter[row][line]);
			}
			if (ele >= 30 && ele < 40)
			{
				showBoomEffect(blocksCenter[row][line]);
			}
			removeBeEliminate(row,line);//
		}
	}
	//signClear();
	log("getElementType()=%d", getElementType());
	if (getElementType() == 2)
	{
		green += removeAllCount;
		log("green:%d",green);
	}
	if (getElementType() == 0)
	{
		blue += removeAllCount;
		log("blue:%d",blue);
	}
	if (getElementType() == 1)
	{
		purple += removeAllCount;
		log("purple:%d",purple);
	}
	if (getElementType() == 3)
	{
		red += removeAllCount;
		log("red:%d",red);
	}
	if (getElementType() == 4)
	{
		yellor += removeAllCount;
		log("yellor:%d",yellor);
	}
	return removeAllCount;
}

//检测并消除可以被消除的元素，改变type值
void MapLayer::removeBeEliminate(int r, int l)
{
	for (int i = max(r - 1, 0); i <= min(r + 1, MATRIX_ROW - 1); i++) // 0,2
	{
		for (int j = max(l - 1, 0); j <= min(l + 1, MATRIX_MLINE - 1); j++) //0,2
		{
			if (elements[i][j])
			{
				if (elements[i][j]->getElement() == 6)
				{
					removeElement(i, j); // 移除精灵
					//修改瓦片属性
					changeTiledType(i, j);

				}
			}
		}

	}
}
// 修改指定位置上瓦片的属性值
void	MapLayer::changeTiledType(int r, int l)
{
	unsigned int gidnum = typeLayer->getTileGIDAt(Vec2(0, 0));

	unsigned int num = typeLayer->getTileGIDAt(Vec2(r, l));
	Value typevalue = _tileMap->getPropertiesForGID(num);
	int type = typevalue.asValueMap()["type"].asInt();
	log("(0,5)type1:%d", type);
	if (type == 6)
	{
		typeLayer->setTileGID(gidnum, Vec2(r, l));
	}

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
	//elementType = ele;//获得元素类型
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
	if (isCount)
	{
		isCount = false;
		return m_count;
		
	}
	else
	{
		return ++m_count;
	}
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
//#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//	LOGD("MapLayer::elementsFall()");
//#endif
	log("elementsFall()");
	bool flag = false; //是否填补了空位
	//for (int i = 0; i < MATRIX_ROW; ++i)//对每一列单独处理 0
	//{
	//	for (int j = MATRIX_LINE; j >= 0; --j)//7
	//	{
	//		if (elements[i][j] == nullptr && checkIsDrop(i,j))
	//		{
	//			log("MapLayer::elementsFall() : i:%d,j:%d", i, j);
	//			//log("MapLayer::elementsFall:%i,%i",i,j);
	//			/*rowMyFall(i, j);*/
	//			rowFall(i, j);//从最下方开始，一旦出现空位，所有上方元素下落，填补空位
	//			flag = true;//填补了空位
	//			break;//每一列只填补最下一个空位
	//		}
	//	}
	//}
	for (int j = MATRIX_LINE; j >= 0; --j)//5
	{
		for (int i = 0; i < MATRIX_ROW; ++i)//对每hang单独处理 0
		{
			if (elements[i][j] == nullptr && checkIsDrop(i, j))
			{
				//log("MapLayer::elementsFall() : i:%d,j:%d", i, j);
				//rowFall(i, j);
				rowMyFall(i,j);
				flag = true;
				break;
			}
		}
	}
	return flag;
}

//检测是否可以下落
bool	MapLayer::checkIsDrop(int r, int l)
{
	//log("MapLayer::checkIsDrop(int r, int l)");
	if (elements[r][l] == nullptr)//05
	{
		for (int i = (l-1); i >= 0; i--)//04
		{
			if (elements[r][i] == nullptr)
			{
				return true;
				//continue;
			}else
			{
				if (elements[r][i])
				{
					if (checkIsNull(r,i))
					{
						if (checkIsNull(max(r - 1, 0), i) && checkIsNull(min(r + 1, 5), i))
						{
							return false;
						}
						else
						{
							if (!checkIsNull(max(r - 1, 0), i) || !checkIsNull(min(r + 1, 5), i))
							{
								return true;
							}
						}
					}
					else
					{
						return true;
					}
				}
			}
		}
		//log("true*//*");
		return true;
	}
	else
	{
		if (checkIsNull(r,l))
		{
			return false;
		}
	}
	return true;
}

bool MapLayer::checkIsNull(int _row, int _line)
{ 
	unsigned int num;
	num = typeLayer->getTileGIDAt(Vec2(_row, _line));
	getType = _tileMap->getPropertiesForGID(num);
	int _type = getType.asValueMap()["type"].asInt();
	if (_type != 1)
	{
		//log("_type == 2");
		return true;
	}
	//if (_type == 3)
	//{
	//	//log("_type == 2");
	//	return true;
	//}
	return false;
}

//是否有效的连接结束
bool MapLayer::isLinkFinish()
{
//#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//	LOGD(" MapLayer::isLinkFinish()");
//#endif
	return linkFinishFlag;
}

//准备下一次连接
void MapLayer::prepareLink()
{
	linkFinishFlag = false;
}

void	MapLayer::rowMyFall(int _row, int bottom) // 15
{
	int	j = bottom - 1;
	log("******j%d ,j+1:%d",j,bottom);
	if (j>=0)
	{
		if (checkIsNull(_row, j))
		{
			//log("MapLayer::rowMyFall(int _row, int bottom):%i", j);
			//j--;
			if (!checkIsNull(max(_row - 1, 0), j))
			{
				//log("if (!checkIsNull(max(_row -1 , 0), j))");
				elements[_row][bottom] = elements[max(_row - 1, 0)][j];
				if (elements[_row][bottom])
				{
					Point pop = getMCenterByCoord(_row, bottom);
					elements[max(_row - 1, 0)][j]->moveToPosition(pop, FALL_TIME);//上方元素掉落一个
					elements[max(_row - 1, 0)][j] = nullptr;
				}
			}
			else if (!checkIsNull(min(_row + 1, 5), j))
			{
				//log("if (!checkIsNull(min(_row + 1, 5), j))");
				elements[_row][bottom] = elements[min(_row + 1, 5)][j];
				if (elements[_row][bottom])
				{
					Point pop = getMCenterByCoord(_row, bottom);
					elements[min(_row + 1, 5)][j]->moveToPosition(pop, FALL_TIME);//上方元素掉落一个
					elements[min(_row + 1, 5)][j] = nullptr;
				}
			}
		}else if(!checkIsDrop(_row, j))
		{
			if (!checkIsNull(max(_row - 1, 0), j))
			{
				log("if (!checkIsNull(max(_row -1 , 0), j))%d,%d", max(_row - 1, 0),j);
				elements[_row][bottom] = elements[max(_row - 1, 0)][j];
				if (elements[_row][bottom])
				{
					Point pop = getMCenterByCoord(_row, bottom);
					elements[max(_row - 1, 0)][j]->moveToPosition(pop, FALL_TIME);//上方元素掉落一个
					elements[max(_row - 1, 0)][j] = nullptr;
				}
			}
			else if (!checkIsNull(min(_row + 1, 5), j) )
			{
				log("if (!checkIsNull(min(_row + 1, 5), j))%d,%d", min(_row + 1, 5),j);
				elements[_row][bottom] = elements[min(_row + 1, 5)][j];
				if (elements[_row][bottom])
				{
					Point pop = getMCenterByCoord(_row, bottom);
					elements[min(_row + 1, 5)][j]->moveToPosition(pop, FALL_TIME);//上方元素掉落一个
					elements[min(_row + 1, 5)][j] = nullptr;
				}
			}
		}else
			{
				elements[_row][bottom] = elements[_row][j]; //null
				if (elements[_row][bottom])
				{
					Point pop = getMCenterByCoord(_row, bottom);
					elements[_row][j]->moveToPosition(pop, FALL_TIME);//上方元素掉落一个
					elements[_row][j] = nullptr;
					//appear(_row);
				}
			}
	}
	if (j < 0)
	{
		appear(_row);
	}

}

//指定列的全部空位上方元素下落，顶端出现新元素i.j
void MapLayer::rowFall(int _row, int bottom)//0,5/1.2 5 5
{
//#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
//	LOGD("MapLayer::rowFall(int _row, int bottom)");
//#endif
	int r = _row;
	bool isapp = true;
	int j;
	for (int i = bottom; i > 0; )//7--i
	{
		 j = i - 1;//6
		if (elements[r][i] == nullptr)
		{
			if (checkIsNull(r, i))
			{
				//--i;
				isapp = false;
				continue;
				//break;
			}
			//while (checkIsNull(_row, j))//06
			if (j >= 0 )
			{
				if (checkIsNull(r, j))
				{
					//log("while (checkSpriteNull({ _row, j })):%i",j);
					//j--;
					isapp = false;
					if (!checkIsNull(max(r - 1, 0), j) )
					{
						if (max(r - 1, 0) != r)
						{
							elements[r][i] = elements[max(r - 1, 0)][j];
							if (elements[r][i])
							{
								Point pop = getMCenterByCoord(r, i);
								elements[max(r - 1, 0)][j]->moveToPosition(pop, FALL_TIME);//上方元素掉落一个
								elements[max(r - 1, 0)][j] = nullptr;
								//r = max(r - 1, 0);
							}
							else
							{
								isapp = true;
							}
						}
						else
						{
							if (!checkIsNull(min(r + 1, 5), j))
							{
								elements[r][i] = elements[min(r + 1, 5)][j];
								if (elements[r][i])
								{
									Point pop = getMCenterByCoord(r, i);
									elements[min(r + 1, 5)][j]->moveToPosition(pop, FALL_TIME);//上方元素掉落一个
									elements[min(r + 1, 5)][j] = nullptr;
									//r = min(r + 1, 5);
								}
								else
								{
									isapp = true;
								}
							}
						}

					}
					else if (!checkIsNull(min(r + 1, 5), j))
					{
						if (min(r + 1, 5) != r)
						{
							elements[r][i] = elements[min(r + 1, 5)][j];
							if (elements[r][i])
							{
								Point pop = getMCenterByCoord(r, i);
								elements[min(r + 1, 5)][j]->moveToPosition(pop, FALL_TIME);//上方元素掉落一个
								elements[min(r + 1, 5)][j] = nullptr;
								//r = min(r + 1, 5);
							}
							else
							{
								isapp = true;
							}
						}
						else
						{
							if (!checkIsNull(max(r - 1, 0), j))
							{
								elements[r][i] = elements[max(r - 1, 0)][j];
								if (elements[r][i])
								{
									Point pop = getMCenterByCoord(r, i);
									elements[max(r - 1, 0)][j]->moveToPosition(pop, FALL_TIME);//上方元素掉落一个
									elements[max(r - 1, 0)][j] = nullptr;
									//r = max(r - 1, 0);
								}
							}
						}

					}
				}
				else
				{
					//if (!checkIsDrop(r, j) && elements[r][j])
					//{
					//	isapp = false;
					//	if (checkIsDrop(max(r-1,0),j))
					//	{
					//		elements[r][i] = elements[max(r - 1, 0)][j];
					//		if (elements[r][i])
					//		{
					//			Point pop = getMCenterByCoord(r, i);
					//			elements[max(r - 1, 0)][j]->moveToPosition(pop, FALL_TIME);//上方元素掉落一个
					//			elements[max(r - 1, 0)][j] = nullptr;
					//		}
					//		else if (checkIsDrop(min(r + 1, 5), j))
					//		{
					//			isapp = false;
					//			elements[r][i] = elements[min(r + 1, 5)][j];
					//			if (elements[r][i])
					//			{
					//				Point pop = getMCenterByCoord(r, i);
					//				elements[min(r + 1, 5)][j]->moveToPosition(pop, FALL_TIME);//上方元素掉落一个
					//				elements[min(r + 1, 5)][j] = nullptr;
					//			}
					//		}
					//	}
					//}
					elements[r][i] = elements[r][j]; //null
					if (elements[r][i])
					{
						Point pop = getMCenterByCoord(r, i);
						elements[r][j]->moveToPosition(pop, FALL_TIME);//上方元素掉落一个
						elements[r][j] = nullptr;
					}
				}
			}

		}
		i = j;
	}
	if (isapp)
	{
		appear(r);
	}
	//appear(r);
} 

//初始化格子，确定格子区域，初始状态矩阵为空
void  MapLayer::initBlocks()
{
//#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//	LOGD(" MapLayer::initBlocks()");
//#endif
	string str = StringUtils::format("tiledmap/map_%d.tmx", GAMEDATA->getCurLevel());
	//_tileMap = TMXTiledMap::create(str);//("TileMaps/hexa-test.tmx");
	_tileMap = TMXTiledMap::create("tiledmap/map_18.tmx");//("TileMaps/hexa-test.tmx");
	if (_tileMap)
	{
		_tileMap->setAnchorPoint(Vec2(0.5, 0.5));
		_tileMap->setPosition(mapPoint);
		_tileMap->setScale(RATIO);
		addChild(_tileMap, 0);

		mapLayer = _tileMap->getLayer("map");
		typeLayer = _tileMap->getLayer("type");
		typeLayer->setVisible(false);

		line = _tileMap->getMapSize().height; // 10
		row = _tileMap->getMapSize().width; // 8

		////mei shi xian
		containsDis = (typeLayer->getMapTileSize().width*RATIO) / 2 - 5;
		log("%f", containsDis);
	}
	//初始化数量
	for (int i = 0; i < 40; ++i) //0-5
	{
		elementCount[i] = 0;
		removeCount[i] = 0;
	}

	ERGODIC_MBLOCK(row,line)
	{
		blocksCenter[row][line] = getMCenterByCoord(row, line); //确定格子中心位置
			/*	int  num = (uint32_t)typeLayer->getTileGIDAt(Vec2(i, j));*/
				//getType = _tileMap->getPropertiesForGID(num);
				//int _type = getType.asValueMap()["type"].asInt();
		if (checkIsNull(row, line))
		{
			unsigned int num;
			num = typeLayer->getTileGIDAt(Vec2(row, line));
			int _type = _tileMap->getPropertiesForGID(num).asValueMap()["type"].asInt();
			//这块瓦片是不显示 
			auto s = ElementUnit::create();
			if (s)
			{
				s->setElementType(ElementType::cannotEliminate); // 不可一消除的元素
				s->createElement(_type, getMCenterByCoord(row, line));
				elements[row][line] = s;
				elementCount[_type]++;
				addChild(elements[row][line], 1);
				//elements[row][line]->setVisible(false);
			}
		}
		else
		{
			auto s = ElementUnit::create();
			if (s)
			{
				s->setElementType(ElementType::basisEliminate); // 基础消除元素
				int color = abs(rand() % 5);
				s->createElement(color, getMCenterByCoord(row, line));
				elements[row][line] = s;
				elementCount[color]++;
				addChild(elements[row][line], 1);
			}		
		}
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
//#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//	LOGD("MapLayer::initSign()");
//#endif
	signClear();
}

//清除所有标记
void  MapLayer::signClear()
{
	ERGODIC_MBLOCK(row,line)
	{
		unsignElement(row, line);
		unsignSpecialElement({ row, line });
	}
}

// 取消标记元素
int MapLayer::unsignElement(int row,int line)
{
	//if (elements[row][line])
	//{
		signFlag[row][line] = false;
		return 0;  //如果指定位置无元素，则不操作
	//}
	//return -1;
}

//取消标记元素
int MapLayer::unsignElement(Coord c)
{
	return unsignElement(c.row, c.line);
}

//标记元素
int MapLayer::signElement(int row, int line)
{
	if (elements[row][line])
	{
		signFlag[row][line] = true;
		return 0;
	}
	return -1;
}

//标记元素
int MapLayer::signElement(Coord c)
{
	return signElement(c.row, c.line);//调用重载函数
}

//标记特殊元素涉及的元素
int		MapLayer::signSpecialElement(Coord c)
{
	if (elements[c.row][c.line])
	{
		signFlagSpecial[c.row][c.line] = true;
		return 0;
	}
	return -1;
}

// 取消特殊元素涉及元素的标记
int		MapLayer::unsignSpecialElement(Coord c)
{
	//if (elements[c.row][c.line])
	//{
		signFlagSpecial[c.row][c.line] = false;
		return 0;
	//}
	//return -1;
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
				//log("element:%i", getElement({ row, line }));
				if (getIsBoom())
				{
					//isBoomElement(row, line);
					return true;
				}
				if (getElement({ row, line }) >= 5 && getElement({ row, line }) < 10)
				{ 
					log("wild");
					touchedFlag = false;//成为有效触摸
					return false;
				}
				else
				{
					linkStart(row, line);//该格子作为连线起点
					touchedFlag = true;//成为有效触摸
					//加个特效看看
					showEffect(blocksCenter[row][line]);
					//ParticleSystem* m_emitter1 = ParticleSystemQuad::create("effect/particle_1.plist");
					//m_emitter1->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,elements[row][line],Vec2(0,0)));
					//elements[row][line]->addChild(m_emitter1);

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
		if (getIsBoom())
		{
			return;
		}
		if (!linkIndex.empty())
		{
			Coord latestPos = linkIndex.back(); //连线的最后一个元素
			int _line = latestPos.line;
			int _row = latestPos.row;
			if (blocksCenter[_row][_line].getDistance(touchPoint) < containsDis)
			{
				//log("return");
				return; //若触摸点仍在同一个格子里，则不操作
			}

			if (linkIndex.size() > 1) //若已经形成连线（有两个或以上元素连接）
			{
				//log("linkIndex.size() > 1");
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
						if (elements[i][j])
						{
							if (!signFlag[i][j]) //如果该元素未被连接过
							{
								if (checkLink(latestPos, { i, j })) //如果两个元素符合连接的条件（颜色或数字相同，或其中一个是万能元素）
								{
									linkElement(latestPos, { i, j }); //连接两个元素 加上了标记
									//特效
									showEffect(blocksCenter[i][j]);
									if (elements[i][j]->getElement() > 10)
									{
										elementType = elements[i][j]->getElement() % 10;//获得元素类型
										log("onTouchMoved(Touch *touch, Event *unused_event) :elementType %d", elementType);
									}
									else
									{
										elementType = elements[i][j]->getElement();//获得元素类型
										log("elementType：%d", elementType);
									}
								}
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

		if (getIsBoom())
		{
			ERGODIC_MBLOCK(row, line)
			{
				if (blocksCenter[row][line].getDistance(touchPoint) < containsDis)
				{ 
					//炸弹数量减一
					showBoomEffect(blocksCenter[row][line]);
					isBoomElement(row, line);
					removeSignSpecial();
					isCount = true;
					linkFinishFlag = true;
				}
				else
				{
					isBoom = false;
				}
			}
		}
		else
		{
			
			removeLink();//进行一次消除，判断符合条件后消除连线的元素
			while (removeEffect());

		}


	}

	touchedFlag = false;  //触摸结束
//#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//	LOGD("onTouchEnded");
//#endif
	if (UserDefault::getInstance()->getBoolForKey("IS_VIBRATE",true))
	Vibrator::cancelVibrate();
	log("onTouchEnded");
}


void MapLayer::showEffect(Point p)
{
	ParticleSystem * efft = ParticleSystemQuad::create("effect/particle_1.plist");
	//ParticleSystem* m_emitter1 = ParticleSystemQuad::create("effect/particle_1.plist");
	efft->setPosition(p);
	m_effects.pushBack(efft);
	addChild(efft, 1);
}

bool MapLayer::removeEffect()
{
	if (m_effects.empty())
	{
		return false;
	}
	ParticleSystem* e = m_effects.back();
	m_effects.popBack();
	removeChild(e);
	return true;
}

void MapLayer::showLineEffect(Point p)
{
	if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
		SimpleAudioEngine::getInstance()->playEffect(ELEMENT_LINE); //播放音效

	SpriteFrameCache *frameCache = SpriteFrameCache::sharedSpriteFrameCache();
	frameCache->addSpriteFramesWithFile("effect/Line.plist");

	auto s = Sprite::createWithSpriteFrameName("line_0.png");
	if (s)
	{
		s->setPosition(Vec2(mapPoint.x,p.y));
		//s->setScale(1);
		addChild(s, 1);
		auto a = CommonFunction::createWithSingleFrameName("line_", 0.05, 1);
		Animate* animate = CCAnimate::create(a);

		CallFunc* call = CallFunc::create([=]{
			if (s)
			{
				removeChild(s);
			}

		});
		Sequence* action = Sequence::create(animate, call, nullptr);

		s->runAction(action);
	}
}
void MapLayer::showRowEffect(Point p)
{
	if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
		SimpleAudioEngine::getInstance()->playEffect(ELEMENT_ROW); //播放音效
	SpriteFrameCache *frameCache = SpriteFrameCache::sharedSpriteFrameCache();
	frameCache->addSpriteFramesWithFile("effect/Line.plist");

	auto s = Sprite::createWithSpriteFrameName("line_0.png");
	if (s)
	{
		s->setPosition(Vec2(p.x, mapPoint.y));
		s->setRotation(90.0f);
		addChild(s, 1);
		auto a = CommonFunction::createWithSingleFrameName("line_", 0.05, 1);
		Animate* animate = CCAnimate::create(a);

		CallFunc* call = CallFunc::create([=]{
			if (s)
			{
				removeChild(s);
			}

		});
		Sequence* action = Sequence::create(animate, call, nullptr);

		s->runAction(action);
	}
}
void MapLayer::showBoomEffect(Point p)
{
	if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
		SimpleAudioEngine::getInstance()->playEffect(ELEMENT_BOOM); //播放音效
	ParticleSystem * efft = ParticleSystemQuad::create("effect/particle_6.plist");
	efft->setPosition(p);
	efft->setAutoRemoveOnFinish(true);
	efft->setDuration(0.1f);
	addChild(efft, 1);

}


//炸弹炸掉的元素
void	MapLayer::isBoomElement(int _row, int _line)
{
	signClear();

	for (int i = max(_row - 1, 0); i <= min(_row + 1, MATRIX_ROW - 1); i++) // 0,2
	{
		for (int j = max(_line - 1, 0); j <= min(_line + 1, MATRIX_MLINE - 1); j++) //0,2
		{
			//log("i: %d,j:%d",i,j);
			if (elements[i][j])
			{
				if (elements[i][j]->getElement() == 5)
				{
					continue;
				}
				else
				{
					int x = signSpecialElement({ i, j });
					//log("***%d", x);
				}
			}
		}
	}

}

//给特殊元素需要消除的元素加上标记
void	 MapLayer::specialSignElement(int ele, Coord c)
{
//#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//	LOGD("MapLayer::specialSignElement(int ele, Coord c) : %d",ele);
//#endif
	log("MapLayer::specialSignElement(int ele, Coord c)");
	if (ele >= 10 && ele < 20) // hengxiang
	{
		log("ele :%d",ele);
		for (int i = 0; i < 6; i++)
		{
			if (elements[i][c.line])
			{
				int eletype = elements[i][c.line]->getElement();
				if (!signFlag[i][c.line] && !signFlagSpecial[i][c.line])
				{
					if (eletype == 5)
					{
						continue;
					}
					if (eletype < 10 && eletype != 5)
					{
						signFlagSpecial[i][c.line] = true; // 加上标记
						if (eletype == 6)
						{
							changeTiledType(i,c.line); //消除的是被消除的，修改瓦片属性
						}
					}
					else
					{
						log("specialSignElement(elements[i][c.line]->getElement(), {i,c.line})");
						signFlagSpecial[i][c.line] = true; // 是特殊元素，给自己加上标记，下次检测的时候就不在检测了，否则会进入死循环一直递归下去
						specialSignElement(elements[i][c.line]->getElement(), {i,c.line});//调用自己，递归的用法。
					}

				}
			}

		}
	}
	if (ele >= 20 && ele < 30) //shuaxiang
	{
		log("ele :%d", ele);
		for (int i = 0; i < 6; i++)
		{
			if (elements[c.row][i])
			{
				int eletype = elements[c.row][i]->getElement();
				if (!signFlag[c.row][i] && !signFlagSpecial[c.row][i])
				{
					if (eletype == 5)
					{
						//signFlagSpecial[c.row][i] = false;
						continue;
					}
					if (eletype < 10 && eletype != 5)
					{
						signFlagSpecial[c.row][i] = true;
					}
					else
					{
						signFlagSpecial[c.row][i] = true;
						specialSignElement(elements[c.row][i]->getElement(), { c.row, i });
					}
				}
			}
		}
	}if (ele >= 30 && ele < 40 )
	{
		log("ele :%d", ele);
		for (int i = max(c.row - 1, 0); i <= min(c.row + 1, MATRIX_ROW - 1); i++) // 0,2
		{
			for (int j = max(c.line - 1, 0); j <= min(c.line + 1, MATRIX_MLINE - 1); j++) //0,2
			{
				if (elements[i][j])
				{
					int eletype = elements[i][j]->getElement();
					if (!signFlag[i][j] && !signFlagSpecial[i][j])
					{
						if (eletype == 5)
						{
							//signFlagSpecial[i][j] = false;
							continue;
						}
						if (eletype < 10 && eletype != 5)
						{
							signFlagSpecial[i][j] = true;
						}
						else
						{
							signFlagSpecial[i][j] = true;
							specialSignElement(elements[i][j]->getElement(), {i,j});
						}
					}
				}
			}
		}
	}
}

//撤销最后一次连接
void MapLayer::undoLink()
{
	Coord latest = linkIndex.back();
	linkIndex.pop_back(); //清除连线序列的最后一个元素
	removeLatestLine(); //删除最后一条连接线
	removeEffect(); // 删除最后一个元素的特效
	unsignElement(latest); //取消最后一个元素的标记
	if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT",true))
		SimpleAudioEngine::getInstance()->playEffect(UNDO_EFFECT); //播放音效
	if (UserDefault::getInstance()->getBoolForKey("IS_VIBRATE",true))
		Vibrator::vibrate(UNDO_VIBRATOR_TIME); //震动
}

//取消撤销关于这特殊元素涉及的所有标识
void	MapLayer::removeSignSpecial()
{
	log("MapLayer::signSpecial()");
	specialScore = 0;
	ERGODIC_MBLOCK(row, line)
	{
		if (elements[row][line])
		{
			if (signFlagSpecial[row][line])
			{
				//log("*******");
				int ele = removeElement(row, line);
				removeCount[ele]++; //消除标记的元素
				specialScore += specialSttlement(ele);
				if (ele >= 10 && ele < 20)
				{
					showLineEffect(blocksCenter[row][line]);
				}
				if (ele >= 20 && ele < 30)
				{
					showRowEffect(blocksCenter[row][line]);
				}
				if (ele >= 30 && ele < 40)
				{
					showBoomEffect(blocksCenter[row][line]);
				}
			}
		}
	}
	if (getIsBoom())
	{
		//log("setIsBoom(false);");
		signClear();
		setIsBoom(false);
	}
}

int		MapLayer::specialSttlement(int ele)//
{
	switch (ele)
	{
	case 0:
		//count * 50 * 1;
		return 50;
		break;
	case 1:
		return 55;
		break;
	case 2:
		return 60;
		break;
	case 3:
		return 65;
		break;
	case 4:
		return 70;
		break;
	case 5 :
		break;
	default:
		//return 75;
		break;
	}
}


////绘制矩形辅助线
//void MapLayer::drawGuideLine(Point leftBottom, Point rightTop)
//{
//	DrawNode* b = DrawNode::create(); //创建画刷
//	addChild(b);
//
//	//通过四角绘制矩形辅助线
//	Point leftTop = Point(leftBottom.x, rightTop.y);
//	Point rightBottom = Point(rightTop.x, leftBottom.y);
//	b->drawSegment(leftBottom, leftTop, GUIDELINE_WIDTH, GUIDELINE_COLOR);
//	b->drawSegment(rightBottom, rightTop, GUIDELINE_WIDTH, GUIDELINE_COLOR);
//	b->drawSegment(leftBottom, rightBottom, GUIDELINE_WIDTH, GUIDELINE_COLOR);
//	b->drawSegment(leftTop, rightTop, GUIDELINE_WIDTH, GUIDELINE_COLOR);
//}

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
	int f = getElement(from); //10 // 0
	int t = getElement(to); //0  // 10
	//log("getElement(to): f:%d,t:%d", f, t);
	//其中一个是万能元素
	if (getElement(to) == (getElement(from) + 10) || getElement(to) == (getElement(from) + 20) || getElement(to) == (getElement(from) + 30))
	{
		return true;
	}
	if (getElement(to) == (getElement(from) - 10) || getElement(to) == (getElement(from) - 20) || getElement(to) == (getElement(from) - 30))
	{
		return true;
	}

	return false; //不能连接

}

//获取指定位置元素类型
int MapLayer::getElement(Coord c)
{
	if (elements[c.row][c.line])
	{
		return elements[c.row][c.line]->getElement();
	}
}

//指定列的顶端出现新元素
void  MapLayer::appear(int row)
{
//#if(CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
//	LOGD("MapLayer::appear(int row)");
//#endif
	//log("appear(int row : %d)",row);
	int top = 0;
	int ele = abs(rand() % 5);// randElement(); // 0 1 2 3 4 
	if (green >= 5)
	{
		//srand((int)time(0));
		ele = 12 + ((rand() % 3 )* 10); //12 22 32 12+0,12+10,12+20
		green = 0;
	}
	if (blue >= 5)
	{
		ele = 10 + ((rand() % 3) * 10); //12 22 32 12+0,12+10,12+20
		blue = 0;
	}
	if (red >= 5)
	{
		ele = 13 + ((rand() % 3) * 10); //12 22 32 12+0,12+10,12+20
		red = 0;
	}
	if (yellor >= 5)
	{
		ele = 14 + ((rand() % 3) * 10); //12 22 32 12+0,12+10,12+20
		yellor = 0;
	}
	if (purple >= 5)
	{
		ele = 11 + ((rand() % 3) * 10); //12 22 32 12+0,12+10,12+20
		purple = 0;
	}
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
	if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT",true))
	SimpleAudioEngine::getInstance()->playEffect(START_LINK_EFFECT); //播放音效
	if (UserDefault::getInstance()->getBoolForKey("IS_VIBRATE",true))
	Vibrator::vibrate(3); //震动
}

//连接两个元素
void MapLayer::linkElement(Coord from, Coord to)
{
	///log("linkElement");
	signElement(to);  //标记元素
	drawLine(from, to); //绘制连线
	linkIndex.push_back(to); //添加到连线序列
	if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT",true))
		SimpleAudioEngine::getInstance()->playEffect(LINK_EFFECT); //播放音效
	if (UserDefault::getInstance()->getBoolForKey("IS_VIBRATE",true))
		Vibrator::vibrate(LINK_VIBRATOR_TIME); //震动

}

//清空矩阵元素
void MapLayer::clearElement()
{
	//mei shi xian
	ERGODIC_MBLOCK(row,line)
	{
		removeElement(row, line); //清除每个格子的元素
	}
	for (int i = 0; i < 40; ++i)
	{
		elementCount[i] = 0;
		//elementCount[i+10] = 0;
		//elementCount[i+20] = 0;
		//elementCount[i+30] = 0;
	}
}

//在指定位置创建给定类型的元素
int  MapLayer::createElement(int element, int row, int line) //22
{
		ElementUnit* &temp = elements[row][line]; //定义临时变量，简化代码
		temp = ElementUnit::create();
		elementCount[element]++; // 对应元素类型基数自增
		if (temp)
		{
			temp->setElementType(ElementType::basisEliminate);//设置为基本消除属性
			temp->createElement(element, getMCenterByCoord(row, line));//创建一个元素并添加到屏幕上
		}
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
	for (int i = 0; i < 6; ++i)
	{
		removeCount[i] = 0;
		removeCount[i + 10] = 0;
		removeCount[i + 20] = 0;
		removeCount[i + 30] = 0;
	}
}

//只标记一个元素（其他标记被取消）
void MapLayer::signOnlyBlock(int row, int line)
{
	signClear();
	signElement(row, line);
}

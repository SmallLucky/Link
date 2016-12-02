
#include "MapLayer.h"
//#include <iostream>
//#include <vector>
#include "Unit/ElementUnit.h"
#include "Data/ElementData.h"
#include "Data/Data.h"
#include "DrawLine.h"
#include "AddCount.h"


using namespace cocos2d;
using namespace std;

#define RATIO   1.0

MapLayer::MapLayer() :isLine(false), green(0), red(0), yellor(0), blue(0), purple(0), elementType(5),lastPoint({0,0}),
isBoom(false), isCount(false), specialScore(0), containsDis(0.0), isTargetElementFinish(false), isRemoveFinish(false)
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


	signBeRemove = new bool *[MATRIX_ROW];
	for (int i = 0; i < MATRIX_ROW; i++)
	{
		signBeRemove[i] = new bool[MATRIX_MLINE];
	}

	////分配各元素数量空间
	//--elementCount = new int[40]; //注意万能元素空间

	////分配消除个元素的数量空间
	//--removeCount = new int[40]; //注意万能元素空间
	auto _listenerElement = EventListenerCustom::create(FINISHTARGETELEMENT, [=](EventCustom*event){
		isTargetElementFinish = true;
	});
	_eventDispatcher->addEventListenerWithFixedPriority(_listenerElement, 1);
	linkBrush.clear();
	m_effects.clear();
}
MapLayer::~MapLayer()
{
	_eventDispatcher->removeCustomEventListeners(FINISHTARGETELEMENT);
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

	CC_SAFE_DELETE_ARRAY(signBeRemove);
	//回收各元素数量空间

	//--CC_SAFE_DELETE_ARRAY(elementCount);

	/*delete[] elementCount;
	elementCount = nullptr;*/
	//--CC_SAFE_DELETE_ARRAY(removeCount);
	//回收消除各元素的数量空间
	//delete[] removeCount;
	//removeCount = nullptr;

	//linkFinishFlag = false;
	linkBrush.clear();
	m_effects.clear();

}

bool MapLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	bool flag = UserDefault::getInstance()->getBoolForKey("IS_MUSIC", true);
	if (flag)
	{
		AudioData::getInstance()->addBgMusic(3);
	}
	else
	{
		AudioData::getInstance()->stopBgMusic();
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	mapPoint = { visibleSize.width / 2, visibleSize.height/2 - 65 };

	initBlocks();//初始化格子，确定格子区域，初始状态矩阵为空
	initSign(); //初始化标记
	initSignBeRemove(); //初始被消标记

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
				if (getElement({ i, j }) == 5 || getElement({ i, j }) == 6)
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
					s->createElement(abs(rand() % 5), blocksCenter[row][line]);//randElement()
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
		removeTargetECount = 0;
		removeSignedElement(); //消除被连接的元素
		removeAllLine(); //删除连接线	
		if (getIsLine())
		{
			removeSignSpecial();
			isLine = false;
		}
		else
		{
			if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
				AudioData::getInstance()->addSpecialEffect(5);
		}
		signClear();  
		linkFinishFlag = true; //完成一次消除
		//if (UserDefault::getInstance()->getBoolForKey("IS_VIBRATE",true))
		//	Vibrator::vibrate(REMOVE_VIBRATOR_TIME); //震动 

		//成功消除一次，步数减少一次
		//removeMyCount(); 
	}
	else //如果连接的元素个数未达到要求
	{
		if (!temp_elementPos.empty())
		{
			undoChangeElement();
		}
		signClear(); //取消对元素的标记
		removeAllLine(); //删除连接线
		if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
			AudioData::getInstance()->addOtherEffect(3);
		//if (UserDefault::getInstance()->getBoolForKey("IS_VIBRATE",true))
		//	Vibrator::vibrate(WRONG_REMOVE_VIBRATOR_TIME); //震动
	}

	isRemoveFinish = true;

	linkIndex.clear(); //清空连线序列
	m_effects.clear();
	linkBrush.clear();
}

//消除被标记的元素，返回消除个数
int		MapLayer::removeSignedElement()
{
	removeAllCount = 0;
	clearSignBeRemove();
	ERGODIC_MBLOCK(row,line)
	{
		if (signFlag[row][line])
		{
			if (elements[row][line])
			{
				if (elements[row][line]->getElement() >= 10 && elements[row][line]->getElement() < 30)
				{
					isLine = true;
					specialSignElement(elements[row][line]->getElement(), { row, line });
				}
				int ele = removeElement(row, line);
				//--removeCount[ele]++; //消除标记的元素
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
					//showBoomEffect(blocksCenter[row][line]);
				}
				removeBeEliminate(row, line);//
			}
		}
	}
	clearSignBeRemove(); //走完同一次的连接,清除被消除标记
	//signClear();
	if (getElementType()%10 == 2)
	{
		green += removeAllCount;
		//log("green:%d",green);
		eventTargetElement(2, removeAllCount);
	}
	if (getElementType() % 10 == 0)
	{
		blue += removeAllCount;
		eventTargetElement(0, removeAllCount);
	}
	if (getElementType() % 10 == 1)
	{
		purple += removeAllCount;
		eventTargetElement(1, removeAllCount);
	}
	if (getElementType() % 10 == 3)
	{
		red += removeAllCount;
		eventTargetElement(3, removeAllCount);
	}
	if (getElementType() % 10 == 4)
	{
		yellor += removeAllCount;
		eventTargetElement(4, removeAllCount);
	}

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	LOGD("removeSignedElement");
#endif

	return removeAllCount;
}

//检测并消除可以被消除的元素，改变type值
void MapLayer::removeBeEliminate(int r, int l)
{
	//for (int i = max(r - 1, 0); i <= min(r + 1, MATRIX_ROW - 1); i++) // 0,2
	//{
	//	for (int j = max(l - 1, 0); j <= min(l + 1, MATRIX_MLINE - 1); j++) //0,2
	//	{
	//		log("removeBeEliminate2:%d,%d", i, j);
	//		if (elements[i][j])
	//		{
	//			log("removeBeEliminate3:%d,%d",i,j);
	//			if (elements[i][j]->getElement() == 6)
	//			{
	//				removeElement(i, j); // 移除精灵
	//				//修改瓦片属性
	//				changeTiledType(i, j);

	//			}
	//		}
	//	}

	//}
	//上下左右
	if (elements[r][max(l - 1, 0)] && !signFlag[r][max(l - 1, 0)])
	{
		if (elements[r][max(l - 1, 0)]->getElement() == 6) //被消除类型	
		{
			elements[r][max(l - 1, 0)]->removeFromParent();
			auto s = ElementUnit::create();
			if (s)
			{
				s->createElement(7, getMCenterByCoord(r, max(l - 1, 0)));
				elements[r][max(l - 1, 0)] = s;
				addChild(elements[r][max(l - 1, 0)], 1);
				jitterEffect(elements[r][max(l - 1, 0)],2.0);
			}

			signBeRemove[r][max(l - 1, 0)] = true; //同一次标记

		}
		else if (elements[r][max(l - 1, 0)]->getElement() == 7 && !signBeRemove[r][max(l - 1, 0)])
		{
			showCloudEffect(blocksCenter[r][max(l - 1, 0)]);
			removeElement(r, max(l - 1, 0)); // 移除精灵
			//修改瓦片属性
			changeTiledType(r, max(l - 1, 0));
		}
		else if (elements[r][max(l - 1, 0)]->getElement() == 8)
		{
			removeElement(r, max(l - 1, 0)); // 移除love
			changeTiledType(r, max(l - 1, 0));
			showLoveEffect(blocksCenter[r][max(l - 1, 0)]);
			GAMEDATA->setLoveNum(GAMEDATA->getLoveNum() + 1); //加一
		}
	}
	if (elements[r][min(l + 1, 5)] && !signFlag[r][min(l+1,5)])
	{
		if (elements[r][min(l+1,5)]->getElement() == 6)
		{
			elements[r][min(l + 1, 5)]->removeFromParent();
			auto s = ElementUnit::create();
			if (s)
			{
				s->createElement(7, getMCenterByCoord(r, min(l + 1, 5)));
				elements[r][min(l + 1, 5)] = s;
				addChild(elements[r][min(l + 1, 5)], 1);
				jitterEffect(elements[r][min(l + 1, 5)], 2.0);
			}

			signBeRemove[r][min(l + 1, 5)] = true;
		}
		else if (elements[r][min(l + 1, 5)]->getElement() == 7 && !signBeRemove[r][min(l + 1, 5)])
		{
			showCloudEffect(blocksCenter[r][min(l + 1, 5)]);
			removeElement(r, min(l + 1, 5)); // 移除精灵
			//修改瓦片属性
			changeTiledType(r, min(l + 1, 5));
		}
		else if (elements[r][min(l + 1, 5)]->getElement() == 8)
		{
			removeElement(r, min(l + 1, 5)); // 移除精灵
			changeTiledType(r, min(l + 1, 5));
			showLoveEffect(blocksCenter[r][min(l + 1, 5)]);
			GAMEDATA->setLoveNum(GAMEDATA->getLoveNum() + 1); //加一
		}
	}
	if (elements[max(r - 1, 0)][l] && !signFlag[max(r - 1, 0)][l])
	{
		if (elements[max(r - 1, 0)][l]->getElement() == 6)
		{
			elements[max(r - 1, 0)][l]->removeFromParent();
			auto s = ElementUnit::create();
			if (s)
			{
				s->createElement(7, getMCenterByCoord(max(r - 1, 0), l));
				elements[max(r - 1, 0)][l] = s;
				addChild(elements[max(r - 1, 0)][l], 1);
				jitterEffect(elements[max(r - 1, 0)][l], 2.0);
			}
			signBeRemove[max(r - 1, 0)][l] = true;
		}
		else if (elements[max(r - 1, 0)][l]->getElement() == 7 && !signBeRemove[max(r - 1, 0)][l])
		{
			showCloudEffect(blocksCenter[max(r - 1, 0)][l]);
			removeElement(max(r - 1, 0), l); // 移除精灵
			//修改瓦片属性
			changeTiledType(max(r - 1, 0), l);
		}
		else if (elements[max(r - 1, 0)][l]->getElement() == 8)
		{
			removeElement(max(r - 1, 0), l); // 移除精灵
			changeTiledType(max(r - 1, 0), l);
			showLoveEffect(blocksCenter[max(r - 1, 0)][l]);
			GAMEDATA->setLoveNum(GAMEDATA->getLoveNum() + 1); //加一
		}
	}
	if (elements[min(r + 1, 5)][l] && !signFlag[min(r + 1, 5)][l])
	{
		if (elements[min(r + 1, 5)][l]->getElement() == 6)
		{
			elements[min(r + 1, 5)][l]->removeFromParent();
			auto s = ElementUnit::create();
			if (s)
			{
				s->createElement(7, getMCenterByCoord(min(r + 1, 5), l));
				elements[min(r + 1, 5)][l] = s;
				addChild(elements[min(r + 1, 5)][l], 1);
				jitterEffect(elements[min(r + 1, 5)][l], 2.0);
			}
			signBeRemove[min(r + 1, 5)][l] = true;
		}
		else if (elements[min(r + 1, 5)][l]->getElement() == 7 && !signBeRemove[min(r + 1, 5)][l])
		{
			showCloudEffect(blocksCenter[min(r + 1, 5)][l]);
			removeElement(min(r + 1, 5), l); // 移除精灵
			//修改瓦片属性
			changeTiledType(min(r + 1, 5), l);
		}
		else if (elements[min(r + 1, 5)][l]->getElement() == 8)
		{
			removeElement(min(r + 1, 5), l); // 移除精灵
			changeTiledType(min(r + 1, 5), l);
			showLoveEffect(blocksCenter[min(r + 1, 5)][l]);
			GAMEDATA->setLoveNum(GAMEDATA->getLoveNum() + 1); //加一
		}
	}
}

//初始被同一次被消标记
void	MapLayer:: initSignBeRemove()
{
	clearSignBeRemove();
}

//清除被消标记
void	MapLayer:: clearSignBeRemove()
{
	ERGODIC_MBLOCK(row, line)
	{
		signBeRemove[row][line] = false;
	}
}

// 修改指定位置上瓦片的属性值
void	MapLayer::changeTiledType(int r, int l)
{
	unsigned int gidnum;
	for (int i = 0; i < 6; i++) //从第一行找到适合替换的瓦片格子
	{
		int t = _tileMap->getPropertiesForGID(typeLayer->getTileGIDAt(Vec2(i, 0))).asValueMap()["type"].asInt();
		if (t == 1)
		{
			gidnum = typeLayer->getTileGIDAt(Vec2(i, 0));
			break;
		}
	}
	
	unsigned int num = typeLayer->getTileGIDAt(Vec2(r, l));
	Value typevalue = _tileMap->getPropertiesForGID(num);
	int type = typevalue.asValueMap()["type"].asInt();
	if (type == 6 || type == 5)
	{
		typeLayer->setTileGID(gidnum, Vec2(r, l));
	}

}
//删除所有线
void MapLayer::removeAllLine()
{
	while (removeLatestLine());  //不断删除线序列中最后一条连接线，直至为空
	while (removeEffect());
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

	showRemoveEffect(blocksCenter[row][line]);

	//showRemoveEleEffect(blocksCenter[row][line]);

	temp->disappear(FALL_TIME);//元素缩小至消失FALL_TIME

	temp = nullptr; 
	elements[row][line] = nullptr;
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
	{//爆炸道具不减步数
		isCount = false;
		return m_count;
		
	}
	else
	{
			if (m_count == GAMEDATA->getCount(GAMEDATA->getCurLevel())-1)
			{
				//如果过关了就不弹出这个窗口，可以发消息到gamescene
				auto time = DelayTime::create(1.0);
				auto callFun = CallFunc::create([=]{
					if (!isTargetElementFinish)
					{
						auto layer = AddCount::create();
						addChild(layer, 1);
						m_count -= 4;
					}
				});
				auto seq = Sequence::create(time, callFun, nullptr);
				this->runAction(seq);
		
				return GAMEDATA->getCount(GAMEDATA->getCurLevel());
			}
			else
			{
				return ++m_count;
			}
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
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	LOGD("MapLayer::elementsFall()");
#endif
	//log("elementsFall()");
	bool flag = false; //是否填补了空位
	if (isRemoveFinish)
	{
		for (int j = MATRIX_LINE; j >= 0; --j)//5
		{
			for (int i = 0; i < MATRIX_ROW; ++i)//对每hang单独处理 0
			{
				if (elements[i][j] == nullptr)// && checkIsDrop(i, j)
				{
					if (checkIsDrop(i, j))
					{
						rowMyFall(i, j);
						flag = true;
						break;
					}
				}
			}
		}
	}
	return flag;
}

//检测是否可以下落
bool	MapLayer::checkIsDrop(int r, int l) // 检测的空格子
{
	//检测它的上一个，上左，上右是否可以有精灵落下来
	if (elements[r][l-1]) //上you 
	{
		if (checkIsnullptr(r,max(l-1,0))) //上一个是石头
		{ //看左右
			if (elements[max(r - 1, 0)][max(l - 1, 0)]) //左边有
			{
				if (checkIsnullptr(max(r - 1, 0), max(l - 1, 0))) //shitou //左边是否有石头 、
				{
					if (elements[min(r + 1, 5)][max(l - 1, 0)]) //右边有
					{
						if (checkIsnullptr(min(r + 1, 5), max(l - 1, 0))) //左边是石头，判断右边
						{
							//上左右都是石头
							return false;
						}
						else //右边有且不是石头
						{
							//上左石头右有且不是
							return true; //表示是普通元素
						}
					}
					else //右边是空，判断右边是否是一个可以下落得到元素的格子
					{
						if (checkIsDrop(min(r + 1, 5), max(l - 1, 0))) //可以下落
						{
							return true;
						}
						else
						{
							return false;
						}
					}
				}
				else //左有 且不是石头
				{
					return true;
				}
			}
			else //上左为空
			{
				//判断是不是一个可以通过下落得到元素的格子
				if (checkIsDrop(max(r - 1, 0), max(l - 1, 0))) //调用自身来判断
				{
					return true;//可以下落的格子
				}
				else //需要判断右边是不是可以下落 
				{
					if (elements[min(r + 1, 5)][max(l - 1, 0)]) //右边有
					{
						if (checkIsnullptr(min(r + 1, 5), max(l - 1, 0))) //左边是石头，判断右边
						{
							//上左右都是石头
							return false;
						}
						else //右边有且不是石头
						{
							//上左石头右有且不是
							return true; //表示是普通元素
						}
					}
					else //右边是空，判断右边是否是一个可以下落得到元素的格子
					{
						if (checkIsDrop(min(r + 1, 5), max(l - 1, 0))) //可以下落
						{
							return true;
						}
						else
						{//左空不可以，上石头，右空不可以
							return false;
						}
					}
				}
			}
		}
		else //上一个有且不是石头
		{
			return true;
		}
	}
	else //上一个格子是空
	{
		//判断是不是一个可以通过下落得到元素的格子调用自身
		if (checkIsDrop(r,max(l-1,0)))
		{
			return true;
		}
		else  //表示上空，但是属于不可以下落的格子，这样话要判断左右
		{
			if (elements[max(r - 1, 0)][max(l - 1, 0)]) //左边有
			{
				if (checkIsnullptr(max(r - 1, 0), max(l - 1, 0))) //shitou //左边是否有石头 、
				{
					if (elements[min(r + 1, 5)][max(l - 1, 0)]) //右边有
					{
						if (checkIsnullptr(min(r + 1, 5), max(l - 1, 0))) //左边是石头，判断右边
						{
							//上左右都是石头
							return false;
						}
						else //右边有且不是石头
						{
							//上左石头右有且不是
							return true; //表示是普通元素
						}
					}
					else //右边是空，判断右边是否是一个可以下落得到元素的格子
					{
						if (checkIsDrop(min(r + 1, 5), max(l - 1, 0))) //可以下落
						{
							return true;
						}
						else
						{
							return false;
						}
					}
				}
				else //左有 且不是石头
				{
					return true;
				}
			}
			else //上左为空
			{
				//判断是不是一个可以通过下落得到元素的格子
				if (checkIsDrop(max(r - 1, 0), max(l - 1, 0))) //调用自身来判断
				{
					return true;//可以下落的格子
				}
				else //需要判断右边是不是可以下落 
				{
					if (elements[min(r + 1, 5)][max(l - 1, 0)]) //右边有
					{
						if (checkIsnullptr(min(r + 1, 5), max(l - 1, 0))) //左边是石头，判断右边
						{
							//上左右都是石头
							return false;
						}
						else //右边有且不是石头
						{
							//上左石头右有且不是
							return true; //表示是普通元素
						}
					}
					else //右边是空，判断右边是否是一个可以下落得到元素的格子
					{
						if (checkIsDrop(min(r + 1, 5), max(l - 1, 0))) //可以下落
						{
							return true;
						}
						else
						{//左空不可以，上空不可以，右空不可以
							return false;
						}
					}
				}
			}
		}
	}
}

bool MapLayer::checkIsnullptr(int _row, int _line)
{ 
	unsigned int num;
	num = typeLayer->getTileGIDAt(Vec2(_row, _line));
	getType = _tileMap->getPropertiesForGID(num);
	int _type = getType.asValueMap()["type"].asInt();
	if (_type != 1)
	{
		return true;
	}
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

void	MapLayer::rowMyFall(int _row, int bottom) // 15 //前提就判断了这个格子是满足掉落条件的，那么就通过循环将格子铺满
{ //在上左右寻找可以掉落的格子，在line为0的时候创建
	int	j = bottom - 1;
	if (j>=0) //空格子是在第二排以下
	{//优先匹配上，再是左，右
		if (elements[_row][j]) //上有
		{
			if (checkIsnullptr(_row,j)) //上是特殊元素不能下落返回的是true
			{ //检测左边
				rightAndLeft(_row,bottom);
			}
			else //上有且不是石头
			{
				elements[_row][bottom] = elements[_row][j];////上
				if (elements[_row][bottom])
				{
					Point pop = getMCenterByCoord(_row, bottom);
					elements[_row][j]->moveToPosition(pop, FALL_TIME);
					elements[_row][j] = nullptr;
				}
			}
		}
		else //上格子是空 
		{//那就判断下这个是可以掉落的空格子不
			if (checkIsDrop(_row,j)) //是可以掉落的
			{
				elements[_row][bottom] = elements[_row][j]; ////上
				if (elements[_row][bottom])
				{
					Point pop = getMCenterByCoord(_row, bottom);
					elements[_row][j]->moveToPosition(pop, FALL_TIME);
					elements[_row][j] = nullptr;
				}
			}
			else //上是空，但是是不可以掉落的格子，那么就需要去检测左边，右边
			{
				rightAndLeft(_row, bottom); //判断左右
			}
		}
	}
	if (j < 0)
	{
		appear(_row);
	}
}

void MapLayer::rightAndLeft(int _row, int bottom)
{
	int	j = bottom - 1;
	if (elements[max(_row - 1, 0)][j]) //上左有
	{
		if (checkIsnullptr(max(_row - 1, 0), j)) //是石头
		{ //那就判断右边，因为是可以下落的，所以有一个可以满足条件
			if (elements[min(_row + 1, 5)][j]) //上右有
			{
				if (checkIsnullptr(min(_row + 1, 5), j))
				{
					log("dou shi shi tou ,zou dao zhe bu ke xue!!!!");
				}
				//上右有且是可以掉落的基本元素
				elements[_row][bottom] = elements[min(_row + 1, 5)][j];
				if (elements[_row][bottom])
				{
					Point pop = getMCenterByCoord(_row, bottom);
					elements[min(_row + 1, 5)][j]->moveToPosition(pop, FALL_TIME);//上方元素掉落一个
					elements[min(_row + 1, 5)][j] = nullptr;
				}
			}
			else //上右 是空
			{
				//那就判断下这个是可以掉落的空格子不
				if (checkIsDrop(min(_row + 1, 5), j)) //是可以掉落的
				{
					elements[_row][bottom] = elements[min(_row + 1, 5)][j];
					if (elements[_row][bottom])
					{
						Point pop = getMCenterByCoord(_row, bottom);
						elements[min(_row + 1, 5)][j]->moveToPosition(pop, FALL_TIME);//上方元素掉落一个
						elements[min(_row + 1, 5)][j] = nullptr;
					}
				}
				else
				{
					log("te me zou dao zhe ye shi bu ke xue de ba !!!!");
				}

			}
		}
		else//上左有且不是石头
		{//那就是基本元素喽
			elements[_row][bottom] = elements[max(_row - 1, 0)][j]; ////上左
			if (elements[_row][bottom])
			{
				Point pop = getMCenterByCoord(_row, bottom);
				elements[max(_row - 1, 0)][j]->moveToPosition(pop, FALL_TIME);
				elements[max(_row - 1, 0)][j] = nullptr;
			}
		}
	}
	else//上左是空
	{
		//那就判断下这个是可以掉落的空格子不
		if (checkIsDrop(max(_row - 1, 0), j)) //是可以掉落的
		{
			elements[_row][bottom] = elements[max(_row - 1, 0)][j]; ////上左
			if (elements[_row][bottom])
			{
				Point pop = getMCenterByCoord(_row, bottom);
				elements[max(_row - 1, 0)][j]->moveToPosition(pop, FALL_TIME);
				elements[max(_row - 1, 0)][j] = nullptr;
			}
		}
		else //是空，但是不可以掉落 /那么意味着，要去检测上右
		{
			if (elements[min(_row + 1, 5)][j]) //you
			{
				if (checkIsnullptr(min(_row + 1, 5), j))
				{
					log("zhe ge zhen de bu ke xue!!!!");
				}
				else //有且不是石头可以下落
				{
					elements[_row][bottom] = elements[min(_row + 1, 5)][j];
					if (elements[_row][bottom])
					{
						Point pop = getMCenterByCoord(_row, bottom);
						elements[min(_row + 1, 5)][j]->moveToPosition(pop, FALL_TIME);//上方元素掉落一个
						elements[min(_row + 1, 5)][j] = nullptr;
					}
				}
			}
			else //nullptr
			{
				if (checkIsDrop(min(_row + 1, 5), j))//直接检测右边是不是可以掉落，是的话直接掉落 ***其实需要检测是不是有
				{
					elements[_row][bottom] = elements[min(_row + 1, 5)][j];
					if (elements[_row][bottom])
					{
						Point pop = getMCenterByCoord(_row, bottom);
						elements[min(_row + 1, 5)][j]->moveToPosition(pop, FALL_TIME);//上方元素掉落一个
						elements[min(_row + 1, 5)][j] = nullptr;
					}
				}
				else
				{
					log("wo ca zhe ye shi bu ke xue de !!");
				}
			}

		}

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
			if (checkIsnullptr(r, i))
			{
				//--i;
				isapp = false;
				continue;
				//break;
			}
			if (j >= 0 )
			{
				if (checkIsnullptr(r, j))
				{
					//log("while (checkSpritenullptr({ _row, j })):%i",j);
					//j--;
					isapp = false;
					if (!checkIsnullptr(max(r - 1, 0), j) )
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
							if (!checkIsnullptr(min(r + 1, 5), j))
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
					else if (!checkIsnullptr(min(r + 1, 5), j))
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
							if (!checkIsnullptr(max(r - 1, 0), j))
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
					elements[r][i] = elements[r][j]; //nullptr
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
	_tileMap = TMXTiledMap::create(str);//("TileMaps/hexa-test.tmx");
	//_tileMap = TMXTiledMap::create("tiledmap/map_25.tmx");//("TileMaps/hexa-test.tmx");
	if (_tileMap)
	{
		_tileMap->setAnchorPoint(Vec2(0.5, 0.5));
		_tileMap->setPosition(mapPoint);
		_tileMap->setScale(RATIO);
		addChild(_tileMap, -1);

		mapLayer = _tileMap->getLayer("map");
		typeLayer = _tileMap->getLayer("type");
		typeLayer->setVisible(false);

		//line = _tileMap->getMapSize().height; // 
		//row = _tileMap->getMapSize().width; // 
		//log("%d,%d", row,line);
		////mei shi xian
		containsDis = (typeLayer->getMapTileSize().width*RATIO) / 2 - 5;
		
	}
	//初始化数量
	//for (int i = 0; i < 40; ++i) //0-5
	//{
	//	//--elementCount[i] = 0;
	//	//--removeCount[i] = 0;
	//}

	ERGODIC_MBLOCK(row,line)
	{
		//log("ERGODIC_MBLOCK :row:%d,line:%d",row,line);
		blocksCenter[row][line] = getMCenterByCoord(row, line); //确定格子中心位置
			/*	int  num = (uint32_t)typeLayer->getTileGIDAt(Vec2(i, j));*/
				//getType = _tileMap->getPropertiesForGID(num);
				//int _type = getType.asValueMap()["type"].asInt();
		if (checkIsnullptr(row, line))
		{
			unsigned int num;
			num = typeLayer->getTileGIDAt(Vec2(row, line));
			int _type = _tileMap->getPropertiesForGID(num).asValueMap()["type"].asInt();
			//这块瓦片是不显示 
			auto s = ElementUnit::create();
			if (s)
			{
				//s->setElementType(ElementType::cannotEliminate); // 不可一消除的元素
				s->createElement(_type, getMCenterByCoord(row, line));
				elements[row][line] = s;
				//--elementCount[_type]++;
				addChild(elements[row][line], 1);
				//elements[row][line]->setVisible(false);
				if (_type == 8)
				{//爱心
					elements[row][line]->setScale(0.7);
					auto seq = Sequence::create(ScaleTo::create(0.5, 1.2), ScaleTo::create(0.6, 0.9),nullptr);
					elements[row][line]->runAction(RepeatForever::create(seq));
				}
				if (_type == 6)
				{
					jitterEffect(elements[row][line],2.0);
					
				}
			}
		}
		else
		{
			auto s = ElementUnit::create();
			if (s)
			{
				//s->setElementType(ElementType::basisEliminate); // 基础消除元素
				int color = abs(rand() % 5);
				s->createElement(color, getMCenterByCoord(row, line));
				elements[row][line] = s;
				//--elementCount[color]++;
				addChild(elements[row][line], 1);
			}		
		}
	}
	removeAllCount = 0;

	while (checkIsNeedRefresh())
	{
		//log("refresh!"); //保证创建的第一个地图是有可以连线的地图
		refreshAllElement();
	}
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
				if ((getElement({ row, line }) >= 5 && getElement({ row, line }) < 10) || getElement({ row, line }) >= 30)
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
					showEffect(blocksCenter[row][line]);//blocksCenter[row][line]
					showQQEffect(elements[row][line]);
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
					if (elements[_row][_line])
					{
						if (elements[_row][_line]->getElement() >= 30) //撤销的是大于30的特殊元素，取消变换元素
						{
							undoChangeElement();
						}
					}
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
									showEffect(blocksCenter[i][j]);//blocksCenter[i][j]
									showQQEffect(elements[i][j]);
									//特殊元素30以上将周围3*3变色

									if (elements[i][j]->getElement() >= 10)
									{ 
										elementType = elements[i][j]->getElement() % 10;//获得元素类型
										if (elements[i][j]->getElement() >= 30)
										{
											changeElement(elementType,i,j); // 修改精灵
										}
									}
									else
									{
										elementType = elements[i][j]->getElement();//获得元素类型
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
					GAMEDATA->setBoomb(GAMEDATA->getBoomb() - 1);
					showBoomEffect(blocksCenter[row][line]);
					isBoomElement(row, line);
					removeSignSpecial();
					EventCustom _event(REFRESHPROPS);
					_eventDispatcher->dispatchEvent(&_event);
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
			
			temp_element.clear();
			temp_elementPos.clear();
		}
	}

	touchedFlag = false;  //触摸结束
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	LOGD("onTouchEnded");
#endif
	//if (UserDefault::getInstance()->getBoolForKey("IS_VIBRATE",true))
	//	Custom::Vibrator::cancelVibrate();
}

//修改周围三乘三的元素
void	MapLayer::changeElement(int ele, int r, int l)
{
	temp_element.clear();
	temp_elementPos.clear();
	for (int i = max(r - 1, 0); i <= min(r + 1, MATRIX_ROW - 1); i++) // 0,2
	{
		for (int j = max(l - 1, 0); j <= min(l + 1, MATRIX_MLINE - 1); j++) //0,2
		{
			if (elements[i][j])
			{
				int typeElement = elements[i][j]->getElement();
				if (typeElement < 5 && typeElement != ele) //特殊元素
				{
					temp_element.push_back(typeElement); // 按顺序存入类型
					temp_elementPos.push_back({i,j}); //存入坐标
					//--elementCount[typeElement]--;
					elements[i][j]->removeFromParent();
					auto s = ElementUnit::create();
					if (s)
					{
						s->createElement(ele, blocksCenter[i][j]);
						elements[i][j] = s;
						//--elementCount[ele]++;
						addChild(elements[i][j],1);

					}

				}
			}
		}

	}
}
//撤销修改的三乘三元素
void	MapLayer::undoChangeElement()
{
	for (int i = 0; i < temp_elementPos.size(); i++)
	{
		if (elements[temp_elementPos[i].row][temp_elementPos[i].line])
		{
			int te = elements[temp_elementPos[i].row][temp_elementPos[i].line]->getElement();
			//--elementCount[te]--;
			elements[temp_elementPos[i].row][temp_elementPos[i].line]->removeFromParent();
			auto s = ElementUnit::create();
			if (s)
			{
				s->createElement(temp_element[i], blocksCenter[temp_elementPos[i].row][temp_elementPos[i].line]);
				elements[temp_elementPos[i].row][temp_elementPos[i].line] = s;
				//--elementCount[temp_element[i]]++;
				addChild(elements[temp_elementPos[i].row][temp_elementPos[i].line], 1);
			}
		}

	}
	if (!temp_element.empty())
	{
		temp_element.clear();
	}if (!temp_elementPos.empty())
	{
		temp_elementPos.clear();
	}
}

void MapLayer::showEffect(Point p)
{
	auto guang = Sprite::create("effect/element_quan.png");
	guang->setPosition(Vec2(p.x,p.y+3));
	addChild(guang,1);

	auto rotate_ac = RotateBy::create(10.0f, 360.0);
	guang->runAction(RepeatForever::create(rotate_ac));

	//ParticleSystem * efft = ParticleSystemQuad::create("effect/particle_1.plist");
	//efft->setPosition(p);
	//addChild(efft, 1);
	m_effects.pushBack(guang);
}

bool MapLayer::removeEffect()
{
	if (m_effects.empty())
	{
		return false;
	}
	Sprite* e = m_effects.back();
	m_effects.popBack();
	removeChild(e);
	return true;
}

void MapLayer::showRemoveEffect(Point p)
{
	ParticleSystem * efft = ParticleSystemQuad::create("effect/bomb_4.plist");
	efft->setPosition(p);
	addChild(efft, -1);
	efft->setTotalParticles(800);
	efft->setSpeedVar(0);
	efft->setStartSizeVar(0);
	efft->setScale(0.16f);

	//SpriteFrameCache *frameCache = SpriteFrameCache::sharedSpriteFrameCache();
	//frameCache->addSpriteFramesWithFile("effect/removeEffect.plist");

	//auto s = Sprite::createWithSpriteFrameName("remove_effe_0.png");
	//if (s)
	//{
	//	s->setPosition(Vec2(p.x, p.y));
	//	s->setScale(1.35);
	//	addChild(s, 1);
	//	auto a = CommonFunction::createWithSingleFrameName("remove_effe_", 0.09, 1);
	//	Animate* animate = CCAnimate::create(a);
	//	CallFunc* call = CallFunc::create([=]{
	//		if (s)
	//		{
	//			removeChild(s);
	//		}
	//	});
	//	Sequence* action = Sequence::create(animate, call, nullptr);
	//	s->runAction(action);
	//}
}

void MapLayer::showQQEffect(Node* node)
{
	auto s1 = ScaleTo::create(0.05,0.85,1.12);
	auto s2 = ScaleTo::create(0.06, 0.89, 1.08);
	auto s3 = ScaleTo::create(0.065,1.05,0.92);
	auto s4 = ScaleTo::create(0.07, 1.03, 0.96);
	auto s5 = ScaleTo::create(0.075, 1, 1);

	auto s6 = ScaleTo::create(0.055, 0.88, 1.09);
	auto s7 = ScaleTo::create(0.06, 0.9, 1.05);
	auto s8 = ScaleTo::create(0.065, 1.02, 0.98);
	auto s9 = ScaleTo::create(0.07, 1.01, 0.99);
	auto s10 = ScaleTo::create(0.07, 1, 1);

	auto s11 = ScaleTo::create(0.06, 0.9, 1.06);
	auto s12 = ScaleTo::create(0.065, 0.95, 1.04);
	auto s13 = ScaleTo::create(0.07, 0.97, 1.02);
	//auto s14 = ScaleTo::create(0.075, 0.99, 1.0);
	auto s15 = ScaleTo::create(0.08, 1, 1);
	auto seq = Sequence::create(s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13, s15, nullptr);// s6, s7, s8, s9, s10,s14,

	node->runAction(Repeat::create(seq,1));
}

void MapLayer::showLineEffect(Point p)
{
	SpriteFrameCache *frameCache = SpriteFrameCache::sharedSpriteFrameCache();
	frameCache->addSpriteFramesWithFile("effect/Line.plist");
	if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
		AudioData::getInstance()->addSpecialEffect(3);
	auto s = Sprite::createWithSpriteFrameName("line_0.png");
	if (s)
	{
		s->setPosition(Vec2(mapPoint.x,p.y));
		//s->setScale(1);
		addChild(s, 1);
		auto a = CommonFunction::createWithSingleFrameName("line_", 0.03, 1);
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
	SpriteFrameCache *frameCache = SpriteFrameCache::sharedSpriteFrameCache();
	frameCache->addSpriteFramesWithFile("effect/Line.plist");
	if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
		AudioData::getInstance()->addSpecialEffect(2);

	auto s = Sprite::createWithSpriteFrameName("line_0.png");
	if (s)
	{
		s->setPosition(Vec2(p.x, mapPoint.y));
		s->setRotation(90.0f);
		addChild(s, 1);
		auto a = CommonFunction::createWithSingleFrameName("line_", 0.03, 1);
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

void MapLayer::showCloudEffect(Point p)
{
	SpriteFrameCache *frameCache = SpriteFrameCache::sharedSpriteFrameCache();
	frameCache->addSpriteFramesWithFile("effect/cloud_effect.plist");

	if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
		AudioData::getInstance()->addSpecialEffect(2);

	auto s = Sprite::createWithSpriteFrameName("cloud_0.png");
	if (s)
	{
		s->setPosition(Vec2(p.x, p.y));
		addChild(s, 1);
		auto a = CommonFunction::createWithSingleFrameName("cloud_", 0.02, 1);
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

void MapLayer::showLoveEffect(Point p)
{
	SpriteFrameCache *frameCache = SpriteFrameCache::sharedSpriteFrameCache();
	frameCache->addSpriteFramesWithFile("effect/Love_Bomb.plist");

	auto s = Sprite::createWithSpriteFrameName("love_effect_0.png");
	if (s)
	{
		s->setPosition(Vec2(p.x, p.y));
		addChild(s, 1);
		auto a = CommonFunction::createWithSingleFrameName("love_effect_", 0.07, 1);
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

void MapLayer::showRemoveEleEffect(Point p)
{
	SpriteFrameCache *frameCache = SpriteFrameCache::sharedSpriteFrameCache();
	frameCache->addSpriteFramesWithFile("effect/LoveBomb.plist");
	//if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
	//	AudioData::getInstance()->addSpecialEffect(2);
	auto s = Sprite::createWithSpriteFrameName("love_0.png");
	if (s)
	{
		s->setPosition(Vec2(p.x, p.y));
		addChild(s, 1);
		auto a = CommonFunction::createWithSingleFrameName("love_", 0.1, 1);
		Animate* animate = CCAnimate::create(a);
		CallFunc* call = CallFunc::create([=]{
			if (s)
			{
				removeChild(s);
			}
		});
		Sequence* action = Sequence::create(animate, call, nullptr);
		s->setScale(0.7);
		s->runAction(action);
	}
}

void MapLayer::showBoomEffect(Point p)
{
	ParticleSystem * efft = ParticleSystemQuad::create("effect/particle_6.plist");
	if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
		AudioData::getInstance()->addSpecialEffect(1);
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
				signSpecialElement({ i, j }); //标记为特殊消除
				if (elements[i][j]->getElement() == 5 || elements[i][j]->getElement() == 6)
				{
					changeTiledType(i,j);
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
	//log("MapLayer::specialSignElement(int ele, Coord c)");
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
						/*
						log("specialSignElement(elements[i][c.line]->getElement(), {i,c.line})");*/
						signFlagSpecial[i][c.line] = true; // 是特殊元素，给自己加上标记，下次检测的时候就不在检测了，否则会进入死循环一直递归下去
						specialSignElement(elements[i][c.line]->getElement(), {i,c.line});//调用自己，递归的用法。
					}

				}
			}

		}
	}
	if (ele >= 20 && ele < 30) //shuaxiang
	{
		//log("ele :%d", ele);
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
		//log("ele :%d", ele);
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
	if (elements[latest.row][latest.line])
	{
		showQQEffect(elements[latest.row][latest.line]);

	}
	linkIndex.pop_back(); //清除连线序列的最后一个元素
	removeLatestLine(); //删除最后一条连接线
	removeEffect(); // 删除最后一个元素的特效
	unsignElement(latest); //取消最后一个元素的标记
	int number = linkIndex.size();

	if (number > 14)
	{
		if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
			AudioData::getInstance()->addLinkEffect(15);
	}
	else
	{
		if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
			AudioData::getInstance()->addLinkEffect(number);
	}
	//if (UserDefault::getInstance()->getBoolForKey("IS_VIBRATE",true))
	//	Custom::Vibrator::vibrate(UNDO_VIBRATOR_TIME); //震动
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
				//--removeCount[ele]++; //消除标记的元素
				specialScore += specialSttlement(ele);
				if (ele >= 10 && ele < 20)
				{
					showLineEffect(blocksCenter[row][line]);
				}
				if (ele >= 20 && ele < 30)
				{
					showRowEffect(blocksCenter[row][line]);
				}
				if (ele > 5 && ele < 10)
				{
					changeTiledType(row,line);
					//showBoomEffect(blocksCenter[row][line]);
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
		eventTargetElement(0,1);
		return 50;
		break;
	case 1:
		eventTargetElement(1, 1);
		return 55;
		break;
	case 2:
		eventTargetElement(2, 1);
		return 60;
		break;
	case 3:
		eventTargetElement(3, 1);
		return 65;
		break;
	case 4:
		eventTargetElement(4, 1);
		return 70;
		break;
	case 5 :
		break;
	default:
		//return 75;
		break;
	}
}

void MapLayer:: eventTargetElement(int ele, int count)
{
	int index = GAMEDATA->getCurLevel();
	if (ele == 0)
	{
		if (REWARDDATA->getBlue(index) != 0)
		{
			removeTargetECount = count;
			EventCustom _event(BLUE);
			_eventDispatcher->dispatchEvent(&_event);
		}
	}
	if (ele == 1)
	{
		if (REWARDDATA->getPurple(index) != 0)
		{
			removeTargetECount = count;
			EventCustom _event(PURPLE);
			_eventDispatcher->dispatchEvent(&_event);
		}

	}
	if (ele == 2)
	{
		if (REWARDDATA->getGreen(index) != 0)
		{
			removeTargetECount = count;
			EventCustom _event(GREEN);
			_eventDispatcher->dispatchEvent(&_event);
		}

	}
	if (ele == 3)
	{
		if (REWARDDATA->getRad(index) != 0)
		{
			removeTargetECount = count;
			EventCustom _event(RAD);
			_eventDispatcher->dispatchEvent(&_event);
		}

	}
	if (ele == 4)
	{
		if (REWARDDATA->getYellor(index) != 0)
		{
			removeTargetECount = count;
			EventCustom _event(YELLOR);
			_eventDispatcher->dispatchEvent(&_event);
		}

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
	auto line = DrawLine::create(getMCenterByCoord(from), getMCenterByCoord(to), "effect/link_0.png");
	addChild(line,-1);

	linkBrush.push_back(line); //将该连接线添加到线序列中
}

//删除最后一条连接线，返回是否删除成功
bool MapLayer::removeLatestLine()
{
	if (linkBrush.empty())
	{
		return false;
	}
	DrawLine* brush = linkBrush.back();
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
	int number = GAMEDATA->getSpecialNum(GAMEDATA->getCurLevel());
	//log("appear()=%d", number);
	int top = 0;
	int ele = abs(rand() % 5);// randElement(); // 0 1 2 3 4 
	if (green >= number)
	{
		//srand((int)time(0));
		ele = 12 + (abs(rand() % 3) * 10); //12 22 32 12+0,12+10,12+20
		green = 0;
	}
	if (blue >= number)
	{
		ele = 10 + (abs(rand() % 3) * 10); //12 22 32 12+0,12+10,12+20
		blue = 0;
	}
	if (red >= number)
	{
		ele = 13 + (abs(rand() % 3) * 10); //12 22 32 12+0,12+10,12+20
		red = 0;
	}
	if (yellor >= number)
	{
		ele = 14 + (abs(rand() % 3) * 10); //12 22 32 12+0,12+10,12+20
		yellor = 0;
	}
	if (purple >= number)
	{
		ele = 11 + (abs(rand() % 3) * 10); //12 22 32 12+0,12+10,12+20
		purple = 0;
	}
	createElement(ele, row, top);
	if (elements[row][top])
	{
		this->addChild(elements[row][top]);
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

	//int *num = new int[ELEMENT_COUNT];//用于存放各种元素出现的概率

	////获取元素类型数量的最大最小值
	//int max = 0;
	//int min = 0x7FFFFFFF;
	//for (int i = 0; i < ELEMENT_COUNT; ++i)
	//{
	//	int temp = elementCount[i];
	//	if (temp >max)
	//	{
	//		max = temp;
	//	}
	//	if (temp < min)
	//	{
	//		min = temp;
	//	}
	//}
	//if (max - min <= MAX_GAP)
	//{
	//	max++;//如果最多元素与最少元素差距没有过大，则最多元素仍有可能出现
	//}
	//for (int i = 0; i < ELEMENT_COUNT; ++i)
	//{
	//	num[i] = max - elementCount[i];//根据元素类型数量确定概率，已存在数量越多，概率越小
	//}

	//int re = randNum(ELEMENT_COUNT, num);//根据概率随机获得一个结果
	//delete[] num;
	//return re;
	return 1;
}



//设置连线的起点
void MapLayer::linkStart(int row, int line)
{
	linkIndex.clear(); //清空连线序列
	signOnlyBlock(row, line); //标记唯一元素
	linkIndex.push_back({ row, line });//添加到连线序列
	//音效，震动
	if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
		AudioData::getInstance()->addLinkEffect(1);
	//if (UserDefault::getInstance()->getBoolForKey("IS_VIBRATE",true))
	//	Custom::Vibrator::vibrate(3); //震动
}

//连接两个元素
void MapLayer::linkElement(Coord from, Coord to)
{
	signElement(to);  //标记元素
	drawLine(from, to); //绘制连线
	linkIndex.push_back(to); //添加到连线序列
	int number = linkIndex.size();

	if (number > 14)
	{
		if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
			AudioData::getInstance()->addLinkEffect(15);
	}
	else
	{
		if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
			AudioData::getInstance()->addLinkEffect(number);
	}
	//if (UserDefault::getInstance()->getBoolForKey("IS_VIBRATE",true))
	//	Custom::Vibrator::vibrate(LINK_VIBRATOR_TIME); //震动
}

//清空矩阵元素
void MapLayer::clearElement()
{
	//mei shi xian
	ERGODIC_MBLOCK(row,line)
	{
		if (elements[row][line])
		{
			removeElement(row, line); //清除每个格子的元素
		}	
	}
	//for (int i = 0; i < 40; ++i)
	//{
	//	//--elementCount[i] = 0;
	//}
}

//在指定位置创建给定类型的元素
int  MapLayer::createElement(int element, int row, int line) //22
{
		//ElementUnit* &temp = elements[row][line]; //定义临时变量，简化代码
		elements[row][line] = ElementUnit::create();
		//--elementCount[element]++; // 对应元素类型基数自增
		if (elements[row][line])
		{
			//elements[row][line]->setElementType(ElementType::basisEliminate);//设置为基本消除属性
			elements[row][line]->createElement(element, getMCenterByCoord(row, line));//创建一个元素并添加到屏幕上
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
	for (int i = 0; i < 40; ++i)
	{
		//--removeCount[i] = 0;
	}
}

//只标记一个元素（其他标记被取消）
void MapLayer::signOnlyBlock(int row, int line)
{
	signClear();
	signElement(row, line);
}

//特效
void  MapLayer::jitterEffect(Node* node, float delay)
{
	auto f_node = dynamic_cast<ElementUnit*>(node);
	if (!f_node){
		return;
	}

	auto delayTime = DelayTime::create(delay);
	auto rotate_1 = RotateTo::create(0.10f, -15);
	auto rotate_2 = RotateTo::create(0.10f, 15);
	auto rotate_3 = RotateTo::create(0.10f, 0);
	auto seq = Sequence::create(delayTime, rotate_1, rotate_2, rotate_3, nullptr);
	auto repeat = RepeatForever::create(seq);
	f_node->runAction(repeat);
}
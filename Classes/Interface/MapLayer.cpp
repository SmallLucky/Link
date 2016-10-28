
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
	//�������Ԫ�ؿռ�
	elements = new ElementUnit* *[MATRIX_ROW];
	for (int i = 0; i < MATRIX_ROW; i++)
	{
		elements[i] = new ElementUnit*[MATRIX_MLINE];
	}

	//�����ǿռ�
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

	//����������Ŀռ�
	blocksCenter = new Point*[MATRIX_ROW];
	for (int i = 0; i < MATRIX_ROW; i++)
	{
		blocksCenter[i] = new Point[MATRIX_MLINE];
	}
	////�����Ԫ�������ռ�
	elementCount = new int[40]; //ע������Ԫ�ؿռ�

	////����������Ԫ�ص������ռ�
	removeCount = new int[40]; //ע������Ԫ�ؿռ�

	linkBrush.clear();
}
MapLayer::~MapLayer()
{
	CC_SAFE_DELETE_ARRAY(elements);
	//���ո���Ԫ�ؿռ�
	//for (int i = 0; i < MATRIX_ROW; i++)
	//	delete[] elements[i];
	//delete[] elements;
	//���ձ�ǿռ�
	CC_SAFE_DELETE_ARRAY(signFlag);
	//for (int i = 0; i < MATRIX_ROW; i++)
	//	delete[] signFlag[i];
	//delete[] signFlag;

	CC_SAFE_DELETE_ARRAY(signFlagSpecial);
	//for (int i = 0; i < MATRIX_ROW; i++)
	//	delete[] signFlagSpecial[i];
	//delete[] signFlagSpecial;
	//���ո������Ŀռ�
	CC_SAFE_DELETE_ARRAY(blocksCenter);
	//for (int i = 0; i < MATRIX_ROW; i++)
	//	delete[] blocksCenter[i];
	//delete[] blocksCenter;

	//���ո�Ԫ�������ռ�

	CC_SAFE_DELETE_ARRAY(elementCount);

	/*delete[] elementCount;
	elementCount = nullptr;*/
	CC_SAFE_DELETE_ARRAY(removeCount);
	//����������Ԫ�ص������ռ�
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

	initBlocks();//��ʼ�����ӣ�ȷ���������򣬳�ʼ״̬����Ϊ��
	initSign(); //��ʼ�����

	forbiddenResponse(); //��ʱ��ֹ�������ڽ���ȴ�����״̬��ͨ��allowResponse�������������̳���TouchableLayer��
	touchedFlag = false; //û����Ч�Ĵ���
	linkFinishFlag = false; //û�����ӽ���

	return true;
}

//����Ƿ���Ҫ����ˢ��λ��
bool  MapLayer::checkIsNeedRefresh()
{
	ERGODIC_MBLOCK(row, line)
	{
		//����һ��Ԫ����Χ������Ԫ�� // 1,1
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

//ˢ�����л���Ԫ�ص�λ��
void  MapLayer::refreshAllElement()
{
	ERGODIC_MBLOCK(row, line)
	{
		if (elements[row][line] != nullptr)
		{
			if (getElement({row,line}) < 5)
			{
				elements[row][line]->removeFromParent(); ///�����ǲ����Ƴ���ֻ�ܻ�λ��
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

//�������������������ӵ�Ԫ�أ���������ʧ��
void MapLayer::removeLink()
{
	if (linkIndex.size() >= MIN_ROMOVE_COUNT) //������ӵ�Ԫ�ظ����ﵽҪ��
	{
		removeSignedElement(); //���������ӵ�Ԫ��
		removeAllLine(); //ɾ��������	
		if (getIsLine())
		{
			removeSignSpecial();
			isLine = false;
		}
		signClear();  
		linkFinishFlag = true; //���һ������
		if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT",true))
			SimpleAudioEngine::getInstance()->playEffect(REMOVE_EFFECT); //������Ч
		if (UserDefault::getInstance()->getBoolForKey("IS_VIBRATE",true))
			Vibrator::vibrate(REMOVE_VIBRATOR_TIME); //�� 

		//�ɹ�����һ�Σ���������һ��
		//removeMyCount();
	}
	else //������ӵ�Ԫ�ظ���δ�ﵽҪ��
	{
		signClear(); //ȡ����Ԫ�صı��
		removeAllLine(); //ɾ��������
		if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT",true))
			SimpleAudioEngine::getInstance()->playEffect(WRONG_REMOVE_EFFECT); //������Ч
		if (UserDefault::getInstance()->getBoolForKey("IS_VIBRATE",true))
			Vibrator::vibrate(WRONG_REMOVE_VIBRATOR_TIME); //��
	}

	linkIndex.clear(); //�����������
	//log("removeLink()");
}

//��������ǵ�Ԫ�أ�������������
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
			removeCount[ele]++; //������ǵ�Ԫ��
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

//��Ⲣ�������Ա�������Ԫ�أ��ı�typeֵ
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
					removeElement(i, j); // �Ƴ�����
					//�޸���Ƭ����
					changeTiledType(i, j);

				}
			}
		}

	}
}
// �޸�ָ��λ������Ƭ������ֵ
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
//ɾ��������
void MapLayer::removeAllLine()
{
	while (removeLatestLine());  //����ɾ�������������һ�������ߣ�ֱ��Ϊ��
}

//����ָ��λ�õ�Ԫ�أ�����Ԫ������
int MapLayer::removeElement(int row,int line)
{
	ElementUnit* &temp = elements[row][line]; //��ʱ�������򻯴���
	if (!temp)
	{
		return -1; //ָ��λ����Ԫ�أ��򲻲���
	}

	int ele = temp->getElement(); //���Ԫ������
	//elementType = ele;//���Ԫ������
	elementCount[ele]--; //��ӦԪ�����ͼ�������
	temp->disappear(FALL_TIME);//Ԫ����С����ʧ
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

//��ȡ�ϴ�����������������������Ϊ�´�׼��
int MapLayer::getRemoveCount()
{
	int r = removeAllCount;
	removeAllCount = 0;
	return r;
}

//Ԫ�����䣬���������Ԫ�ء������Ƿ���˿�λ��ÿ�������һ����λ
bool MapLayer::elementsFall()
{
//#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//	LOGD("MapLayer::elementsFall()");
//#endif
	log("elementsFall()");
	bool flag = false; //�Ƿ���˿�λ
	//for (int i = 0; i < MATRIX_ROW; ++i)//��ÿһ�е������� 0
	//{
	//	for (int j = MATRIX_LINE; j >= 0; --j)//7
	//	{
	//		if (elements[i][j] == nullptr && checkIsDrop(i,j))
	//		{
	//			log("MapLayer::elementsFall() : i:%d,j:%d", i, j);
	//			//log("MapLayer::elementsFall:%i,%i",i,j);
	//			/*rowMyFall(i, j);*/
	//			rowFall(i, j);//�����·���ʼ��һ�����ֿ�λ�������Ϸ�Ԫ�����䣬���λ
	//			flag = true;//��˿�λ
	//			break;//ÿһ��ֻ�����һ����λ
	//		}
	//	}
	//}
	for (int j = MATRIX_LINE; j >= 0; --j)//5
	{
		for (int i = 0; i < MATRIX_ROW; ++i)//��ÿhang�������� 0
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

//����Ƿ��������
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

//�Ƿ���Ч�����ӽ���
bool MapLayer::isLinkFinish()
{
//#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//	LOGD(" MapLayer::isLinkFinish()");
//#endif
	return linkFinishFlag;
}

//׼����һ������
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
					elements[max(_row - 1, 0)][j]->moveToPosition(pop, FALL_TIME);//�Ϸ�Ԫ�ص���һ��
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
					elements[min(_row + 1, 5)][j]->moveToPosition(pop, FALL_TIME);//�Ϸ�Ԫ�ص���һ��
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
					elements[max(_row - 1, 0)][j]->moveToPosition(pop, FALL_TIME);//�Ϸ�Ԫ�ص���һ��
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
					elements[min(_row + 1, 5)][j]->moveToPosition(pop, FALL_TIME);//�Ϸ�Ԫ�ص���һ��
					elements[min(_row + 1, 5)][j] = nullptr;
				}
			}
		}else
			{
				elements[_row][bottom] = elements[_row][j]; //null
				if (elements[_row][bottom])
				{
					Point pop = getMCenterByCoord(_row, bottom);
					elements[_row][j]->moveToPosition(pop, FALL_TIME);//�Ϸ�Ԫ�ص���һ��
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

//ָ���е�ȫ����λ�Ϸ�Ԫ�����䣬���˳�����Ԫ��i.j
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
								elements[max(r - 1, 0)][j]->moveToPosition(pop, FALL_TIME);//�Ϸ�Ԫ�ص���һ��
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
									elements[min(r + 1, 5)][j]->moveToPosition(pop, FALL_TIME);//�Ϸ�Ԫ�ص���һ��
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
								elements[min(r + 1, 5)][j]->moveToPosition(pop, FALL_TIME);//�Ϸ�Ԫ�ص���һ��
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
									elements[max(r - 1, 0)][j]->moveToPosition(pop, FALL_TIME);//�Ϸ�Ԫ�ص���һ��
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
					//			elements[max(r - 1, 0)][j]->moveToPosition(pop, FALL_TIME);//�Ϸ�Ԫ�ص���һ��
					//			elements[max(r - 1, 0)][j] = nullptr;
					//		}
					//		else if (checkIsDrop(min(r + 1, 5), j))
					//		{
					//			isapp = false;
					//			elements[r][i] = elements[min(r + 1, 5)][j];
					//			if (elements[r][i])
					//			{
					//				Point pop = getMCenterByCoord(r, i);
					//				elements[min(r + 1, 5)][j]->moveToPosition(pop, FALL_TIME);//�Ϸ�Ԫ�ص���һ��
					//				elements[min(r + 1, 5)][j] = nullptr;
					//			}
					//		}
					//	}
					//}
					elements[r][i] = elements[r][j]; //null
					if (elements[r][i])
					{
						Point pop = getMCenterByCoord(r, i);
						elements[r][j]->moveToPosition(pop, FALL_TIME);//�Ϸ�Ԫ�ص���һ��
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

//��ʼ�����ӣ�ȷ���������򣬳�ʼ״̬����Ϊ��
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
	//��ʼ������
	for (int i = 0; i < 40; ++i) //0-5
	{
		elementCount[i] = 0;
		removeCount[i] = 0;
	}

	ERGODIC_MBLOCK(row,line)
	{
		blocksCenter[row][line] = getMCenterByCoord(row, line); //ȷ����������λ��
			/*	int  num = (uint32_t)typeLayer->getTileGIDAt(Vec2(i, j));*/
				//getType = _tileMap->getPropertiesForGID(num);
				//int _type = getType.asValueMap()["type"].asInt();
		if (checkIsNull(row, line))
		{
			unsigned int num;
			num = typeLayer->getTileGIDAt(Vec2(row, line));
			int _type = _tileMap->getPropertiesForGID(num).asValueMap()["type"].asInt();
			//�����Ƭ�ǲ���ʾ 
			auto s = ElementUnit::create();
			if (s)
			{
				s->setElementType(ElementType::cannotEliminate); // ����һ������Ԫ��
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
				s->setElementType(ElementType::basisEliminate); // ��������Ԫ��
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

// tile����ת����Ƭ�������ĵ�OpenGL����  //�����Ƭê�������
Vec2 MapLayer::positionForTileCoord(const Vec2& tileCoord) //0,0
{
	Size mapSize = _tileMap->getMapSize(); // w:8 h:10
	Size tileSize = _tileMap->getTileSize();//w:100 h:100
	int x = ((tileCoord.x )* tileSize.width*RATIO) + (mapPoint.x - mapSize.width / 2 * tileSize.width*RATIO); //0+(240-4*48)=48
	int y = ((mapSize.height - tileCoord.y - 1) * tileSize.height*RATIO) + (mapPoint.y - mapSize.height / 2 * tileSize.height*RATIO); // 9*48 + 160=592
	//log("positionForTileCoord:x:%i,y:%i",x,y);
	return Vec2(x, y); 
}

// tile����ת����Ƭ�������ĵ�OpenGL����  
Vec2 MapLayer::MypositionForTileCoord(const Vec2& tileCoord)
{
	Size mapSize = _tileMap->getMapSize();
	Size tileSize = _tileMap->getTileSize();
	int x = tileCoord.x * tileSize.width + tileSize.width / 2;
	int y = (mapSize.height - tileCoord.y)*tileSize.height - tileSize.height / 2;
	return Vec2(x, y);
}

//��ʼ�����
void MapLayer::initSign()
{
//#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//	LOGD("MapLayer::initSign()");
//#endif
	signClear();
}

//������б��
void  MapLayer::signClear()
{
	ERGODIC_MBLOCK(row,line)
	{
		unsignElement(row, line);
		unsignSpecialElement({ row, line });
	}
}

// ȡ�����Ԫ��
int MapLayer::unsignElement(int row,int line)
{
	//if (elements[row][line])
	//{
		signFlag[row][line] = false;
		return 0;  //���ָ��λ����Ԫ�أ��򲻲���
	//}
	//return -1;
}

//ȡ�����Ԫ��
int MapLayer::unsignElement(Coord c)
{
	return unsignElement(c.row, c.line);
}

//���Ԫ��
int MapLayer::signElement(int row, int line)
{
	if (elements[row][line])
	{
		signFlag[row][line] = true;
		return 0;
	}
	return -1;
}

//���Ԫ��
int MapLayer::signElement(Coord c)
{
	return signElement(c.row, c.line);//�������غ���
}

//�������Ԫ���漰��Ԫ��
int		MapLayer::signSpecialElement(Coord c)
{
	if (elements[c.row][c.line])
	{
		signFlagSpecial[c.row][c.line] = true;
		return 0;
	}
	return -1;
}

// ȡ������Ԫ���漰Ԫ�صı��
int		MapLayer::unsignSpecialElement(Coord c)
{
	//if (elements[c.row][c.line])
	//{
		signFlagSpecial[c.row][c.line] = false;
		return 0;
	//}
	//return -1;
}

bool MapLayer::onTouchBegan(Touch *touch, Event *unused_event) //��ʼ����
{
	if (isResponse())//���������
	{
		if (touchedFlag)
		{
			return false;//��������Ч����������Դ˴δ���
		}
		Point touchPoint = Director::getInstance()->convertToGL(touch->getLocationInView());//��ô�����

		ERGODIC_MBLOCK(row,line)
		{
			if (blocksCenter[row][line].getDistance(touchPoint) < containsDis)//����������ڸ�����Ч�뾶��
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
					touchedFlag = false;//��Ϊ��Ч����
					return false;
				}
				else
				{
					linkStart(row, line);//�ø�����Ϊ�������
					touchedFlag = true;//��Ϊ��Ч����
					//�Ӹ���Ч����
					showEffect(blocksCenter[row][line]);
					//ParticleSystem* m_emitter1 = ParticleSystemQuad::create("effect/particle_1.plist");
					//m_emitter1->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,elements[row][line],Vec2(0,0)));
					//elements[row][line]->addChild(m_emitter1);

					return true; //�Ըôδ����ĺ�������������ӳ
				}
			}
		}

	}

	return true;//������������δ�����κθ��ӣ���Ϊ��Ч���������Ժ�������
}
void MapLayer::onTouchMoved(Touch *touch, Event *unused_event) //�������ƶ�
{
	if (isResponse()) //���������
	{
		Point touchPoint = Director::getInstance()->convertToGL(touch->getLocationInView()); //��ô�����
		if (getIsBoom())
		{
			return;
		}
		if (!linkIndex.empty())
		{
			Coord latestPos = linkIndex.back(); //���ߵ����һ��Ԫ��
			int _line = latestPos.line;
			int _row = latestPos.row;
			if (blocksCenter[_row][_line].getDistance(touchPoint) < containsDis)
			{
				//log("return");
				return; //������������ͬһ��������򲻲���
			}

			if (linkIndex.size() > 1) //���Ѿ��γ����ߣ�������������Ԫ�����ӣ�
			{
				//log("linkIndex.size() > 1");
				Coord linkPos = *(linkIndex.end() - 2); //���ߵĵ����ڶ���Ԫ��
				if (blocksCenter[linkPos.row][linkPos.line].getDistance(touchPoint) < containsDis)
				{
					undoLink(); //��������ص���һ�����ӵ�Ԫ�أ��������һ������
					return;
				}
			}

			//�����������һ��Ԫ����Χ������Ԫ�� // 1,1
			for (int i = max(_row - 1, 0); i <= min(_row + 1, MATRIX_ROW - 1); i++) // 0,2
			{
				for (int j = max(_line - 1, 0); j <= min(_line + 1, MATRIX_MLINE - 1); j++) //0,2
				{
					if (blocksCenter[i][j].getDistance(touchPoint) < containsDis) //����������ڸ�����Ч�뾶��
					{
						if (elements[i][j])
						{
							if (!signFlag[i][j]) //�����Ԫ��δ�����ӹ�
							{
								if (checkLink(latestPos, { i, j })) //�������Ԫ�ط������ӵ���������ɫ��������ͬ��������һ��������Ԫ�أ�
								{
									linkElement(latestPos, { i, j }); //��������Ԫ�� �����˱��
									//��Ч
									showEffect(blocksCenter[i][j]);
									if (elements[i][j]->getElement() > 10)
									{
										elementType = elements[i][j]->getElement() % 10;//���Ԫ������
										log("onTouchMoved(Touch *touch, Event *unused_event) :elementType %d", elementType);
									}
									else
									{
										elementType = elements[i][j]->getElement();//���Ԫ������
										log("elementType��%d", elementType);
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
void MapLayer::onTouchEnded(Touch *touch, Event *unused_event) //��������
{
	if (isResponse())//���������
	{
		Point touchPoint = Director::getInstance()->convertToGL(touch->getLocationInView());

		if (getIsBoom())
		{
			ERGODIC_MBLOCK(row, line)
			{
				if (blocksCenter[row][line].getDistance(touchPoint) < containsDis)
				{ 
					//ը��������һ
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
			
			removeLink();//����һ���������жϷ����������������ߵ�Ԫ��
			while (removeEffect());

		}


	}

	touchedFlag = false;  //��������
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
		SimpleAudioEngine::getInstance()->playEffect(ELEMENT_LINE); //������Ч

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
		SimpleAudioEngine::getInstance()->playEffect(ELEMENT_ROW); //������Ч
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
		SimpleAudioEngine::getInstance()->playEffect(ELEMENT_BOOM); //������Ч
	ParticleSystem * efft = ParticleSystemQuad::create("effect/particle_6.plist");
	efft->setPosition(p);
	efft->setAutoRemoveOnFinish(true);
	efft->setDuration(0.1f);
	addChild(efft, 1);

}


//ը��ը����Ԫ��
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

//������Ԫ����Ҫ������Ԫ�ؼ��ϱ��
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
						signFlagSpecial[i][c.line] = true; // ���ϱ��
						if (eletype == 6)
						{
							changeTiledType(i,c.line); //�������Ǳ������ģ��޸���Ƭ����
						}
					}
					else
					{
						log("specialSignElement(elements[i][c.line]->getElement(), {i,c.line})");
						signFlagSpecial[i][c.line] = true; // ������Ԫ�أ����Լ����ϱ�ǣ��´μ���ʱ��Ͳ��ڼ���ˣ�����������ѭ��һֱ�ݹ���ȥ
						specialSignElement(elements[i][c.line]->getElement(), {i,c.line});//�����Լ����ݹ���÷���
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

//�������һ������
void MapLayer::undoLink()
{
	Coord latest = linkIndex.back();
	linkIndex.pop_back(); //����������е����һ��Ԫ��
	removeLatestLine(); //ɾ�����һ��������
	removeEffect(); // ɾ�����һ��Ԫ�ص���Ч
	unsignElement(latest); //ȡ�����һ��Ԫ�صı��
	if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT",true))
		SimpleAudioEngine::getInstance()->playEffect(UNDO_EFFECT); //������Ч
	if (UserDefault::getInstance()->getBoolForKey("IS_VIBRATE",true))
		Vibrator::vibrate(UNDO_VIBRATOR_TIME); //��
}

//ȡ����������������Ԫ���漰�����б�ʶ
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
				removeCount[ele]++; //������ǵ�Ԫ��
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


////���ƾ��θ�����
//void MapLayer::drawGuideLine(Point leftBottom, Point rightTop)
//{
//	DrawNode* b = DrawNode::create(); //������ˢ
//	addChild(b);
//
//	//ͨ���Ľǻ��ƾ��θ�����
//	Point leftTop = Point(leftBottom.x, rightTop.y);
//	Point rightBottom = Point(rightTop.x, leftBottom.y);
//	b->drawSegment(leftBottom, leftTop, GUIDELINE_WIDTH, GUIDELINE_COLOR);
//	b->drawSegment(rightBottom, rightTop, GUIDELINE_WIDTH, GUIDELINE_COLOR);
//	b->drawSegment(leftBottom, rightBottom, GUIDELINE_WIDTH, GUIDELINE_COLOR);
//	b->drawSegment(leftTop, rightTop, GUIDELINE_WIDTH, GUIDELINE_COLOR);
//}

//������������Ԫ�ص���
void MapLayer::drawLine(Coord from, Coord to)
{
	DrawNode* brush = DrawNode::create();
	addChild(brush, -1);
	brush->drawSegment(getMCenterByCoord(from), getMCenterByCoord(to), LINKLINE_WIDTH, LINKLINE_COLOR);
	linkBrush.push_back(brush); //������������ӵ���������
}

//ɾ�����һ�������ߣ������Ƿ�ɾ���ɹ�
bool MapLayer::removeLatestLine()
{
	if (linkBrush.empty())
	{
		return false;
	}
	DrawNode* brush = linkBrush.back();
	removeChild(brush);
	linkBrush.pop_back();//ɾ���������е����һ��
	return true;
}

//����Ƿ�����������Ԫ�أ���ͨ���޸ĸú�������ʵ�ֶ���������
bool MapLayer::checkLink(Coord from, Coord to)
{
	if (getElement(from) == getElement(to))  //Ԫ��������ͬ
	{
		return true;
	}
	int f = getElement(from); //10 // 0
	int t = getElement(to); //0  // 10
	//log("getElement(to): f:%d,t:%d", f, t);
	//����һ��������Ԫ��
	if (getElement(to) == (getElement(from) + 10) || getElement(to) == (getElement(from) + 20) || getElement(to) == (getElement(from) + 30))
	{
		return true;
	}
	if (getElement(to) == (getElement(from) - 10) || getElement(to) == (getElement(from) - 20) || getElement(to) == (getElement(from) - 30))
	{
		return true;
	}

	return false; //��������

}

//��ȡָ��λ��Ԫ������
int MapLayer::getElement(Coord c)
{
	if (elements[c.row][c.line])
	{
		return elements[c.row][c.line]->getElement();
	}
}

//ָ���еĶ��˳�����Ԫ��
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

//ͨ������Ԫ�����ͷֲ���������һ��Ԫ������
int MapLayer::randElement()
{
	//bool  wildFlag = randFlag(WILD_PROBABILITY - 1, 1); //��������Ԫ�صĸ�����1/WILD_PROBABILITY
	//if (wildFlag)
	//{
	//	return WILD_NUM; //�������������Ԫ��
	//}

	int *num = new int[ELEMENT_COUNT];//���ڴ�Ÿ���Ԫ�س��ֵĸ���

	//��ȡԪ�����������������Сֵ
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
		max++;//������Ԫ��������Ԫ�ز��û�й��������Ԫ�����п��ܳ���
	}
	for (int i = 0; i < ELEMENT_COUNT; ++i)
	{
		num[i] = max - elementCount[i];//����Ԫ����������ȷ�����ʣ��Ѵ�������Խ�࣬����ԽС
	}

	int re = randNum(ELEMENT_COUNT, num);//���ݸ���������һ�����
	delete[] num;
	return re;
}



//�������ߵ����
void MapLayer::linkStart(int row, int line)
{
	linkIndex.clear(); //�����������
	signOnlyBlock(row, line); //���ΨһԪ��
	linkIndex.push_back({ row, line });//��ӵ���������
	//��Ч����
	if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT",true))
	SimpleAudioEngine::getInstance()->playEffect(START_LINK_EFFECT); //������Ч
	if (UserDefault::getInstance()->getBoolForKey("IS_VIBRATE",true))
	Vibrator::vibrate(3); //��
}

//��������Ԫ��
void MapLayer::linkElement(Coord from, Coord to)
{
	///log("linkElement");
	signElement(to);  //���Ԫ��
	drawLine(from, to); //��������
	linkIndex.push_back(to); //��ӵ���������
	if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT",true))
		SimpleAudioEngine::getInstance()->playEffect(LINK_EFFECT); //������Ч
	if (UserDefault::getInstance()->getBoolForKey("IS_VIBRATE",true))
		Vibrator::vibrate(LINK_VIBRATOR_TIME); //��

}

//��վ���Ԫ��
void MapLayer::clearElement()
{
	//mei shi xian
	ERGODIC_MBLOCK(row,line)
	{
		removeElement(row, line); //���ÿ�����ӵ�Ԫ��
	}
	for (int i = 0; i < 40; ++i)
	{
		elementCount[i] = 0;
		//elementCount[i+10] = 0;
		//elementCount[i+20] = 0;
		//elementCount[i+30] = 0;
	}
}

//��ָ��λ�ô����������͵�Ԫ��
int  MapLayer::createElement(int element, int row, int line) //22
{
		ElementUnit* &temp = elements[row][line]; //������ʱ�������򻯴���
		temp = ElementUnit::create();
		elementCount[element]++; // ��ӦԪ�����ͻ�������
		if (temp)
		{
			temp->setElementType(ElementType::basisEliminate);//����Ϊ������������
			temp->createElement(element, getMCenterByCoord(row, line));//����һ��Ԫ�ز���ӵ���Ļ��
		}
		return element;
}

//ͨ�������ø�������
Point MapLayer::getMCenterByCoord(int row, int line)
{
	//Size mapSize = _tileMap->getMapSize();// ��ȡ��tiles����Ϊ��λ�ĵ�ͼ�ߴ�
	Size tileSize = _tileMap->getTileSize();// ��ȡ�����ص�Ϊ��λ��tile�ߴ����� // 100
	//log("size:%i,%i", tileSize.width, tileSize.height);

	Point c = positionForTileCoord(Vec2(row,line));
	//log("row:%i,line:%i",row,line);
	///log("getMCenterByCoord//:c.x:%f,c.y:%f", c.x, c.y);
	int x = c.x + (tileSize.width * RATIO)/ 2;
	int y = c.y + (tileSize.height *RATIO)/ 2;
	//log("getMCenterByCoord//: x:%i,y:%i", x, y);
	return Vec2(x, y);
}

//ͨ�������ø�������
Point  MapLayer::getMCenterByCoord(Coord c)
{
	return getMCenterByCoord(c.row, c.line);
}

//�����������
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

//ֻ���һ��Ԫ�أ�������Ǳ�ȡ����
void MapLayer::signOnlyBlock(int row, int line)
{
	signClear();
	signElement(row, line);
}

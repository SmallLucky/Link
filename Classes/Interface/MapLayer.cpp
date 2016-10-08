
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

	//����������Ŀռ�
	blocksCenter = new Point*[MATRIX_ROW];
	for (int i = 0; i < MATRIX_ROW; i++)
	{
		blocksCenter[i] = new Point[MATRIX_MLINE];
	}
	//�����Ԫ�������ռ�
	elementCount = new int[ELEMENT_COUNT + 1]; //ע������Ԫ�ؿռ�

	//����������Ԫ�ص������ռ�
	removeCount = new int[ELEMENT_COUNT + 1]; //ע������Ԫ�ؿռ�

	//linkBrush.clear();
}
MapLayer::~MapLayer()
{
	//���ո���Ԫ�ؿռ�
	for (int i = 0; i < MATRIX_ROW; i++)
		delete[] elements[i];
	delete[] elements;
	//���ձ�ǿռ�
	for (int i = 0; i < MATRIX_ROW; i++)
		delete[] signFlag[i];
	delete[] signFlag;
	//���ո������Ŀռ�
	for (int i = 0; i < MATRIX_ROW; i++)
		delete[] blocksCenter[i];
	delete[] blocksCenter;

	//���ո�Ԫ�������ռ�
	delete[] elementCount;

	//����������Ԫ�ص������ռ�
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

	initBlocks();//��ʼ�����ӣ�ȷ���������򣬳�ʼ״̬����Ϊ��
	initSign(); //��ʼ�����

	forbiddenResponse(); //��ʱ��ֹ�������ڽ���ȴ�����״̬��ͨ��allowResponse�������������̳���TouchableLayer��
	touchedFlag = false; //û����Ч�Ĵ���
	linkFinishFlag = false; //û�����ӽ���

	return true;
}

//�������������������ӵ�Ԫ�أ���������ʧ��
void MapLayer::removeLink()
{
	if (linkIndex.size() >= MIN_ROMOVE_COUNT) //������ӵ�Ԫ�ظ����ﵽҪ��
	{
		removeSignedElement(); //���������ӵ�Ԫ��
		removeAllLine(); //ɾ��������
		linkFinishFlag = true; //���һ������
		//if (UserDefault::getInstance()->getBoolForKey(EFFECT_UD, EFFECT_DEFAULT_FLAG))
		//	SimpleAudioEngine::getInstance()->playEffect(REMOVE_EFFECT); //������Ч
		//if (UserDefault::getInstance()->getBoolForKey(VIBRATE_UD, VIBRATE_DEFAULT_FLAG))
		//	Vibrator::vibrate(REMOVE_VIBRATOR_TIME); //��
		if (getIsLine())
		{
			removeSignedElement(); //���������ӵ�Ԫ��
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
		//�ɹ�����һ�Σ���������һ��
		//removeMyCount();
	}
	else //������ӵ�Ԫ�ظ���δ�ﵽҪ��
	{
		signClear(); //ȡ����Ԫ�صı��
		removeAllLine(); //ɾ��������
		//if (UserDefault::getInstance()->getBoolForKey(EFFECT_UD, EFFECT_DEFAULT_FLAG))
		//	SimpleAudioEngine::getInstance()->playEffect(WRONG_REMOVE_EFFECT); //������Ч
		//if (UserDefault::getInstance()->getBoolForKey(VIBRATE_UD, VIBRATE_DEFAULT_FLAG))
		//	Vibrator::vibrate(WRONG_REMOVE_VIBRATOR_TIME); //��
	}

	linkIndex.clear(); //�����������
}

//�������������������Ԫ�ء�
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
	}//�����Ҫ������Ԫ��

	removeSignedElement();

}

//��������ǵ�Ԫ�أ�������������
int  MapLayer::removeSignedElement()
{
	removeAllCount = 0;
	ERGODIC_MBLOCK(row,line)
	{
		if (signFlag[row][line])
		{
			if (getElement({ row, line }) >= 10)//ֻ��һ��
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
			removeCount[removeElement(row, line)]++; //������ǵ�Ԫ��
			removeAllCount++;
		}
	}
	//signClear();
	return removeAllCount;
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
	elementType = ele;//���Ԫ������
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
	log("m_count:%d", m_count);
	return ++m_count;
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
	bool flag = false; //�Ƿ���˿�λ
	for (int i = 0; i < MATRIX_ROW; ++i)//��ÿһ�е������� 0
	{
		for (int j = MATRIX_LINE; j >= 0; --j)//7
		{
			if (elements[i][j] == nullptr)//07
			{
				//log("MapLayer::elementsFall:%i,%i",i,j);
				rowFall(i, j);//�����·���ʼ��һ�����ֿ�λ�������Ϸ�Ԫ�����䣬���λ
				flag = true;//��˿�λ
				break;//ÿһ��ֻ�����һ����λ
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

//�Ƿ���Ч�����ӽ���
bool MapLayer::isLinkFinish()
{
	return linkFinishFlag;
}

//׼����һ������
void MapLayer::prepareLink()
{
	linkFinishFlag = false;
}

//ָ���е�ȫ����λ�Ϸ�Ԫ�����䣬���˳�����Ԫ��i.j
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
			elements[_row][i]->moveToPosition(pop, FALL_TIME);//�Ϸ�Ԫ�ص���һ��
		}
		i = j;
	}
	appear(_row);
} 

//��ʼ�����ӣ�ȷ���������򣬳�ʼ״̬����Ϊ��
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
		blocksCenter[row][line] = getMCenterByCoord(row, line); //ȷ����������λ��
			/*	int  num = (uint32_t)typeLayer->getTileGIDAt(Vec2(i, j));*/
				//getType = _tileMap->getPropertiesForGID(num);
				//int _type = getType.asValueMap()["type"].asInt();
		if (checkIsNull(row, line))
		{
			//�����Ƭ�ǲ���ʾ 
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

	//��ʼ������
	for (int i = 0; i < 6; ++i)
	{
		log("elementCount[i]:%i",i);
		elementCount[i] = 0;
		removeCount[i] = 0;
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
	signClear();
}

//������б��
void  MapLayer::signClear()
{
	ERGODIC_MBLOCK(row,line)
	{
		unsignElement(row, line);
	}
}

// ȡ�����Ԫ��
int MapLayer::unsignElement(int row,int line)
{
	if (!elements[row][line])
	{
		return -1;  //���ָ��λ����Ԫ�أ��򲻲���
	}
	signFlag[row][line] = false;
	return 0;
}

//ȡ�����Ԫ��
int MapLayer::unsignElement(Coord c)
{
	return unsignElement(c.row, c.line);
}

//���Ԫ��
int MapLayer::signElement(int row, int line)
{
	if (!elements[row][line])
	{
		return -1;
	}
	signFlag[row][line] = true;

	return 0;
}

//���Ԫ��
int MapLayer::signElement(Coord c)
{
	return signElement(c.row, c.line);//�������غ���
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
				log("element:%i", getElement({ row, line }));
				if (getElement({ row, line }) == 5)
				{
					log("wild");
					touchedFlag = false;//��Ϊ��Ч����
					return false;
				}
				else
				{
					linkStart(row, line);//�ø�����Ϊ�������
					touchedFlag = true;//��Ϊ��Ч����
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
		if (!linkIndex.empty())
		{
			Coord latestPos = linkIndex.back(); //���ߵ����һ��Ԫ��
			int _line = latestPos.line;
			int _row = latestPos.row;
			if (blocksCenter[_row][_line].getDistance(touchPoint) < containsDis)
			{
				log("return");
				return; //������������ͬһ��������򲻲���
			}

			if (linkIndex.size() > 1) //���Ѿ��γ����ߣ�������������Ԫ�����ӣ�
			{
				log("linkIndex.size() > 1");
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
						if (!signFlag[i][j]) //�����Ԫ��δ�����ӹ�
						{
							if (checkLink(latestPos, { i, j })) //�������Ԫ�ط������ӵ���������ɫ��������ͬ��������һ��������Ԫ�أ�
							{
								linkElement(latestPos, { i, j }); //��������Ԫ��
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

		removeLink();//����һ���������жϷ����������������ߵ�Ԫ��
		
	}
	touchedFlag = false;  //��������
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

//�������һ������
void MapLayer::undoLink()
{
	Coord latest = linkIndex.back();
	linkIndex.pop_back(); //����������е����һ��Ԫ��
	removeLatestLine(); //ɾ�����һ��������
	unsignElement(latest); //ȡ�����һ��Ԫ�صı��
}

//���ƾ��θ�����
void MapLayer::drawGuideLine(Point leftBottom, Point rightTop)
{
	DrawNode* b = DrawNode::create(); //������ˢ
	addChild(b);

	//ͨ���Ľǻ��ƾ��θ�����
	Point leftTop = Point(leftBottom.x, rightTop.y);
	Point rightBottom = Point(rightTop.x, leftBottom.y);
	b->drawSegment(leftBottom, leftTop, GUIDELINE_WIDTH, GUIDELINE_COLOR);
	b->drawSegment(rightBottom, rightTop, GUIDELINE_WIDTH, GUIDELINE_COLOR);
	b->drawSegment(leftBottom, rightBottom, GUIDELINE_WIDTH, GUIDELINE_COLOR);
	b->drawSegment(leftTop, rightTop, GUIDELINE_WIDTH, GUIDELINE_COLOR);
}

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
	int f = getElement(from);
	int t = getElement(to);
	log("getElement(to): f:%d,t:%d", f, t);
	//����һ��������Ԫ��
	if (getElement(to) == (getElement(from) + 10) || getElement(to) == (getElement(from) + 20) || getElement(to) == (getElement(from) + 30))
	{
		return true;
	}
	if (getElement(to) == (getElement(from) - 10) || getElement(to) == (getElement(from) - 20) || getElement(to) == (getElement(from) - 30))
		return true;

	return false; //��������

}

//��ȡָ��λ��Ԫ������
int MapLayer::getElement(Coord c)
{
	return elements[c.row][c.line]->getElement();
}

//ָ���еĶ��˳�����Ԫ��
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
	/*	if (UserDefault::getInstance()->getBoolForKey(EFFECT_UD, EFFECT_DEFAULT_FLAG))
	SimpleAudioEngine::getInstance()->playEffect(START_LINK_EFFECT); //������Ч
	if (UserDefault::getInstance()->getBoolForKey(VIBRATE_UD, VIBRATE_DEFAULT_FLAG))
	Vibrator::vibrate(START_LINK_VIBRATOR_TIME); //��*/
}

//��������Ԫ��
void MapLayer::linkElement(Coord from, Coord to)
{
	log("linkElement");
	signElement(to);  //���Ԫ��
	drawLine(from, to); //��������
	linkIndex.push_back(to); //��ӵ���������

}

//��վ���Ԫ��
void MapLayer::clearElement()
{
	//mei shi xian
	ERGODIC_MBLOCK(row,line)
	{
		removeElement(row, line); //���ÿ�����ӵ�Ԫ��
	}
	for (int i = 0; i < ELEMENT_COUNT; ++i)
	{
		elementCount[i] = 0;
	}
}

//��ָ��λ�ô����������͵�Ԫ��
int  MapLayer::createElement(int element, int row, int line)
{
		ElementUnit* &temp = elements[row][line]; //������ʱ�������򻯴���
		temp = ElementUnit::create();
		elementCount[element]++; // ��ӦԪ�����ͻ�������
		temp->createElement(element, getMCenterByCoord(row, line));//����һ��Ԫ�ز���ӵ���Ļ��

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
	for (int i = 0; i < ELEMENT_COUNT; ++i)
	{
		removeCount[i] = 0;
	}
}

//ֻ���һ��Ԫ�أ�������Ǳ�ȡ����
void MapLayer::signOnlyBlock(int row, int line)
{
	signClear();
	signElement(row, line);
}

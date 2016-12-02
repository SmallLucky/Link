
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


	signBeRemove = new bool *[MATRIX_ROW];
	for (int i = 0; i < MATRIX_ROW; i++)
	{
		signBeRemove[i] = new bool[MATRIX_MLINE];
	}

	////�����Ԫ�������ռ�
	//--elementCount = new int[40]; //ע������Ԫ�ؿռ�

	////����������Ԫ�ص������ռ�
	//--removeCount = new int[40]; //ע������Ԫ�ؿռ�
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

	CC_SAFE_DELETE_ARRAY(signBeRemove);
	//���ո�Ԫ�������ռ�

	//--CC_SAFE_DELETE_ARRAY(elementCount);

	/*delete[] elementCount;
	elementCount = nullptr;*/
	//--CC_SAFE_DELETE_ARRAY(removeCount);
	//����������Ԫ�ص������ռ�
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

	initBlocks();//��ʼ�����ӣ�ȷ���������򣬳�ʼ״̬����Ϊ��
	initSign(); //��ʼ�����
	initSignBeRemove(); //��ʼ�������

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
					s->createElement(abs(rand() % 5), blocksCenter[row][line]);//randElement()
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
		removeTargetECount = 0;
		removeSignedElement(); //���������ӵ�Ԫ��
		removeAllLine(); //ɾ��������	
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
		linkFinishFlag = true; //���һ������
		//if (UserDefault::getInstance()->getBoolForKey("IS_VIBRATE",true))
		//	Vibrator::vibrate(REMOVE_VIBRATOR_TIME); //�� 

		//�ɹ�����һ�Σ���������һ��
		//removeMyCount(); 
	}
	else //������ӵ�Ԫ�ظ���δ�ﵽҪ��
	{
		if (!temp_elementPos.empty())
		{
			undoChangeElement();
		}
		signClear(); //ȡ����Ԫ�صı��
		removeAllLine(); //ɾ��������
		if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
			AudioData::getInstance()->addOtherEffect(3);
		//if (UserDefault::getInstance()->getBoolForKey("IS_VIBRATE",true))
		//	Vibrator::vibrate(WRONG_REMOVE_VIBRATOR_TIME); //��
	}

	isRemoveFinish = true;

	linkIndex.clear(); //�����������
	m_effects.clear();
	linkBrush.clear();
}

//��������ǵ�Ԫ�أ�������������
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
				//--removeCount[ele]++; //������ǵ�Ԫ��
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
	clearSignBeRemove(); //����ͬһ�ε�����,������������
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

//��Ⲣ�������Ա�������Ԫ�أ��ı�typeֵ
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
	//				removeElement(i, j); // �Ƴ�����
	//				//�޸���Ƭ����
	//				changeTiledType(i, j);

	//			}
	//		}
	//	}

	//}
	//��������
	if (elements[r][max(l - 1, 0)] && !signFlag[r][max(l - 1, 0)])
	{
		if (elements[r][max(l - 1, 0)]->getElement() == 6) //����������	
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

			signBeRemove[r][max(l - 1, 0)] = true; //ͬһ�α��

		}
		else if (elements[r][max(l - 1, 0)]->getElement() == 7 && !signBeRemove[r][max(l - 1, 0)])
		{
			showCloudEffect(blocksCenter[r][max(l - 1, 0)]);
			removeElement(r, max(l - 1, 0)); // �Ƴ�����
			//�޸���Ƭ����
			changeTiledType(r, max(l - 1, 0));
		}
		else if (elements[r][max(l - 1, 0)]->getElement() == 8)
		{
			removeElement(r, max(l - 1, 0)); // �Ƴ�love
			changeTiledType(r, max(l - 1, 0));
			showLoveEffect(blocksCenter[r][max(l - 1, 0)]);
			GAMEDATA->setLoveNum(GAMEDATA->getLoveNum() + 1); //��һ
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
			removeElement(r, min(l + 1, 5)); // �Ƴ�����
			//�޸���Ƭ����
			changeTiledType(r, min(l + 1, 5));
		}
		else if (elements[r][min(l + 1, 5)]->getElement() == 8)
		{
			removeElement(r, min(l + 1, 5)); // �Ƴ�����
			changeTiledType(r, min(l + 1, 5));
			showLoveEffect(blocksCenter[r][min(l + 1, 5)]);
			GAMEDATA->setLoveNum(GAMEDATA->getLoveNum() + 1); //��һ
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
			removeElement(max(r - 1, 0), l); // �Ƴ�����
			//�޸���Ƭ����
			changeTiledType(max(r - 1, 0), l);
		}
		else if (elements[max(r - 1, 0)][l]->getElement() == 8)
		{
			removeElement(max(r - 1, 0), l); // �Ƴ�����
			changeTiledType(max(r - 1, 0), l);
			showLoveEffect(blocksCenter[max(r - 1, 0)][l]);
			GAMEDATA->setLoveNum(GAMEDATA->getLoveNum() + 1); //��һ
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
			removeElement(min(r + 1, 5), l); // �Ƴ�����
			//�޸���Ƭ����
			changeTiledType(min(r + 1, 5), l);
		}
		else if (elements[min(r + 1, 5)][l]->getElement() == 8)
		{
			removeElement(min(r + 1, 5), l); // �Ƴ�����
			changeTiledType(min(r + 1, 5), l);
			showLoveEffect(blocksCenter[min(r + 1, 5)][l]);
			GAMEDATA->setLoveNum(GAMEDATA->getLoveNum() + 1); //��һ
		}
	}
}

//��ʼ��ͬһ�α������
void	MapLayer:: initSignBeRemove()
{
	clearSignBeRemove();
}

//����������
void	MapLayer:: clearSignBeRemove()
{
	ERGODIC_MBLOCK(row, line)
	{
		signBeRemove[row][line] = false;
	}
}

// �޸�ָ��λ������Ƭ������ֵ
void	MapLayer::changeTiledType(int r, int l)
{
	unsigned int gidnum;
	for (int i = 0; i < 6; i++) //�ӵ�һ���ҵ��ʺ��滻����Ƭ����
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
//ɾ��������
void MapLayer::removeAllLine()
{
	while (removeLatestLine());  //����ɾ�������������һ�������ߣ�ֱ��Ϊ��
	while (removeEffect());
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

	showRemoveEffect(blocksCenter[row][line]);

	//showRemoveEleEffect(blocksCenter[row][line]);

	temp->disappear(FALL_TIME);//Ԫ����С����ʧFALL_TIME

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
	{//��ը���߲�������
		isCount = false;
		return m_count;
		
	}
	else
	{
			if (m_count == GAMEDATA->getCount(GAMEDATA->getCurLevel())-1)
			{
				//��������˾Ͳ�����������ڣ����Է���Ϣ��gamescene
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
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	LOGD("MapLayer::elementsFall()");
#endif
	//log("elementsFall()");
	bool flag = false; //�Ƿ���˿�λ
	if (isRemoveFinish)
	{
		for (int j = MATRIX_LINE; j >= 0; --j)//5
		{
			for (int i = 0; i < MATRIX_ROW; ++i)//��ÿhang�������� 0
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

//����Ƿ��������
bool	MapLayer::checkIsDrop(int r, int l) // ���Ŀո���
{
	//���������һ�������������Ƿ�����о���������
	if (elements[r][l-1]) //��you 
	{
		if (checkIsnullptr(r,max(l-1,0))) //��һ����ʯͷ
		{ //������
			if (elements[max(r - 1, 0)][max(l - 1, 0)]) //�����
			{
				if (checkIsnullptr(max(r - 1, 0), max(l - 1, 0))) //shitou //����Ƿ���ʯͷ ��
				{
					if (elements[min(r + 1, 5)][max(l - 1, 0)]) //�ұ���
					{
						if (checkIsnullptr(min(r + 1, 5), max(l - 1, 0))) //�����ʯͷ���ж��ұ�
						{
							//�����Ҷ���ʯͷ
							return false;
						}
						else //�ұ����Ҳ���ʯͷ
						{
							//����ʯͷ�����Ҳ���
							return true; //��ʾ����ͨԪ��
						}
					}
					else //�ұ��ǿգ��ж��ұ��Ƿ���һ����������õ�Ԫ�صĸ���
					{
						if (checkIsDrop(min(r + 1, 5), max(l - 1, 0))) //��������
						{
							return true;
						}
						else
						{
							return false;
						}
					}
				}
				else //���� �Ҳ���ʯͷ
				{
					return true;
				}
			}
			else //����Ϊ��
			{
				//�ж��ǲ���һ������ͨ������õ�Ԫ�صĸ���
				if (checkIsDrop(max(r - 1, 0), max(l - 1, 0))) //�����������ж�
				{
					return true;//��������ĸ���
				}
				else //��Ҫ�ж��ұ��ǲ��ǿ������� 
				{
					if (elements[min(r + 1, 5)][max(l - 1, 0)]) //�ұ���
					{
						if (checkIsnullptr(min(r + 1, 5), max(l - 1, 0))) //�����ʯͷ���ж��ұ�
						{
							//�����Ҷ���ʯͷ
							return false;
						}
						else //�ұ����Ҳ���ʯͷ
						{
							//����ʯͷ�����Ҳ���
							return true; //��ʾ����ͨԪ��
						}
					}
					else //�ұ��ǿգ��ж��ұ��Ƿ���һ����������õ�Ԫ�صĸ���
					{
						if (checkIsDrop(min(r + 1, 5), max(l - 1, 0))) //��������
						{
							return true;
						}
						else
						{//��ղ����ԣ���ʯͷ���ҿղ�����
							return false;
						}
					}
				}
			}
		}
		else //��һ�����Ҳ���ʯͷ
		{
			return true;
		}
	}
	else //��һ�������ǿ�
	{
		//�ж��ǲ���һ������ͨ������õ�Ԫ�صĸ��ӵ�������
		if (checkIsDrop(r,max(l-1,0)))
		{
			return true;
		}
		else  //��ʾ�Ͽգ��������ڲ���������ĸ��ӣ�������Ҫ�ж�����
		{
			if (elements[max(r - 1, 0)][max(l - 1, 0)]) //�����
			{
				if (checkIsnullptr(max(r - 1, 0), max(l - 1, 0))) //shitou //����Ƿ���ʯͷ ��
				{
					if (elements[min(r + 1, 5)][max(l - 1, 0)]) //�ұ���
					{
						if (checkIsnullptr(min(r + 1, 5), max(l - 1, 0))) //�����ʯͷ���ж��ұ�
						{
							//�����Ҷ���ʯͷ
							return false;
						}
						else //�ұ����Ҳ���ʯͷ
						{
							//����ʯͷ�����Ҳ���
							return true; //��ʾ����ͨԪ��
						}
					}
					else //�ұ��ǿգ��ж��ұ��Ƿ���һ����������õ�Ԫ�صĸ���
					{
						if (checkIsDrop(min(r + 1, 5), max(l - 1, 0))) //��������
						{
							return true;
						}
						else
						{
							return false;
						}
					}
				}
				else //���� �Ҳ���ʯͷ
				{
					return true;
				}
			}
			else //����Ϊ��
			{
				//�ж��ǲ���һ������ͨ������õ�Ԫ�صĸ���
				if (checkIsDrop(max(r - 1, 0), max(l - 1, 0))) //�����������ж�
				{
					return true;//��������ĸ���
				}
				else //��Ҫ�ж��ұ��ǲ��ǿ������� 
				{
					if (elements[min(r + 1, 5)][max(l - 1, 0)]) //�ұ���
					{
						if (checkIsnullptr(min(r + 1, 5), max(l - 1, 0))) //�����ʯͷ���ж��ұ�
						{
							//�����Ҷ���ʯͷ
							return false;
						}
						else //�ұ����Ҳ���ʯͷ
						{
							//����ʯͷ�����Ҳ���
							return true; //��ʾ����ͨԪ��
						}
					}
					else //�ұ��ǿգ��ж��ұ��Ƿ���һ����������õ�Ԫ�صĸ���
					{
						if (checkIsDrop(min(r + 1, 5), max(l - 1, 0))) //��������
						{
							return true;
						}
						else
						{//��ղ����ԣ��Ͽղ����ԣ��ҿղ�����
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

void	MapLayer::rowMyFall(int _row, int bottom) // 15 //ǰ����ж������������������������ģ���ô��ͨ��ѭ������������
{ //��������Ѱ�ҿ��Ե���ĸ��ӣ���lineΪ0��ʱ�򴴽�
	int	j = bottom - 1;
	if (j>=0) //�ո������ڵڶ�������
	{//����ƥ���ϣ���������
		if (elements[_row][j]) //����
		{
			if (checkIsnullptr(_row,j)) //��������Ԫ�ز������䷵�ص���true
			{ //������
				rightAndLeft(_row,bottom);
			}
			else //�����Ҳ���ʯͷ
			{
				elements[_row][bottom] = elements[_row][j];////��
				if (elements[_row][bottom])
				{
					Point pop = getMCenterByCoord(_row, bottom);
					elements[_row][j]->moveToPosition(pop, FALL_TIME);
					elements[_row][j] = nullptr;
				}
			}
		}
		else //�ϸ����ǿ� 
		{//�Ǿ��ж�������ǿ��Ե���Ŀո��Ӳ�
			if (checkIsDrop(_row,j)) //�ǿ��Ե����
			{
				elements[_row][bottom] = elements[_row][j]; ////��
				if (elements[_row][bottom])
				{
					Point pop = getMCenterByCoord(_row, bottom);
					elements[_row][j]->moveToPosition(pop, FALL_TIME);
					elements[_row][j] = nullptr;
				}
			}
			else //���ǿգ������ǲ����Ե���ĸ��ӣ���ô����Ҫȥ�����ߣ��ұ�
			{
				rightAndLeft(_row, bottom); //�ж�����
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
	if (elements[max(_row - 1, 0)][j]) //������
	{
		if (checkIsnullptr(max(_row - 1, 0), j)) //��ʯͷ
		{ //�Ǿ��ж��ұߣ���Ϊ�ǿ�������ģ�������һ��������������
			if (elements[min(_row + 1, 5)][j]) //������
			{
				if (checkIsnullptr(min(_row + 1, 5), j))
				{
					log("dou shi shi tou ,zou dao zhe bu ke xue!!!!");
				}
				//���������ǿ��Ե���Ļ���Ԫ��
				elements[_row][bottom] = elements[min(_row + 1, 5)][j];
				if (elements[_row][bottom])
				{
					Point pop = getMCenterByCoord(_row, bottom);
					elements[min(_row + 1, 5)][j]->moveToPosition(pop, FALL_TIME);//�Ϸ�Ԫ�ص���һ��
					elements[min(_row + 1, 5)][j] = nullptr;
				}
			}
			else //���� �ǿ�
			{
				//�Ǿ��ж�������ǿ��Ե���Ŀո��Ӳ�
				if (checkIsDrop(min(_row + 1, 5), j)) //�ǿ��Ե����
				{
					elements[_row][bottom] = elements[min(_row + 1, 5)][j];
					if (elements[_row][bottom])
					{
						Point pop = getMCenterByCoord(_row, bottom);
						elements[min(_row + 1, 5)][j]->moveToPosition(pop, FALL_TIME);//�Ϸ�Ԫ�ص���һ��
						elements[min(_row + 1, 5)][j] = nullptr;
					}
				}
				else
				{
					log("te me zou dao zhe ye shi bu ke xue de ba !!!!");
				}

			}
		}
		else//�������Ҳ���ʯͷ
		{//�Ǿ��ǻ���Ԫ���
			elements[_row][bottom] = elements[max(_row - 1, 0)][j]; ////����
			if (elements[_row][bottom])
			{
				Point pop = getMCenterByCoord(_row, bottom);
				elements[max(_row - 1, 0)][j]->moveToPosition(pop, FALL_TIME);
				elements[max(_row - 1, 0)][j] = nullptr;
			}
		}
	}
	else//�����ǿ�
	{
		//�Ǿ��ж�������ǿ��Ե���Ŀո��Ӳ�
		if (checkIsDrop(max(_row - 1, 0), j)) //�ǿ��Ե����
		{
			elements[_row][bottom] = elements[max(_row - 1, 0)][j]; ////����
			if (elements[_row][bottom])
			{
				Point pop = getMCenterByCoord(_row, bottom);
				elements[max(_row - 1, 0)][j]->moveToPosition(pop, FALL_TIME);
				elements[max(_row - 1, 0)][j] = nullptr;
			}
		}
		else //�ǿգ����ǲ����Ե��� /��ô��ζ�ţ�Ҫȥ�������
		{
			if (elements[min(_row + 1, 5)][j]) //you
			{
				if (checkIsnullptr(min(_row + 1, 5), j))
				{
					log("zhe ge zhen de bu ke xue!!!!");
				}
				else //���Ҳ���ʯͷ��������
				{
					elements[_row][bottom] = elements[min(_row + 1, 5)][j];
					if (elements[_row][bottom])
					{
						Point pop = getMCenterByCoord(_row, bottom);
						elements[min(_row + 1, 5)][j]->moveToPosition(pop, FALL_TIME);//�Ϸ�Ԫ�ص���һ��
						elements[min(_row + 1, 5)][j] = nullptr;
					}
				}
			}
			else //nullptr
			{
				if (checkIsDrop(min(_row + 1, 5), j))//ֱ�Ӽ���ұ��ǲ��ǿ��Ե��䣬�ǵĻ�ֱ�ӵ��� ***��ʵ��Ҫ����ǲ�����
				{
					elements[_row][bottom] = elements[min(_row + 1, 5)][j];
					if (elements[_row][bottom])
					{
						Point pop = getMCenterByCoord(_row, bottom);
						elements[min(_row + 1, 5)][j]->moveToPosition(pop, FALL_TIME);//�Ϸ�Ԫ�ص���һ��
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
							if (!checkIsnullptr(min(r + 1, 5), j))
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
					else if (!checkIsnullptr(min(r + 1, 5), j))
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
							if (!checkIsnullptr(max(r - 1, 0), j))
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
					elements[r][i] = elements[r][j]; //nullptr
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
	//��ʼ������
	//for (int i = 0; i < 40; ++i) //0-5
	//{
	//	//--elementCount[i] = 0;
	//	//--removeCount[i] = 0;
	//}

	ERGODIC_MBLOCK(row,line)
	{
		//log("ERGODIC_MBLOCK :row:%d,line:%d",row,line);
		blocksCenter[row][line] = getMCenterByCoord(row, line); //ȷ����������λ��
			/*	int  num = (uint32_t)typeLayer->getTileGIDAt(Vec2(i, j));*/
				//getType = _tileMap->getPropertiesForGID(num);
				//int _type = getType.asValueMap()["type"].asInt();
		if (checkIsnullptr(row, line))
		{
			unsigned int num;
			num = typeLayer->getTileGIDAt(Vec2(row, line));
			int _type = _tileMap->getPropertiesForGID(num).asValueMap()["type"].asInt();
			//�����Ƭ�ǲ���ʾ 
			auto s = ElementUnit::create();
			if (s)
			{
				//s->setElementType(ElementType::cannotEliminate); // ����һ������Ԫ��
				s->createElement(_type, getMCenterByCoord(row, line));
				elements[row][line] = s;
				//--elementCount[_type]++;
				addChild(elements[row][line], 1);
				//elements[row][line]->setVisible(false);
				if (_type == 8)
				{//����
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
				//s->setElementType(ElementType::basisEliminate); // ��������Ԫ��
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
		//log("refresh!"); //��֤�����ĵ�һ����ͼ���п������ߵĵ�ͼ
		refreshAllElement();
	}
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
				if ((getElement({ row, line }) >= 5 && getElement({ row, line }) < 10) || getElement({ row, line }) >= 30)
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
					showEffect(blocksCenter[row][line]);//blocksCenter[row][line]
					showQQEffect(elements[row][line]);
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
					if (elements[_row][_line])
					{
						if (elements[_row][_line]->getElement() >= 30) //�������Ǵ���30������Ԫ�أ�ȡ���任Ԫ��
						{
							undoChangeElement();
						}
					}
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
									showEffect(blocksCenter[i][j]);//blocksCenter[i][j]
									showQQEffect(elements[i][j]);
									//����Ԫ��30���Ͻ���Χ3*3��ɫ

									if (elements[i][j]->getElement() >= 10)
									{ 
										elementType = elements[i][j]->getElement() % 10;//���Ԫ������
										if (elements[i][j]->getElement() >= 30)
										{
											changeElement(elementType,i,j); // �޸ľ���
										}
									}
									else
									{
										elementType = elements[i][j]->getElement();//���Ԫ������
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
			removeLink();//����һ���������жϷ����������������ߵ�Ԫ��
			
			temp_element.clear();
			temp_elementPos.clear();
		}
	}

	touchedFlag = false;  //��������
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	LOGD("onTouchEnded");
#endif
	//if (UserDefault::getInstance()->getBoolForKey("IS_VIBRATE",true))
	//	Custom::Vibrator::cancelVibrate();
}

//�޸���Χ��������Ԫ��
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
				if (typeElement < 5 && typeElement != ele) //����Ԫ��
				{
					temp_element.push_back(typeElement); // ��˳���������
					temp_elementPos.push_back({i,j}); //��������
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
//�����޸ĵ�������Ԫ��
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
				signSpecialElement({ i, j }); //���Ϊ��������
				if (elements[i][j]->getElement() == 5 || elements[i][j]->getElement() == 6)
				{
					changeTiledType(i,j);
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
						signFlagSpecial[i][c.line] = true; // ���ϱ��
						if (eletype == 6)
						{
							changeTiledType(i,c.line); //�������Ǳ������ģ��޸���Ƭ����
						}
					}
					else
					{
						/*
						log("specialSignElement(elements[i][c.line]->getElement(), {i,c.line})");*/
						signFlagSpecial[i][c.line] = true; // ������Ԫ�أ����Լ����ϱ�ǣ��´μ���ʱ��Ͳ��ڼ���ˣ�����������ѭ��һֱ�ݹ���ȥ
						specialSignElement(elements[i][c.line]->getElement(), {i,c.line});//�����Լ����ݹ���÷���
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

//�������һ������
void MapLayer::undoLink()
{
	Coord latest = linkIndex.back();
	if (elements[latest.row][latest.line])
	{
		showQQEffect(elements[latest.row][latest.line]);

	}
	linkIndex.pop_back(); //����������е����һ��Ԫ��
	removeLatestLine(); //ɾ�����һ��������
	removeEffect(); // ɾ�����һ��Ԫ�ص���Ч
	unsignElement(latest); //ȡ�����һ��Ԫ�صı��
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
	//	Custom::Vibrator::vibrate(UNDO_VIBRATOR_TIME); //��
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
				//--removeCount[ele]++; //������ǵ�Ԫ��
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
	auto line = DrawLine::create(getMCenterByCoord(from), getMCenterByCoord(to), "effect/link_0.png");
	addChild(line,-1);

	linkBrush.push_back(line); //������������ӵ���������
}

//ɾ�����һ�������ߣ������Ƿ�ɾ���ɹ�
bool MapLayer::removeLatestLine()
{
	if (linkBrush.empty())
	{
		return false;
	}
	DrawLine* brush = linkBrush.back();
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

//ͨ������Ԫ�����ͷֲ���������һ��Ԫ������
int MapLayer::randElement()
{
	//bool  wildFlag = randFlag(WILD_PROBABILITY - 1, 1); //��������Ԫ�صĸ�����1/WILD_PROBABILITY
	//if (wildFlag)
	//{
	//	return WILD_NUM; //�������������Ԫ��
	//}

	//int *num = new int[ELEMENT_COUNT];//���ڴ�Ÿ���Ԫ�س��ֵĸ���

	////��ȡԪ�����������������Сֵ
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
	//	max++;//������Ԫ��������Ԫ�ز��û�й��������Ԫ�����п��ܳ���
	//}
	//for (int i = 0; i < ELEMENT_COUNT; ++i)
	//{
	//	num[i] = max - elementCount[i];//����Ԫ����������ȷ�����ʣ��Ѵ�������Խ�࣬����ԽС
	//}

	//int re = randNum(ELEMENT_COUNT, num);//���ݸ���������һ�����
	//delete[] num;
	//return re;
	return 1;
}



//�������ߵ����
void MapLayer::linkStart(int row, int line)
{
	linkIndex.clear(); //�����������
	signOnlyBlock(row, line); //���ΨһԪ��
	linkIndex.push_back({ row, line });//��ӵ���������
	//��Ч����
	if (UserDefault::getInstance()->getBoolForKey("IS_EFFECT", true))
		AudioData::getInstance()->addLinkEffect(1);
	//if (UserDefault::getInstance()->getBoolForKey("IS_VIBRATE",true))
	//	Custom::Vibrator::vibrate(3); //��
}

//��������Ԫ��
void MapLayer::linkElement(Coord from, Coord to)
{
	signElement(to);  //���Ԫ��
	drawLine(from, to); //��������
	linkIndex.push_back(to); //��ӵ���������
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
	//	Custom::Vibrator::vibrate(LINK_VIBRATOR_TIME); //��
}

//��վ���Ԫ��
void MapLayer::clearElement()
{
	//mei shi xian
	ERGODIC_MBLOCK(row,line)
	{
		if (elements[row][line])
		{
			removeElement(row, line); //���ÿ�����ӵ�Ԫ��
		}	
	}
	//for (int i = 0; i < 40; ++i)
	//{
	//	//--elementCount[i] = 0;
	//}
}

//��ָ��λ�ô����������͵�Ԫ��
int  MapLayer::createElement(int element, int row, int line) //22
{
		//ElementUnit* &temp = elements[row][line]; //������ʱ�������򻯴���
		elements[row][line] = ElementUnit::create();
		//--elementCount[element]++; // ��ӦԪ�����ͻ�������
		if (elements[row][line])
		{
			//elements[row][line]->setElementType(ElementType::basisEliminate);//����Ϊ������������
			elements[row][line]->createElement(element, getMCenterByCoord(row, line));//����һ��Ԫ�ز���ӵ���Ļ��
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
	for (int i = 0; i < 40; ++i)
	{
		//--removeCount[i] = 0;
	}
}

//ֻ���һ��Ԫ�أ�������Ǳ�ȡ����
void MapLayer::signOnlyBlock(int row, int line)
{
	signClear();
	signElement(row, line);
}

//��Ч
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
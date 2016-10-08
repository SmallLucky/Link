
#include "MatrixTLayer.h"
#include "Data/Data.h"

MatrixTLayer::MatrixTLayer()
{
	//分配格子元素空间
	elements = new ElementUnit* *[MATRIX_LINE];
	for (int i = 0; i < MATRIX_LINE; i++)
	{
		elements[i] = new ElementUnit*[MATRIX_ROW];
	}

	//分配格子中心空间
	blocksCenter = new Point*[MATRIX_LINE];
	for (int i = 0; i < MATRIX_LINE; i++)
	{
		blocksCenter[i] = new Point[MATRIX_ROW];
	}
	//分配标记空间
	signFlag = new bool*[MATRIX_LINE];
	for (int i = 0; i < MATRIX_LINE; i++)
	{
		signFlag[i] = new bool[MATRIX_ROW];
	}
	//分配各元素数量空间
	elementCount = new int[ELEMENT_COUNT + 1]; //注意万能元素空间

	//分配消除个元素的数量空间
	removeCount = new int[ELEMENT_COUNT + 1]; //注意万能元素空间

}
MatrixTLayer ::~MatrixTLayer()
{
	//回收格子元素空间
	for (int i = 0; i < MATRIX_LINE; i++)
		delete[] elements[i];
	delete[] elements;

	//回收格子中心空间
	for (int i = 0; i < MATRIX_LINE; i++)
		delete[] blocksCenter[i];
	delete[] blocksCenter;

	//回收标记空间
	for (int i = 0; i < MATRIX_LINE; i++)
		delete[] signFlag[i];
	delete[] signFlag;

	//回收各元素数量空间
	delete[] elementCount;

	//回收消除各元素的数量空间
	delete[] removeCount;
}

bool MatrixTLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	initBlocks(); //初始化矩阵，初始为空矩阵，通过elementsFall函数填充矩阵
	initSign(); //初始化标记

	drawGuideLine(MATRIX_BOTTOM_LEFT, MATRIX_TOP_RIGHT); //绘制矩阵区域外围的辅助线
	forbiddenResponse(); //暂时禁止触摸，在进入等待连接状态后通过allowResponse函数允许触摸（继承自TouchableLayer）

	touchedFlag = false; //没有有效的触摸
	linkFinishFlag = false; //没有连接结束
	return true;
}

//符合条件则消除被连接的元素，否则连接失败
void MatrixTLayer::removeLink()
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

int MatrixTLayer::removeMyCount()
{
	log("m_count:%d", m_count);
	return ++m_count;
}

//获取上次连接中消除个数，并清零为下次准备
int MatrixTLayer::getRemoveCount()
{
	int r = removeAllCount;
	removeAllCount = 0;
	return r;
}

//元素下落，填补被消除的元素。返回是否填补了空位，每列填补最下一个空位
bool MatrixTLayer::elementsFall()
{
	bool flag = false; //是否填补了空位
	for (int j = 0; j < MATRIX_ROW; ++j)//对每一列单独处理
	{
		for (int i = 0; i < MATRIX_LINE; ++i)
		{
			if (elements[i][j] == NULL)//1,0
			{
				rowFall(j,i);//从最下方开始，一旦出现空位，所有上方元素下落，填补空位
				flag = true;//填补了空位
				break;//每一列只填补最下一个空位
			}
		}
	}
	return flag;
}

//是否有效的连接结束
bool MatrixTLayer::isLinkFinish()
{
	return linkFinishFlag;
}

//准备下一次连接
void MatrixTLayer::prepareLink()
{
	linkFinishFlag = false;
}


bool MatrixTLayer::onTouchBegan(Touch *touch, Event *unused_event) //开始触控接口
{
	if (isResponse())//如果允许触摸
	{
		if (touchedFlag)
		{
			return false;//若已有有效触摸，则忽略此次触摸
		}
		Point touchPoint = Director::getInstance()->convertToGL(touch->getLocationInView());//获得触摸点

		ERGODIC_BLOCK(i, j)
		{
			if (blocksCenter[i][j].getDistance(touchPoint) < containsDis)//如果触摸点在格子有效半径内
			{
				linkStart(i,j);//该格子作为连线起点
				touchedFlag = true;//成为有效触摸
				return true; //对该次触摸的后续操作做出反映
			}  
		}
	}

	return true;//若不允许触摸或未触摸任何格子，则为无效触摸，忽略后续操作
}
void MatrixTLayer::onTouchMoved(Touch *touch, Event *unused_event)//触点移动接口
{
	
	if (isResponse()) //如果允许触摸
	{
		Point touchPoint = Director::getInstance()->convertToGL(touch->getLocationInView()); //获得触摸点

		Coord latestPos = linkIndex.back(); //连线的最后一个元素
		int line = latestPos.line;
		int row = latestPos.row;
		if (blocksCenter[line][row].getDistance(touchPoint) < containsDis)
		{
			return; //若触摸点仍在同一个格子里，则不操作
		}

		if (linkIndex.size() > 1) //若已经形成连线（有两个或以上元素连接）
		{
			Coord linkPos = *(linkIndex.end() - 2); //连线的倒数第二个元素
			if (blocksCenter[linkPos.line][linkPos.row].getDistance(touchPoint) < containsDis)
			{
				undoLink(); //若触摸点回到上一个连接的元素，则撤销最后一次连接
				return;
			}
		}

		//遍历连线最后一个元素周围的所有元素
		for (int i = max(line - 1, 0); i <= min(line + 1, MATRIX_LINE - 1); ++i)
		{
			for (int j = max(row - 1, 0); j <= min(row + 1, MATRIX_ROW - 1); ++j)
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

void MatrixTLayer::onTouchEnded(Touch *touch, Event *unused_event) //触控结束接口
{
	if (isResponse())//如果允许触摸
	{
		Point touchPoint = Director::getInstance()->convertToGL(touch->getLocationInView());

		removeLink();//进行一次消除，判断符合条件后消除连线的元素
	}
	touchedFlag = false;  //触摸结束
}




//绘制矩形辅助线
void MatrixTLayer::drawGuideLine(Point leftBottom, Point rightTop)
{
	DrawNode* b = DrawNode::create(); //创建画刷
	addChild(b);

	//通过四角绘制矩形辅助线
	Point leftTop = Point(leftBottom.x, rightTop.y);
	Point rightBottom = Point(rightTop.x,leftBottom.y);
	b->drawSegment(leftBottom,leftTop,GUIDELINE_WIDTH,GUIDELINE_COLOR);
	b->drawSegment(rightBottom, rightTop, GUIDELINE_WIDTH, GUIDELINE_COLOR);
	b->drawSegment(leftBottom,rightBottom,GUIDELINE_WIDTH,GUIDELINE_COLOR);
	b->drawSegment(leftTop, rightTop, GUIDELINE_WIDTH, GUIDELINE_COLOR);
}

//绘制连接两个元素的线
void MatrixTLayer::drawLine(Coord from, Coord to)
{
	DrawNode* brush = DrawNode::create();
	addChild(brush,-1);
	brush->drawSegment(getCenterByCoord(from), getCenterByCoord(to), LINKLINE_WIDTH,LINKLINE_COLOR);
	linkBrush.push_back(brush); //将该连接线添加到线序列中
} 

//删除最后一条连接线，返回是否删除成功
bool MatrixTLayer::removeLatestLine()
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

//删除所有线
void MatrixTLayer::removeAllLine()
{
	while (removeLatestLine());  //不断删除线序列中最后一条连接线，直至为空

}

//检查是否能连接两个元素，，通过修改该函数即可实现多样化规则
bool MatrixTLayer::checkLink(Coord from, Coord to)
{
	if (getElement(from) == getElement(to))  //元素类型相同
	{
		return true;
	}

	//其中一个是万能元素
	if (getElement(from) == WILD_NUM)
	{
		return true;
	}
	if (getElement(to) == WILD_NUM)
		return true;

	return false; //不能连接

}

//设置连线的起点
void MatrixTLayer::linkStart(int line, int row)
{
	linkIndex.clear(); //清空连线序列
	signOnlyBlock(line,row); //标记唯一元素
	linkIndex.push_back({ line, row });//添加到连线序列
	//音效，震动
	/*	if (UserDefault::getInstance()->getBoolForKey(EFFECT_UD, EFFECT_DEFAULT_FLAG))
		SimpleAudioEngine::getInstance()->playEffect(START_LINK_EFFECT); //播放音效
	if (UserDefault::getInstance()->getBoolForKey(VIBRATE_UD, VIBRATE_DEFAULT_FLAG))
		Vibrator::vibrate(START_LINK_VIBRATOR_TIME); //震动*/
}

//连接两个元素
void MatrixTLayer::linkElement(Coord from, Coord to)
{
	signElement(to);  //标记元素
	drawLine(from,to); //绘制连线
	linkIndex.push_back(to); //添加到连线序列

}

//撤销最后一次连接
void MatrixTLayer::undoLink()
{
	Coord latest = linkIndex.back();
	linkIndex.pop_back(); //清除连线序列的最后一个元素
	removeLatestLine(); //删除最后一条连接线
	unsignElement(latest); //取消最后一个元素的标记
}


//初始化格子，确定格子区域，初始状态矩阵为空
void MatrixTLayer::initBlocks()
{
	//mei shi xian
	containsDis = (BLOCK_WIDTH + BLOCK_HEIGHT) / 4 * TOUCH_RADIUS;
	ERGODIC_BLOCK(i, j)
	{
		blocksCenter[i][j] = getCenterByCoord(i, j); //确定格子中心位置
		elements[i][j] = NULL;
	}

	//初始化数量
	for (int i = 0; i < ELEMENT_COUNT; ++i)
	{
		elementCount[i] = 0;
		removeCount[i] = 0;
	}
	removeAllCount = 0;
}

//清空矩阵元素
void MatrixTLayer::clearElement()
{
	//mei shi xian
	ERGODIC_BLOCK(i, j)
	{
		removeElement(i, j); //清除每个格子的元素
	}
	for (int i = 0; i < ELEMENT_COUNT; ++i)
	{
		elementCount[i] = 0;
	}
}


//在指定位置创建给定类型的元素
int MatrixTLayer::createElement(int element,int line, int row)
{
	ElementUnit* &temp = elements[line][row]; //定义临时变量，简化代码
	temp = ElementUnit::create();
	elementCount[element]++; // 对应元素类型基数自增
	temp->createElement(element, getCenterByCoord(line, row));//创建一个元素并添加到屏幕上

	return element;
}

//消除指定位置的元素，返回元素类型
int MatrixTLayer::removeElement(int line, int row)
{
	ElementUnit* &temp = elements[line][row]; //临时变量。简化代码
	if (!temp)
	{
		return -1; //指定位置五元素，则不操作
	}

	int ele = temp->getElement(); //获得元素类型
	elementCount[ele]--; //对应元素类型计数自增
	temp->disappear(FALL_TIME);//元素缩小至消失
	temp = NULL;
	signFlag[line][row] = false;
	return ele;

}

//消除被标记的元素，翻消除个数
int MatrixTLayer::removeSignedElement()
{
	removeAllCount = 0;
	ERGODIC_BLOCK(i,j)
	{
		if (signFlag[i][j])
		{
			removeCount[removeElement(i, j)]++; //消除标记的元素
			removeAllCount++;
		}
	}
	signClear();
	return removeAllCount;
}

//获取指定位置的元素类型
int MatrixTLayer::getElement(int line, int row)
{
	return elements[line][row]->getElement();
}

//获取指定位置元素类型
int MatrixTLayer::getElement(Coord c)
{
	return elements[c.line][c.row]->getElement();
}

//指定列的全部空位上方元素下落，顶端出现新元素
void MatrixTLayer::rowFall(int row, int bottom)//0,1
{
	for (int i = bottom; i < MATRIX_LINE - 1; ++i)
	{
		elements[i][row] = elements[i + 1][row];//10203040506070
		if (elements[i][row])//70
		{
			elements[i][row]->moveToPosition(getCenterByCoord(i, row), FALL_TIME);//上方元素掉落一个
		}
	}
	appear(row);
}

//指定列的顶端出现新元素
void MatrixTLayer::appear(int row)
{
	int top = MATRIX_LINE - 1;
	int ele = randElement();
	createElement(ele,top,row);
	this->addChild(elements[top][row]);
	elements[top][row]->appear(FALL_TIME);
}

//通过现有元素类型分布，随机获得一个元素类型
int MatrixTLayer::randElement()
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

//初始化标记
void MatrixTLayer::initSign()
{
	signClear();
}

//标记元素
int MatrixTLayer::signElement(int line, int row)
{
	if (!elements[line][row])
	{
		return -1;
	}
	signFlag[line][row] = true;

	return 0;
}

//标记元素
int MatrixTLayer::signElement(Coord c)
{
	return signElement(c.line, c.row);//调用重载函数
}

// 取消标记元素
int MatrixTLayer::unsignElement(int line, int row)
{
	if (!elements[line][row])
	{
		return -1;  //如果指定位置无元素，则不操作
	}
	signFlag[line][row] = false;
	return 0;
}

//取消标记元素
int MatrixTLayer::unsignElement(Coord c)
{
	return unsignElement(c.line,c.row);
}

//清除所有标记
void MatrixTLayer::signClear()
{
	ERGODIC_BLOCK(i, j)
	{
		unsignElement(i,j);
	}
}


//清空消除个数
void MatrixTLayer::removeCountCleaar()
{
	for (int i = 0; i < ELEMENT_COUNT; ++i)
	{
		removeCount[i] = 0;
	}
}

//只标记一个元素（其他标记被取消）
void MatrixTLayer::signOnlyBlock(int line, int row)
{
	signClear();
	signElement(line,row);
}

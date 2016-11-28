#pragma once


#include "cocos2d.h"
#include <iostream>
#include "ui/UIText.h"
#include "ui/UIButton.h"

using namespace std;
using namespace cocos2d;
using namespace ui;

class ShopItem :public Node
{
public:
	ShopItem();
	~ShopItem();
	//CREATE_FUNC(ShopItem);
	//第一个参数是创建item的类型
	static ShopItem*createShopItem(int type, int moneyNum, int num);
	virtual bool init(int type, int moneyNum, int num);

	void addRefreshPropsCallBarck( int num); //道具按钮旁边添加道具回调方法
	void addBoomPropsCallBarck(int num);
private:
	void btnClickEvent(Ref* pSender);

};

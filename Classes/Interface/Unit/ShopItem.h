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
	static ShopItem*createShopItem(int type, int moneyNum); 
	virtual bool init(int type, int moneyNum);

private:
	void btnClickEvent(Ref* pSender);

};

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
	static ShopItem*createShopItem(int diamNum, int giveNum, int moneyNum);
	virtual bool init(int diamNum, int giveNum, int moneyNum);

private:
	void btnClickEvent(Ref* pSender);

};

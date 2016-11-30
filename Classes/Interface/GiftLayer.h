#pragma once

#include "Global.h"
#include "LayerEffect/LayerEffect.h"

class GiftLayer : public TopDownLayer
{
public:
	GiftLayer();
	~GiftLayer();
	bool init();
	CREATE_FUNC(GiftLayer);

	void addUI();
	void backButCallBack(Ref* sender);

};

class GiftItem : public Node
{
public:
	GiftItem();
	~GiftItem();
	static GiftItem*createItem(int changeNum, int bombNum, float moneyNum);
	virtual bool init(int changeNum, int bombNum, float moneyNum);
	void addUI(int changeNum, int bombNum, float moneyNum);
private:
	void btnClickEvent(Ref* pSender);
};

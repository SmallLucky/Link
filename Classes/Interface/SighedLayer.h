#pragma once

#include "Global.h"
#include "LayerEffect/LayerEffect.h"
#include "TTButton.h"

class SignedLayer : public TopDownLayer
{
public:
	SignedLayer();
	~SignedLayer();
	bool init();
	CREATE_FUNC(SignedLayer);

	void addUI();
	void backButCallBack(Ref* sender);
	void signedButCallBack(Ref* sender);
	void rewardsUI(int days);

private:
	TTButton* sighedOk_btn;
	Sprite* re_bg;
};

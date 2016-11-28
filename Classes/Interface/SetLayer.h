#pragma once

#include "Global.h"

#include "TouchSwallowLayer.h"
#include "LayerEffect/LayerEffect.h"

class SetLayer : public EasePop
{
public:
	bool init();
	CREATE_FUNC(SetLayer);

	void	addUI();
	void	musicCBoxCallBack(Ref* pSender, cocos2d::ui::CheckBox::EventType  type);
	void	soundCBoxCallBack(Ref* pSender, cocos2d::ui::CheckBox::EventType  type);
	void	vibrationCBoxCallBack(Ref* pSender, cocos2d::ui::CheckBox::EventType  type);
	void	backButtonCallBack();

private:

	TouchSwallowLayer* _swallowLayer;

};
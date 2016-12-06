
#include "BackgroundLayer.h"
#include "Data/Data.h"
#include "CommonFunction.h"

bool BackgroundLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	Size visiblesize = Director::getInstance()->getVisibleSize();
	addSprite(backgroun, "bg/game_bg.png", Vec2(visiblesize.width/2,visiblesize.height/2), BACKGROUND_SIZE, this); //½«±³¾°Í¼Æ¬Ìí¼Óµ½ÆÁÄ»

	return true;
}
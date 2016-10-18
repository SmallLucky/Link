#pragma  once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
//using namespace ui;
using namespace cocos2d;

class TTButton : public Layer
{
	
public:
	bool init();
	CREATE_FUNC(TTButton);
	void callFunc();

	TMXLayer* mapLayer;
	TMXLayer* typeLayer;
	TMXTiledMap* _tiled;
};
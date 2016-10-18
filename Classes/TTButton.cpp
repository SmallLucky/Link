
#include "TTButton.h"
#include "CommonFunction.h"
using namespace ui;
bool TTButton::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto l = LayerColor::create();
	if (l)
	{
		l->setColor(Color3B::BLACK);
		l->setPosition(CommonFunction::getVisibleAchor(Anchor::Center, Vec2(0, 0)));
		addChild(l);
	}


	 _tiled =  TMXTiledMap::create("tiledmap/map_15.tmx");
	_tiled->setAnchorPoint(Vec2(0.5, 0.5));
	_tiled->setPosition(CommonFunction::getVisibleAchor(Anchor::Center,Vec2(0,0)));
	addChild(_tiled, 0);

	mapLayer = _tiled->getLayer("map");
	typeLayer = _tiled->getLayer("type");
	//typeLayer->setVisible();

	Button * but = Button::create("button/set.png");
	if (but)
	{
		but->setPosition(CommonFunction::getVisibleAchor(Anchor::MidButtom, _tiled,Vec2(0, -100)));
		_tiled->addChild(but);
		but->addClickEventListener(CC_CALLBACK_0(TTButton::callFunc, this));
	}
	return true;
}

void  TTButton::callFunc()
{
	unsigned int gidnum = typeLayer->getTileGIDAt(Vec2(0,0));

	unsigned int num1 = typeLayer->getTileGIDAt(Vec2(0, 5));
	Value typevalue1 = _tiled->getPropertiesForGID(num1);
	int type1 = typevalue1.asValueMap()["type"].asInt();
	log("(0,5)type1:%d", type1);

	typeLayer->setTileGID(gidnum,Vec2(0,5));

	unsigned int num = typeLayer->getTileGIDAt(Vec2(0,5));
	Value typevalue = _tiled->getPropertiesForGID(num);
	int type = typevalue.asValueMap()["type"].asInt();
	log("(0,5)type:%d",type);
}
#pragma once

#include "cocos2d.h"

using namespace cocos2d;

class DrawLight : public DrawNode
{
public:
	DrawLight();
	~DrawLight();

	CREATE_FUNC(DrawLight);
	bool init();
	//static DrawLight* createDrawLight(float x1, float y1, float x2, float y2, float displace);

	//virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags);
	void drawLighting(float x1, float y1, float x2, float y2, float displace);
private:
	float curDetail;
	//Point pos1;
	//Point pos2;
};


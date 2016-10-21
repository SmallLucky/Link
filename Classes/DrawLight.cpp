#include "DrawLight.h"


DrawLight::DrawLight()
{
}

DrawLight::~DrawLight()
{
}
bool DrawLight::init()
{
	if (!DrawNode::init())
	{
		return false;
	}
	curDetail = 5;
	ccDrawColor4B(255, 0, 0, 0);
	glLineWidth(5);
	return true;
}
//DrawLight* DrawLight::createDrawLight(float x1, float y1, float x2, float y2, float displace)
//{
//
//}
//void DrawLight::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags)
//{
//
//	//drawLighting(pos1.x, pos1.y, pos2.x, pos2.y, 20);  //这里多画几条线就可以看到更多了   
//}

void DrawLight::drawLighting(float x1, float y1, float x2, float y2, float displace)
{
	if (displace < curDetail)
	{
		ccDrawLine(ccp(x1, y1), ccp(x2, y2));
	}
	else
	{
		float mid_x = (x2 + x1) / 2;
		float mid_y = (y2 + y1) / 2;
		mid_x += (CCRANDOM_0_1() - 0.5) * displace;
		mid_y += (CCRANDOM_0_1() - 0.5) * displace;
		drawLighting(x1, y1, mid_x, mid_y, displace / 2);
		drawLighting(x2, y2, mid_x, mid_y, displace / 2);

	}
}
#pragma once
#include "cocos2d.h"
USING_NS_CC;

class DrawLine : public Node{

public:
	DrawLine();
	~DrawLine();

	bool init(Vec2 from, Vec2 to,  std::string img);
	static DrawLine* create(Vec2 from, Vec2 to,  std::string img);
	void addLine(Vec2 from, Vec2 to,  std::string img);
	float getAngle( Vec2 beginPoint,  Vec2 endPoint);
	void cleanLine();
private:
	//DrawNode* m_drawNode;
	//Vector <Sprite*> m_LineVec;
};
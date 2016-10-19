#pragma once

#include "cocos2d.h"

USING_NS_CC;

class MyEffect
{
public:

	MyEffect();
	~MyEffect();
	void showLineEffect(Point p, Node* node);

	void showRowEffect(Point p, Node* node);

	void showBoomEffect(Point p, Node* node);

	void showEffect(Point p, Node* node);

	bool removeEffect(Node* node);
	void removeAllEffect(Node* node);
	
private:
	Vector<ParticleSystem*> m_effects;
};




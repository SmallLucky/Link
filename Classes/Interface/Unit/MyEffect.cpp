
#include "MyEffect.h"

MyEffect::MyEffect()
{
	m_effects.clear();
}

MyEffect::~MyEffect()
{
}

void MyEffect::showLineEffect(Point p, Node* node)
{

}

void MyEffect::showRowEffect(Point p, Node* node)
{

}

void MyEffect::showBoomEffect(Point p, Node* node)
{

}

void MyEffect::showEffect(Point p, Node* node)
{
	ParticleSystem * efft = ParticleSystemQuad::create("effect/particle_1.plist");
		//ParticleSystem* m_emitter1 = ParticleSystemQuad::create("effect/particle_1.plist");
	efft->setPosition(p);
	m_effects.pushBack(efft);
	node->addChild(efft, 1);
}

bool MyEffect::removeEffect(Node* node)
{
	if (m_effects.empty())
	{
		return false;
	}
	ParticleSystem* e = m_effects.back();
	m_effects.popBack();
	node ->removeChild(e);
	return true;
}

void MyEffect::removeAllEffect(Node* node)
{
	if (!m_effects.empty())
	{
		//while ();
	}
}
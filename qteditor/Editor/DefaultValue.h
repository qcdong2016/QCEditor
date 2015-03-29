#ifndef _DEFAULT_VALUE_H_
#define _DEFAULT_VALUE_H_

#include "2d/CCNode.h"
#include "2d/CCSprite.h"
#include "2d/CCParticleSystemQuad.h"
USING_NS_CC;

namespace DefaultValue
{
	Node* defaultNodeCtor();
	Sprite* defaultSpriteCtor();
	ParticleSystemQuad* defaultParticleSystem();
}


#endif//_DEFAULT_VALUE_H_
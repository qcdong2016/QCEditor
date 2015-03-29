#ifndef _DEFAULT_VALUE_H_
#define _DEFAULT_VALUE_H_

namespace cocos2d
{
	class Node;
	class LabelTTF;
	class LabelBMFont;
	class Sprite;
	class ParticleSystemQuad;
}

using namespace cocos2d;

namespace DefaultValue
{
	Node* defaultNodeCtor();
	LabelBMFont* defaultBMFont();
	LabelTTF* defaultLabelTTF();
	Sprite* defaultSpriteCtor();
	ParticleSystemQuad* defaultParticleSystem();
}


#endif//_DEFAULT_VALUE_H_
#include "DefaultValue.h"
#ifdef QT_GUI_LIB//lazy
#include "Editor/EditorNode.h"
#endif

#include "2d/CCNode.h"
#include "2d/CCSprite.h"
#include "2d/CCParticleSystemQuad.h"
#include "2d/CCLabelBMFont.h"
#include "2d/CCLabelTTF.h"

Node* DefaultValue::defaultNodeCtor()
{
#ifdef QT_GUI_LIB//lazy
	Node* node = EditorNode::create();
#else
	Node* node = Node::create();
#endif
	node->setContentSize(Size(100, 100));
	return node;
}

Sprite* DefaultValue::defaultSpriteCtor()
{
	Sprite* sp = Sprite::create("image.png");
	return sp;
}

ParticleSystemQuad* DefaultValue::defaultParticleSystem()
{
	ParticleSystemQuad* psq = ParticleSystemQuad::create("default_particle.plist");
	psq->setContentSize(Size(100, 100));
	return psq;
}

LabelBMFont* DefaultValue::defaultBMFont()
{
	return LabelBMFont::create("123", "helvetica-32.fnt");
}

LabelTTF* DefaultValue::defaultLabelTTF()
{
	return LabelTTF::create("123", "Marker Felt.ttf", 20);
}

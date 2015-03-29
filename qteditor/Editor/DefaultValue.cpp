#include "DefaultValue.h"
#include "EditorNode.h"

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
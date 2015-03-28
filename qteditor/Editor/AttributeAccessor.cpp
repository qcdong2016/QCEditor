#include "AttributeAccessor.h"
#include "2d/CCNode.h"
#include "2d/CCSprite.h"
#include "2d/CCParticleSystem.h"

AAManager AAManager::_instance;

const AAManager& AAManager::getInstance()
{
	return _instance;
}

AAManager::AAManager()
{
	initAll();
}

AAManager::~AAManager()
{
	releaseAll();
}

void AAManager::releaseAll()
{
}

static std::string getSpriteTextureName(const Sprite*) { return "image.png"; }//lazy
static void setSpriteTextureName(Sprite* sp, const std::string& name) { sp->setTexture(name); }

static Node* defaultNodeCtor() 
{
	Node* node = Node::create();
	node->setContentSize(Size(100, 100));
	return node;
}

static Sprite* defaultSpriteCtor()
{
	Sprite* sp = Sprite::create("image.png");
	return sp;
}

static ParticleSystem* defaultParticleSystem()
{
	return ParticleSystem::create("default_particle.plist");
}

#define ATTR_(trait, name, get, set, typeName, defaultValue) \
	currentGroup->add(new AAInfo(new AttributeAccessorImpl<Node, typeName, trait<typeName> >(name, get, set), defaultValue)) \

#define ATTR(name, get, set, typeName, defaultValue) ATTR_(AttributeTrait, name, get, set, typeName, defaultValue)
#define ATTRMixed(name, get, set, typeName, defaultValue) ATTR_(MixedAttributeTrait, name, get, set, typeName, defaultValue)

#define ATTRMMS(name, get, set, typeName, defaultValue, mini, maxi, step) \
	currentGroup->add(new AAInfo(new AttributeAccessorImpl<Node, typeName, AttributeTrait<typeName> >(name, get, set), defaultValue, mini, maxi, step)) \

#define ATTRSTEP(name, get, set, typeName, defaultValue, step) \
	ATTRMMS(name, get, set, typeName, defaultValue, Variant(), Variant(), step)

#define StartGroup(typeName, ctor) \
	currentGroup = (_groups[#typeName] = new ObjectMethodInfo(#typeName, new StaticConstructor<typeName>(ctor)))

#define EndGroup()

#define ATTR1_(trait, name, get, set, typeName, defaultValue) \
	currentGroup->add(new AAInfo(new AttributeAccessorHelper<Sprite, typeName, trait<typeName> >(name, get, set), defaultValue))
#define ATTR1(name, get, set, typeName, defaultValue) ATTR1_(AttributeTrait, name, get, set, typeName, defaultValue)
#define ATTRMixed1(name, get, set, typeName, defaultValue) ATTR1_(MixedAttributeTrait, name, get, set, typeName, defaultValue)

#define Require(typeName) /*todo*/

void AAManager::initAll()
{
	ObjectMethodInfo* currentGroup;

	StartGroup(Node, defaultNodeCtor);
	ATTR("Local Z Order", &Node::getLocalZOrder, &Node::setLocalZOrder, int, 0);
	ATTR("Global Z Order", &Node::getGlobalZOrder, &Node::setGlobalZOrder, float, 0.0f);
	ATTR("Visible", &Node::isVisible, &Node::setVisible, bool, true);
	ATTRSTEP("Scale X", &Node::getScaleX, &Node::setScaleX, float, 1.0, 0.1);
	ATTRSTEP("Scale Y", &Node::getScaleY, &Node::setScaleY, float, 1.0, 0.1);
	ATTR("Rotation", &Node::getRotation, &Node::setRotation, float, 0);
	ATTR("Position", &Node::getPosition, &Node::setPosition, Vec2, Vec2(0, 0));
	ATTR("Tag", &Node::getTag, &Node::setTag, int, 0);
	ATTRMixed("Name", &Node::getName, &Node::setName, std::string, std::string());
	ATTRMMS("Anchor Pos", &Node::getAnchorPoint, &Node::setAnchorPoint, Vec2, Vec2(0, 0), Vec2(0, 0), Vec2(1, 1), Vec2(0.1, 0.1));
	EndGroup();

	StartGroup(Sprite, defaultSpriteCtor);
	Require(Node);
	ATTRMixed1("Texture", &getSpriteTextureName, &setSpriteTextureName, std::string, std::string());
	EndGroup();

	StartGroup(ParticleSystem, defaultParticleSystem);
	Require(Node);
	//todo
	EndGroup();
}

#include "AttributeAccessor.h"
#include "2d/CCNode.h"
#include "2d/CCSprite.h"
#include "2d/CCParticleSystem.h"
#include "2d/CCParticleSystemQuad.h"

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

AccessorGroup* AAManager::getGroup(const std::string& name) const
{
	auto iter = _groups.find(name);

	if (iter != _groups.end())
		return iter->second;

	return nullptr;
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

static ParticleSystemQuad* defaultParticleSystem()
{
	ParticleSystemQuad* psq = ParticleSystemQuad::create("default_particle.plist");
	psq->setContentSize(Size(100, 100));
	return psq;
}

#define ATTR_(trait, name, get, set, typeName, defaultValue) \
	currentGroup->add(new AAInfo(new AttributeAccessorImpl<TYPE, typeName, trait<typeName> >(name, get, set), defaultValue)) \

#define ATTR(name, get, set, typeName, defaultValue) ATTR_(AttributeTrait, name, get, set, typeName, defaultValue)
#define ATTRMixed(name, get, set, typeName, defaultValue) ATTR_(MixedAttributeTrait, name, get, set, typeName, defaultValue)

#define ATTRMMS(name, get, set, typeName, defaultValue, mini, maxi, step) \
	currentGroup->add(new AAInfo(new AttributeAccessorImpl<TYPE, typeName, AttributeTrait<typeName> >(name, get, set), defaultValue, mini, maxi, step)) \

#define ATTRSTEP(name, get, set, typeName, defaultValue, step) \
	ATTRMMS(name, get, set, typeName, defaultValue, Variant(), Variant(), step)

#define StartGroup(typeName, ctor) {\
	typedef typeName TYPE; \
	currentGroup = (_groups[#typeName] = new AccessorGroup(#typeName, new StaticConstructor<typeName>(ctor)))

#define EndGroup() }

#define ATTR1_(trait, name, get, set, typeName, defaultValue) \
	currentGroup->add(new AAInfo(new AttributeAccessorHelper<Sprite, typeName, trait<typeName> >(name, get, set), defaultValue))
#define ATTR1(name, get, set, typeName, defaultValue) ATTR1_(AttributeTrait, name, get, set, typeName, defaultValue)
#define ATTRMixed1(name, get, set, typeName, defaultValue) ATTR1_(MixedAttributeTrait, name, get, set, typeName, defaultValue)

#define Require(typeName) currentGroup->parent = AAManager::getInstance().getGroup(#typeName)

void AAManager::initAll()
{
	AccessorGroup* currentGroup;

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

	StartGroup(ParticleSystemQuad, defaultParticleSystem);
	Require(Node);

	ATTR("Duration", &ParticleSystem::getDuration, &ParticleSystem::setDuration, float, 0);
	ATTR("Source Position", &ParticleSystem::getSourcePosition, &ParticleSystem::setSourcePosition, Vec2, Vec2(0,0));
	ATTR("Pos Var", &ParticleSystem::getPosVar, &ParticleSystem::setPosVar, Vec2, Vec2(0, 0));
	ATTR("Life", &ParticleSystem::getLife, &ParticleSystem::setLife, float, 0);
	ATTR("Life Var", &ParticleSystem::getLifeVar, &ParticleSystem::setLifeVar, float, 0);
	ATTR("Angle", &ParticleSystem::getAngle, &ParticleSystem::setAngle, float, 0);
	ATTR("Angle Var", &ParticleSystem::getAngleVar, &ParticleSystem::setAngleVar, float, 0);
	ATTR("Start Size", &ParticleSystem::getStartSize, &ParticleSystem::setStartSize, float, 0);
	ATTR("Start Size Var", &ParticleSystem::getStartSizeVar, &ParticleSystem::setStartSizeVar, float, 0);
	ATTR("End Size", &ParticleSystem::getEndSize, &ParticleSystem::setEndSize, float, 0);
	ATTR("End Size Var", &ParticleSystem::getEndSizeVar, &ParticleSystem::setEndSizeVar, float, 0);
	ATTR("Start Spin", &ParticleSystem::getStartSpin, &ParticleSystem::setStartSpin, float, 0);
	ATTR("Start Spin Var", &ParticleSystem::getStartSpinVar, &ParticleSystem::setStartSpinVar, float, 0);
	ATTR("End Spin", &ParticleSystem::getEndSpin, &ParticleSystem::setEndSpin, float, 0);
	ATTR("End Spin Var", &ParticleSystem::getEndSpinVar, &ParticleSystem::setEndSpinVar, float, 0);

	ATTR("Start Color", &ParticleSystem::getStartColor, &ParticleSystem::setStartColor, Color4F, Color4F::WHITE);
	ATTR("Start Color Var", &ParticleSystem::getStartColorVar, &ParticleSystem::setStartColorVar, Color4F, Color4F::WHITE);
	ATTR("End Color", &ParticleSystem::getEndColor, &ParticleSystem::setEndColor, Color4F, Color4F::WHITE);
	ATTR("End Color Var", &ParticleSystem::getEndColorVar, &ParticleSystem::setEndColorVar, Color4F, Color4F::WHITE);

	ATTR("Emission Rate", &ParticleSystem::getEmissionRate, &ParticleSystem::setEmissionRate, float, 0);
	ATTR("Total Particles", &ParticleSystem::getTotalParticles, &ParticleSystem::setTotalParticles, int, 0);
	ATTR("Opacity Modify RGB", &ParticleSystem::isOpacityModifyRGB, &ParticleSystem::setOpacityModifyRGB, bool, false);

	EndGroup();
}

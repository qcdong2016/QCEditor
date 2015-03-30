#include "AttributeAccessor.h"
#include "2d/CCNode.h"
#include "2d/CCSprite.h"
#include "2d/CCParticleSystem.h"
#include "2d/CCParticleSystemQuad.h"
#include "DefaultValue.h"
#include "2d/CCLabelTTF.h"
#include "2d/CCLabelBMFont.h"
#include "StringUtil.h"

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

#define type_to_string(type) \
	static void type ## _to_string(const type & v, std::string& ret) { ret = StringUtil::toString(v); } \

type_to_string(int);
type_to_string(float);
type_to_string(double);
type_to_string(bool);

static void Vec2_to_string(const Vec2& v, std::string& ret) { ret = StringUtil::toString(v.x, v.y); }
static void Size_to_string(const Size& v, std::string& ret) { ret = StringUtil::toString(v.width, v.height); }
static void Color4F_to_string(const Color4F& v, std::string& ret) { ret = StringUtil::toString(v.r, v.g, v.b, v.a); }
static void string_to_string(const std::string& v, std::string& ret) { ret = v; }

static std::string getSpriteTextureName(const Sprite*) { return "image.png"; }//lazy
static void setSpriteTextureName(Sprite* sp, const std::string& name) { sp->setTexture(name); }

static std::string getFntFile(const LabelBMFont* label) { return label->getFntFile(); }
static void setFntFile(LabelBMFont* label, const std::string& file) { label->setFntFile(file); }

#define ATTR_(trait, name, get, set, typeName, defaultValue) \
	currentGroup->add(new AAInfo(new AttributeAccessorImpl<TYPE, typeName, trait<typeName> >(name, get, set, typeName ## _to_string), defaultValue)) \

#define ATTR(name, get, set, typeName, defaultValue) ATTR_(AttributeTrait, name, get, set, typeName, defaultValue)
#define ATTRMixed(name, get, set, typeName, defaultValue) ATTR_(MixedAttributeTrait, name, get, set, typeName, defaultValue)

#define ATTRMMS(name, get, set, typeName, defaultValue, mini, maxi, step) \
	currentGroup->add(new AAInfo(new AttributeAccessorImpl<TYPE, typeName, AttributeTrait<typeName> >(name, get, set, typeName ## _to_string), defaultValue, mini, maxi, step)) \

#define ATTRSTEP(name, get, set, typeName, defaultValue, step) \
	ATTRMMS(name, get, set, typeName, defaultValue, Variant(), Variant(), step)

#define StartGroup(typeName, ctor) {\
	typedef typeName TYPE; \
	currentGroup = (_groups[#typeName] = new AccessorGroup(#typeName, new StaticConstructor<typeName>(ctor)))

#define EndGroup() }

#define ATTR1_(trait, name, get, set, typeName, defaultValue) \
	currentGroup->add(new AAInfo(new AttributeAccessorHelper<TYPE, typeName, trait<typeName> >(name, get, set, typeName ## _to_string), defaultValue))
#define ATTR1(name, get, set, typeName, defaultValue) ATTR1_(AttributeTrait, name, get, set, typeName, defaultValue)
#define ATTRMixed1(name, get, set, typeName, defaultValue) ATTR1_(MixedAttributeTrait, name, get, set, typeName, defaultValue)

#define Require(typeName) currentGroup->parent = AAManager::getInstance().getGroup(#typeName)

void AAManager::initAll()
{
	using namespace std;

	AccessorGroup* currentGroup;

	StartGroup(Node, DefaultValue::defaultNodeCtor);
	ATTR("Local Z Order", &Node::getLocalZOrder, &Node::setLocalZOrder, int, 0);
	ATTR("Global Z Order", &Node::getGlobalZOrder, &Node::setGlobalZOrder, float, 0.0f);
	ATTR("Visible", &Node::isVisible, &Node::setVisible, bool, true);
	ATTRSTEP("Scale X", &Node::getScaleX, &Node::setScaleX, float, 1.0, 0.1);
	ATTRSTEP("Scale Y", &Node::getScaleY, &Node::setScaleY, float, 1.0, 0.1);
	ATTR("Rotation", &Node::getRotation, &Node::setRotation, float, 0);
	ATTR("Position", &Node::getPosition, &Node::setPosition, Vec2, Vec2(0, 0));
	ATTR("Tag", &Node::getTag, &Node::setTag, int, 0);
	ATTRMixed("Name", &Node::getName, &Node::setName, string, string());
	ATTRMMS("Anchor Pos", &Node::getAnchorPoint, &Node::setAnchorPoint, Vec2, Vec2(0, 0), Vec2(0, 0), Vec2(1, 1), Vec2(0.1, 0.1));
	EndGroup();

	StartGroup(Sprite, DefaultValue::defaultSpriteCtor);
	Require(Node);
	ATTRMixed1("Texture", &getSpriteTextureName, &setSpriteTextureName, string, string());
	EndGroup();

	StartGroup(LabelBMFont, DefaultValue::defaultBMFont);
	Require(Node);
	ATTR("String", &LabelBMFont::getString, &LabelBMFont::setString, string, string());
	ATTRMixed1("Fnt File", &::getFntFile, &::setFntFile, string, string());
	EndGroup();

	StartGroup(LabelTTF, DefaultValue::defaultLabelTTF);
	Require(Node);
	ATTR("String", &LabelTTF::getString, &LabelTTF::setString, string, string());
	ATTR("Font File", &LabelTTF::getFontName, &LabelTTF::setFontName, string, string());
	ATTR("Font Size", &LabelTTF::getFontSize, &LabelTTF::setFontSize, float, 20);
	ATTR("Dimensions", &LabelTTF::getDimensions, &LabelTTF::setDimensions, Size, Size(0,0));
	EndGroup();

	StartGroup(ParticleSystemQuad, DefaultValue::defaultParticleSystem);
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

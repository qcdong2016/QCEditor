#include "AttributeAccessor.h"
#include "2d/CCNode.h"

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

void AAManager::initAll()
{
#define ATTR_(trait, name, get, set, typeName, defaultValue) \
	(_map[name] = new AttributeAccessorInfo(new AttributeAccessorImpl<Node, typeName, trait<typeName> >(name, get, set), defaultValue)) \

#define ATTR(name, get, set, typeName, defaultValue) ATTR_(AttributeTrait, name, get, set, typeName, defaultValue)
#define ATTRMixed(name, get, set, typeName, defaultValue) ATTR_(MixedAttributeTrait, name, get, set, typeName, defaultValue)

#define ATTRSTEP(name, get, set, typeName, defaultValue, step) \
	ATTR(name, get, set, typeName, defaultValue)->singleStep = Variant(step)

#define ATTRMMS(name, get, set, typeName, defaultValue, mini, maxi, step) \
	ATTR(name, get, set, typeName, defaultValue)->setValue(maxi, mini, step)

	//b.beginGroup("Node Property");
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
	//b.endGroup();
}

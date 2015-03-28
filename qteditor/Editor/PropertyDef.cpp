#include "PropertyDef.h"
#include "qtpropertybrowser.h"
#include "qtvariantproperty.h"
#include "2d/CCNode.h"
#include "GL/glew.h"
#include "qpoint.h"
#include "qttreepropertybrowser.h"
#include "StringUtil.h"
#include "math/Vec2.h"
#include <string>
#include "AttributeAccessor.h"

USING_NS_CC;

class Builder
{
public:

	void set(Node* node, QtTreePropertyBrowser* browser, QtVariantPropertyManager* mgr)
	{
		_node = node;
		_mgr = mgr;
		_browser = browser;
	}

	void clear() 
	{
		for (auto& iter : _map)
		{
			delete iter.second;
		}
		_map.clear();
	}

	void setValue(Node* node, QtProperty* p, const QVariant& v)
	{
		auto& iter = _map.find(p);
		if (iter != _map.end())
		{
			AttributeAccessor* pr = iter->second;
			pr->set(node, v);
		}
	}

	AttributeAccessor* get(QtProperty* p)
	{
		auto& iter = _map.find(p);
		if (iter != _map.end())
			return iter->second;
		else
			return nullptr;
	}

	virtual void beginGroup(const char* name)
	{
		_current = _mgr->addProperty(QtVariantPropertyManager::groupTypeId(), QLatin1String(name));
	}

	virtual void endGroup()
	{
		_browser->addProperty(_current);
	}

	virtual QtVariantProperty* add(const char* name, AttributeAccessor* accessor, const Variant& defaultValue)
	{
		QString dfname = defaultValue.value<QVariant>().typeName();
		QtVariantProperty *item = _mgr->addProperty(defaultValue.value<QVariant>().type(), QLatin1String(name));

		_map[item] = accessor;

		Variant value;
		accessor->get(_node, value);
		QVariant v = value.value<QVariant>();
		QString tpname = v.typeName();

		item->setValue(v);

		_current->addSubProperty(item);

		return item;
	}

	virtual QtVariantProperty* add(
		const char* name, AttributeAccessor* accessor, const Variant& defaultValue, const Variant& minimum, const Variant& maximum)
	{
		QtVariantProperty *item = add(name, accessor, defaultValue);

		item->setAttribute(QLatin1String("minimum"), minimum.value<QVariant>());
		item->setAttribute(QLatin1String("maximum"), maximum.value<QVariant>());

		return item;
	}

	virtual QtVariantProperty* add(
		const char* name, AttributeAccessor* accessor, const Variant& defaultValue, const Variant& minimum, const Variant& maximum, const Variant& step)
	{
		QtVariantProperty *item = add(name, accessor, defaultValue, minimum, maximum);

		item->setAttribute(QLatin1String("singleStep"), step.value<QVariant>());
		return item;
	}

	virtual QtVariantProperty* add(const char* name, AttributeAccessor* accessor, const Variant& defaultValue, const Variant& step)
	{
		QtVariantProperty *item = add(name, accessor, defaultValue);
		item->setAttribute(QLatin1String("singleStep"), step.value<QVariant>());
		return item;
	}

	virtual QtProperty* get(const QString& name)
	{
		for (auto iter = _map.begin(); iter != _map.end(); ++iter)
		{
			QtProperty* p = iter->first;

			if (p->propertyName() == name)
				return p;
		}
		return nullptr;
	}

	Node* _node;
	QtTreePropertyBrowser* _browser;
	QtVariantProperty* _current;
	QtVariantPropertyManager* _mgr;
	std::map<QtProperty*, AttributeAccessor*> _map;
};

static Builder s_builder;
static QtVariantProperty* s_position_prop;

#define ATTR_(trait, name, get, set, typeName, defaultValue) {\
	auto accessor = new AttributeAccessorImpl<Node, typeName, trait<typeName> >(); \
	accessor->setFunction(get, set); \
	b.add(name, accessor, defaultValue); }

#define ATTR(name, get, set, typeName, defaultValue) ATTR_(AttributeTrait, name, get, set, typeName, defaultValue)
#define ATTRMixed(name, get, set, typeName, defaultValue) ATTR_(MixedAttributeTrait, name, get, set, typeName, defaultValue)

#define ATTRSTEP(name, get, set, typeName, defaultValue, step) {\
	auto accessor = new AttributeAccessorImpl<Node, typeName, AttributeTrait<typeName> >(); \
	accessor->setFunction(get, set); \
	b.add(name, accessor, defaultValue, step); }

#define ATTRMMS(name, get, set, typeName, defaultValue, mini, maxi, step) {\
	auto accessor = new AttributeAccessorImpl<Node, typeName, AttributeTrait<typeName> >(); \
	accessor->setFunction(get, set); \
	b.add(name, accessor, defaultValue, mini, maxi, step); }


void PropertyDef::setupProperties(Node* node, QtTreePropertyBrowser* browser, QtVariantPropertyManager* mgr)
{
	Builder& b = s_builder;
	b.set(node, browser, mgr);

	b.beginGroup("Node Property");
		ATTR("Local Z Order", &Node::getLocalZOrder, &Node::setLocalZOrder, int, 0);
		ATTR("Global Z Order", &Node::getGlobalZOrder, &Node::setGlobalZOrder, float, 0.0f);
		ATTR("Visible", &Node::isVisible, &Node::setVisible, bool, true);
		ATTRSTEP("Scale X", &Node::getScaleX, &Node::setScaleX, float, 1.0, 0.1);//don't using 1.0f,
		ATTRSTEP("Scale Y", &Node::getScaleY, &Node::setScaleY, float, 1.0, 0.1);
		ATTR("Rotation", &Node::getRotation, &Node::setRotation, float, 0);
		ATTR("Position", &Node::getPosition, &Node::setPosition, Vec2, Vec2(0, 0));
		ATTR("Tag", &Node::getTag, &Node::setTag, int, 0);
		ATTRMixed("Name", &Node::getName, &Node::setName, std::string, std::string());
		ATTRMMS("Anchor Pos", &Node::getAnchorPoint, &Node::setAnchorPoint, Vec2, Vec2(0.5, 0.5), Vec2(0, 0), Vec2(1, 1), Vec2(0.1, 0.1));
	b.endGroup();

	s_position_prop = (QtVariantProperty*)b.get("Position");
}

void PropertyDef::setProperty(Node* node, QtProperty* prop, const QVariant& value)
{
	s_builder.setValue(node, prop, value);
}

void PropertyDef::clear()
{
	s_builder.clear();
}

void PropertyDef::setPosition(Node* node, const QPoint& pos)
{
	s_position_prop->setValue(pos);
}

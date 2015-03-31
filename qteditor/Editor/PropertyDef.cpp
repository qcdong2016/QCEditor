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

	virtual void beginGroup(const std::string& name)
	{
		_current = _mgr->addProperty(QtVariantPropertyManager::groupTypeId(), QLatin1String(name.c_str()));
		_rootProps[name] = _current;
	}

	virtual void endGroup()
	{
		_browser->addProperty(_current);
	}

	virtual void updateValue(QtVariantProperty* item, AttributeAccessor* accessor, const Variant& minimum, const Variant& maximum, const Variant& step)
	{
		Variant value;
		accessor->get(_node, value);
		QVariant v = value.value<QVariant>();
		QString tpname = v.typeName();

		item->setValue(v);

		if (!minimum.isNull())
			item->setAttribute(QLatin1String("minimum"), minimum.value<QVariant>());
		if (!maximum.isNull())
			item->setAttribute(QLatin1String("maximum"), maximum.value<QVariant>());
		if (!step.isNull())
			item->setAttribute(QLatin1String("singleStep"), step.value<QVariant>());

	}

	virtual QtVariantProperty* add(
		const std::string& name, AttributeAccessor* accessor, QVariant::Type qtype, const Variant& minimum, const Variant& maximum, const Variant& step)
	{
		QtVariantProperty *item = _mgr->addProperty(qtype, QLatin1String(name.c_str()));

		_map[item] = accessor;
		updateValue(item, accessor, minimum, maximum, step);
		_current->addSubProperty(item);

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

	virtual QtVariantProperty* isIn(const std::string& name)
	{
		auto iter = _rootProps.find(name);
		if (iter != _rootProps.end())
		{
			return iter->second;
		}

		return nullptr;
	}

	Node* _node;
	QtTreePropertyBrowser* _browser;
	QtVariantProperty* _current;
	QtVariantPropertyManager* _mgr;
	std::map<QtProperty*, AttributeAccessor*> _map;
	std::map<std::string, QtVariantProperty*> _rootProps;
};

static Builder s_builder;
static QtVariantProperty* s_position_prop = nullptr;

static void addAccessorGroup(AccessorGroup* ag)
{
	QtVariantProperty* old = s_builder.isIn(ag->name);
	//if old valid, update it, not re-create

	s_builder.beginGroup(ag->name);

	AccessorGroup::AAInfoList::iterator iter;

	for (iter = ag->infolist.begin(); iter != ag->infolist.end(); iter++)
	{
		AAInfo* aainfo = *iter;
		s_builder.add(aainfo->accessor->getName(),
			aainfo->accessor, Variant::typeToQtype(aainfo->type), aainfo->mini, aainfo->maxi, aainfo->singleStep);
	}
	s_builder.endGroup();

	if (ag->parent != nullptr)
		addAccessorGroup(ag->parent);
}

void PropertyDef::setupProperties(const std::string& typeName, Node* instance, QtTreePropertyBrowser* browser, QtVariantPropertyManager* mgr)
{
	browser->clear();//re-write

	Builder& b = s_builder;
	b.set(instance, browser, mgr);

	const AAManager::GroupMap& map = AAManager::getInstance().getGroups();
	
	AAManager::GroupMap::const_iterator iter;

	iter = map.find(typeName);
	if (iter != map.end())
	{
		addAccessorGroup(iter->second);
	}

	s_position_prop = (QtVariantProperty*)b.get("Position");
}

void PropertyDef::setProperty(Node* node, QtProperty* prop, const QVariant& value)
{
	s_builder.setValue(node, prop, value);
}

void PropertyDef::setPosition(Node* node, const QPoint& pos)
{
	s_position_prop->setValue(pos);
}

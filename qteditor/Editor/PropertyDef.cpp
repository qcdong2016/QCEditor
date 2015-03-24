#include "PropertyDef.h"
#include "qtpropertybrowser.h"
#include "qtvariantproperty.h"
#include "2d/CCNode.h"
#include "GL/glew.h"
#include "qpoint.h"
#include "qttreepropertybrowser.h"

USING_NS_CC;

template<typename ValueType>
class PropImpl : public Prop
{
public:
	typedef ValueType(*getFunctionPtr)(const Node*);
	typedef void(*setFunctionPtr)(Node*, const ValueType&);

	PropImpl(getFunctionPtr getF, setFunctionPtr setF)
		: getFunction_(getF)
		, setFunction_(setF)
	{}
	virtual void get(const Node* classPtr, QVariant& dest) const
	{
		dest = getFunction_(classPtr);
	}

	virtual void set(Node* classPtr, const QVariant& src)
	{
		setFunction_(classPtr, src.value<ValueType>());
	}

	getFunctionPtr getFunction_;
	setFunctionPtr setFunction_;
};

static int getOpacity(const Node* node) { return node->getOpacity(); }
static void setOpacity(Node* node, const int& value) { node->setOpacity((GLubyte)value); }
static bool getVisible(const Node* node) { return node->isVisible(); }
static void setVisible(Node* node, const bool& value) { node->setVisible(value); }

static int getLocalZOrder(const Node* node) { return node->getLocalZOrder(); }
static void setLocalZOrder(Node* node, const int& value) { node->setLocalZOrder(value); }

static int getGlobalZOrder(const Node* node) { return node->getGlobalZOrder(); }
static void setGlobalZOrder(Node* node, const int& value) { node->setGlobalZOrder(value); }

static float getScaleX(const Node* node) { return node->getScaleX(); }
static void setScaleX(Node* node, const float& value) { node->setScaleX(value); }
static float getScaleY(const Node* node) { return node->getScaleY(); }
static void setScaleY(Node* node, const float& value) { node->setScaleY(value); }

static float getRotation(const Node* node) { return node->getRotation(); }
static void setRotation(Node* node, const float& value) { node->setRotation(value); }

static int getTag(const Node* node) { return node->getTag(); }
static void setTag(Node* node, const int& value) { node->setTag(value); }

static QString getName(const Node* node) { return node->getName().c_str(); }
static void setName(Node* node, const QString& value) { node->setName(std::string(value.toUtf8().constData())); }

static QPointF getPos(const Node* node) {
	QPointF spos;
	spos.setX(node->getPositionX());
	spos.setY(node->getPositionY());

	return spos;
}

static void setPos(Node* node, const QPointF& pos) {
	node->setPosition(pos.x(), pos.y());
}

static QPointF getAnchorPoint(const Node* node) {
	QPointF spos;
	Vec2 ach = node->getAnchorPoint();

	return QPointF(ach.x, ach.y);
}

static void setAnchorPoint(Node* node, const QPointF& pos) {
	node->setAnchorPoint(Vec2(pos.x(), pos.y()));
}


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
			Prop* pr = iter->second;
			pr->set(node, v);
		}
	}

	Prop* get(QtProperty* p)
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

	virtual QtVariantProperty* add(const char* name, Prop* accessor, const QVariant& defaultValue)
	{
		QtVariantProperty *item = _mgr->addProperty(defaultValue.type(), QLatin1String(name));

		_map[item] = accessor;

		QVariant value;
		accessor->get(_node, value);
		item->setValue(value);

		_current->addSubProperty(item);

		return item;
	}

	virtual QtVariantProperty* add(
		const char* name, Prop* accessor, const QVariant& defaultValue, const QVariant& minimum, const QVariant& maximum)
	{
		QtVariantProperty *item = add(name, accessor, defaultValue);

		item->setAttribute(QLatin1String("minimum"), minimum);
		item->setAttribute(QLatin1String("maximum"), maximum);

		QVariant value;
		accessor->get(_node, value);

		item->setValue(value);

		return item;
	}

	virtual QtVariantProperty* add(
		const char* name, Prop* accessor, const QVariant& defaultValue, const QVariant& minimum, const QVariant& maximum, const QVariant& step)
	{
		QtVariantProperty *item = add(name, accessor, defaultValue, minimum, maximum);

		item->setAttribute(QLatin1String("singleStep"), step);
		return item;
	}

	virtual QtVariantProperty* add(const char* name, Prop* accessor, const QVariant& defaultValue, const QVariant& step)
	{
		QtVariantProperty *item = add(name, accessor, defaultValue);
		item->setAttribute(QLatin1String("singleStep"), step);
		return item;
	}

	Node* _node;
	QtTreePropertyBrowser* _browser;
	QtVariantProperty* _current;
	QtVariantPropertyManager* _mgr;
	std::map<QtProperty*, Prop*> _map;
};

static Builder s_builder;
static QtVariantProperty* s_position_prop;

void PropertyDef::setupProperties(Node* node, QtTreePropertyBrowser* browser, QtVariantPropertyManager* mgr)
{
	Builder& b = s_builder;
	b.set(node, browser, mgr);

#define ATTR(name, getFunction, setFunction, typeName, defaultValue) \
	b.add(name, new PropImpl<typeName >(getFunction, setFunction), defaultValue)

#define ATTRSTEP(name, getFunction, setFunction, typeName, defaultValue, step) \
	b.add(name, new PropImpl<typeName >(getFunction, setFunction), defaultValue, step)

#define ATTRMMS(name, getFunction, setFunction, typeName, defaultValue, mini, maxi, step) \
	b.add(name, new PropImpl<typeName >(getFunction, setFunction), defaultValue, mini, maxi, step)

	b.beginGroup("Node Property");
		ATTR("Local Z Order", getLocalZOrder, setLocalZOrder, int, 0);
		ATTR("Global Z Order", getGlobalZOrder, setGlobalZOrder, int, 0);
		ATTR("Visible", getVisible, setVisible, bool, true);
		ATTRSTEP("Scale X", getScaleX, setScaleX, float, 1.0, 0.1);//don't using 1.0f,
		ATTRSTEP("Scale Y", getScaleY, setScaleY, float, 1.0, 0.1);
		ATTR("Rotation", getRotation, setRotation, float, 0);
		s_position_prop = ATTR("Position", getPos, setPos, QPointF, QPointF(0, 0));
		ATTR("Tag", getTag, setTag, int, 0);
		ATTR("Name", getName, setName, QString, QString());
		ATTRMMS("Anchor Pos", getAnchorPoint, setAnchorPoint, QPointF, QPointF(0.5, 0.5), QPointF(0, 0), QPointF(1, 1), QPointF(0.1, 0.1));
	b.endGroup();
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

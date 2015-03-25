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

USING_NS_CC;

template<typename T>
static inline QVariant toQVariant(T v)
{
	return QVariant(v);
}

static inline QVariant toQVariant(const Vec2& v)
{
	return QVariant(QPointF(v.x, v.y));
}

static inline QVariant toQVariant(const Size& v)
{
	return QVariant(QSizeF(v.width, v.height));
}

static inline QVariant toQVariant(const std::string& v)
{
	return QVariant(QString(v.c_str()));
}

template<typename T>
static inline T toCCValue(const QVariant& value)
{
	return value.value<T>();
}
template<>
static inline std::string toCCValue(const QVariant& value)
{
	QString str = value.toString();
	return std::string(str.toUtf8(), str.length());
}

template<>
static inline Vec2 toCCValue(const QVariant& value)
{
	QPointF p = value.toPointF();
	return Vec2(p.x(), p.y());
}

template<>
static inline Size toCCValue(const QVariant& value)
{
	QSizeF s = value.toSizeF();
	return Size(s.width(), s.height());
}

template<typename T> struct AttributeTrait
{
	/// Get function return type.
	typedef const T& ReturnType;
	/// Set function parameter type.
	typedef const T& ParameterType;
};

template<> struct AttributeTrait<int>
{
	typedef int ReturnType;
	typedef int ParameterType;
};

template<> struct AttributeTrait<bool>
{
	typedef bool ReturnType;
	typedef bool ParameterType;
};

template<> struct AttributeTrait<float>
{
	typedef float ReturnType;
	typedef float ParameterType;
};

template<typename T> struct MixedAttributeTrait
{
	typedef T ReturnType;
	typedef const T& ParameterType;
};


/// Template implementation of the attribute accessor invoke helper class.
template <typename T, typename U, typename Trait> class AttributeAccessorImpl : public Prop
{
public:
	typedef typename Trait::ReturnType(T::*getFunctionPtr)() const;
	typedef void (T::*setFunctionPtr)(typename Trait::ParameterType);
	typedef QVariant (*fromStringFunctionPtr)(const std::string&);

	void setFunction(getFunctionPtr getFunction, setFunctionPtr setFunction)
	{
		_getFunc = getFunction;
		_setFunc = setFunction;
	}

	void setFunction(fromStringFunctionPtr setFunction)
	{
	}

	/// Invoke getter function.
	virtual void get(const Node* ptr, QVariant& dest) const
	{
		assert(ptr);
		const T* classPtr = static_cast<const T*>(ptr);
		U value = (classPtr->*_getFunc)();
		toQVariant(value);
	}

	/// Invoke setter function.
	virtual void set(Node* ptr, const QVariant& value)
	{
		assert(ptr);
		T* classPtr = static_cast<T*>(ptr);
		(classPtr->*_setFunc)(toCCValue<U>(value));
	}

	virtual std::string save(const QVariant& value)
	{
		return StringUtil::toString(value);
	}

	virtual QVariant read(const std::string& str)
	{
		return QVariant();
	}

	/// Class-specific pointer to getter function.
	getFunctionPtr _getFunc;
	/// Class-specific pointer to setter function.
	setFunctionPtr _setFunc;
};

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
	std::map<QtProperty*, Prop*> _map;
};

static Builder s_builder;
static QtVariantProperty* s_position_prop;

#define ATTR_(trait, name, get, set, typeName, defaultValue) {\
	auto accessor = new AttributeAccessorImpl<Node, typeName, trait<typeName> >(); \
	accessor->setFunction(get, set); \
	b.add(name, accessor, toQVariant(defaultValue)); }

#define ATTR(name, get, set, typeName, defaultValue) ATTR_(AttributeTrait, name, get, set, typeName, defaultValue)
#define ATTRMixed(name, get, set, typeName, defaultValue) ATTR_(MixedAttributeTrait, name, get, set, typeName, defaultValue)

#define ATTRSTEP(name, get, set, typeName, defaultValue, step) {\
	auto accessor = new AttributeAccessorImpl<Node, typeName, AttributeTrait<typeName> >(); \
	accessor->setFunction(get, set); \
	b.add(name, accessor, toQVariant(defaultValue), toQVariant(step)); }

#define ATTRMMS(name, get, set, typeName, defaultValue, mini, maxi, step) {\
	auto accessor = new AttributeAccessorImpl<Node, typeName, AttributeTrait<typeName> >(); \
	accessor->setFunction(get, set); \
	b.add(name, accessor, toQVariant(defaultValue), toQVariant(mini), toQVariant(maxi), toQVariant(step)); }


void PropertyDef::setupProperties(Node* node, QtTreePropertyBrowser* browser, QtVariantPropertyManager* mgr)
{
	Builder& b = s_builder;
	b.set(node, browser, mgr);

	b.beginGroup("Node Property");
		ATTR("Local Z Order", &Node::getLocalZOrder, &Node::setLocalZOrder, int, 0);
		ATTR("Global Z Order", &Node::getGlobalZOrder, &Node::setGlobalZOrder, float, 0.0);
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

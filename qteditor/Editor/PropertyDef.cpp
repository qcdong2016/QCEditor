#include "PropertyDef.h"
#include "qtpropertybrowser.h"
#include "qtvariantproperty.h"
#include "2d/CCNode.h"
#include "GL/glew.h"
#include "qpoint.h"

USING_NS_CC;

template<typename ValueType>
class PropImpl : public Prop
{
public:
	typedef ValueType(*getFunctionPtr)(const EditAble*);
	typedef void(*setFunctionPtr)(EditAble*, const ValueType&);

	PropImpl(getFunctionPtr getF, setFunctionPtr setF)
		: getFunction_(getF)
		, setFunction_(setF)
	{}
	virtual void get(const EditAble* classPtr, QVariant& dest) const
	{
		dest = getFunction_(classPtr);
	}

	virtual void set(EditAble* classPtr, const QVariant& src)
	{
		setFunction_(classPtr, src.value<ValueType>());
	}

	getFunctionPtr getFunction_;
	setFunctionPtr setFunction_;
};

#define ATTR(name, getFunction, setFunction, typeName, defaultValue) \
	new AttributeInfo(name, new PropImpl<typeName >(getFunction, setFunction), defaultValue)

#define ATTRSTEP(name, getFunction, setFunction, typeName, defaultValue, step) \
	new AttributeInfo(name, new PropImpl<typeName >(getFunction, setFunction), defaultValue, step)

#define ATTRMMS(name, getFunction, setFunction, typeName, defaultValue, mini, maxi, step) \
	new AttributeInfo(name, new PropImpl<typeName >(getFunction, setFunction), defaultValue, mini, maxi, step)

static int getOpacity(const EditAble* node) { return node->getOpacity(); }
static void setOpacity(EditAble* node, const int& value) { node->setOpacity((GLubyte)value); }
static bool getVisible(const EditAble* node) { return node->isVisible(); }
static void setVisible(EditAble* node, const bool& value) { node->setVisible(value); }

static int getLocalZOrder(const EditAble* node) { return node->getLocalZOrder(); }
static void setLocalZOrder(EditAble* node, const int& value) { node->setLocalZOrder(value); }

static int getGlobalZOrder(const EditAble* node) { return node->getGlobalZOrder(); }
static void setGlobalZOrder(EditAble* node, const int& value) { node->setGlobalZOrder(value); }

static float getScaleX(const EditAble* node) { return node->getScaleX(); }
static void setScaleX(EditAble* node, const float& value) { node->setScaleX(value); }
static float getScaleY(const EditAble* node) { return node->getScaleY(); }
static void setScaleY(EditAble* node, const float& value) { node->setScaleY(value); }

static float getRotation(const EditAble* node) { return node->getRotation(); }
static void setRotation(EditAble* node, const float& value) { node->setRotation(value); }

static int getTag(const EditAble* node) { return node->getTag(); }
static void setTag(EditAble* node, const int& value) { node->setTag(value); }

static QString getName(const EditAble* node) { return node->getName().c_str(); }
static void setName(EditAble* node, const QString& value) { node->setName(std::string(value.toUtf8().constData())); }

static QPointF getPos(const EditAble* node) {
	QPointF spos;
	spos.setX(node->getPositionX());
	spos.setY(node->getPositionY());

	return spos;
}

static void setPos(EditAble* node, const QPointF& pos) {
	node->setPosition(pos.x(), pos.y());
}

static QPointF getAnchorPoint(const EditAble* node) {
	QPointF spos;
	Vec2 ach = node->getAnchorPoint();

	return QPointF(ach.x, ach.y);
}

static void setAnchorPoint(EditAble* node, const QPointF& pos) {
	node->setAnchorPoint(Vec2(pos.x(), pos.y()));
}

void PropertyDef::cocos2d_Node_properties(AttributeInfoMap& map)
{
	map.set(ATTR("Local Z Order", getLocalZOrder, setLocalZOrder, int, 0));
	map.set(ATTR("Global Z Order", getGlobalZOrder, setGlobalZOrder, int, 0));
	map.set(ATTR("Visible", getVisible, setVisible, bool, true));
	map.set(ATTRSTEP("Scale X", getScaleX, setScaleX, float, 1.0, 0.1));//don't using 1.0f,
	map.set(ATTRSTEP("Scale Y", getScaleY, setScaleY, float, 1.0, 0.1));
	map.set(ATTR("Rotation", getRotation, setRotation, float, 0));
	map.set(ATTR("Position", getPos, setPos, QPointF, QPointF(0, 0)));

	map.set(ATTR("Tag", getTag, setTag, int, 0));
	map.set(ATTR("Name", getName, setName, QString, QString()));

	//fuck,qpoint can't using Single Step.
	map.set(ATTRMMS("Anchor Pos", getAnchorPoint, setAnchorPoint, QPointF, QPointF(0.5, 0.5), QPointF(0, 0), QPointF(1, 1), QPointF(0.1, 0.1)));
}

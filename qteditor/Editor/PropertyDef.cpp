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

static int getOpacity(const EditAble* node) { return node->getOpacity(); }
static void setOpacity(EditAble* node, const int& value) { node->setOpacity((GLubyte)value); }
static bool getVisible(const EditAble* node) { return node->isVisible(); }
static void setVisible(EditAble* node, const bool& value) { node->setVisible(value); }

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
	map.set(ATTR("Visible", getVisible, setVisible, bool, true));
	map.set(ATTR("Opacity", getOpacity, setOpacity, int, 255));
	map.set(ATTR("Position", getPos, setPos, QPointF, QPointF(0, 0)));
	map.set(ATTR("Anchor Pos", getAnchorPoint, setAnchorPoint, QPointF, QPointF(0.5, 0.5)));
}

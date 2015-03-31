#ifndef _VARIANT_H_
#define _VARIANT_H_

#include "Any.h"
#include "math/Vec2.h"
#include "math/CCGeometry.h"
#include "base/ccTypes.h"

#ifdef QT_GUI_LIB//lazy
#include "qvariant.h"
#include "qsize.h"
#include "qpoint.h"
#include "qcolor.h"
#endif

USING_NS_CC;

//all number in Variant was a double

#define isNumber(type) (((type) & 0x10) == 0x10)
class Variant
{
public:

	enum Type
	{
		TNull = 0x00,
		TBool = 0x01, 
		TVec2 = 0x02, 
		TSize = 0x03, 
		TString = 0x04, 
		TColor = 0x05,

		TInt = 0x10,
		TFloat = 0x11,
		TDouble = 0x12,
	};
	Variant() : _type(TNull) {}

	Variant(bool v) { *this = v; }
	Variant(int v) { *this = v; }
	Variant(float v) { *this = v; }
	Variant(double v) { *this = v; }
	Variant(const Vec2& v) { *this = v; }
	Variant(const Size& v) { *this = v; }
	Variant(const std::string& v) { *this = v; }
	Variant(const Color4F& v) { *this = v; }
	Variant(const Variant& v) { _type = v._type; _content = v._content; }

#ifdef QT_GUI_LIB//lazy
	Variant(const QSizeF& v) { *this = v; }
	Variant(const QPointF& v) { *this = v; }
	Variant(const QString& v) { *this = v; }
	Variant(const QVariant& v) { *this = v; }
	Variant(const QColor& v) { *this = v; }
#endif
	bool isNull() const { return _type == TNull; }

public:

	template<typename valuetype> valuetype value() const;


	Variant& operator=(const double& v)
	{
		_type = TDouble;
		_content = v;
		return *this;
	}

	Variant& operator=(const float& v)
	{
		_type = TFloat;
		_content.operator=<double>(v);
		return *this;
	}

	Variant& operator=(const int& v)
	{
		_type = TInt;
		_content.operator=<double>(v);
		return *this;
	}

	Variant& operator=(const bool& v)
	{
		_type = TBool;
		_content = v;
		return *this;
	}

	Variant& operator=(const std::string& v)
	{
		_type = TString;
		_content = v;
		return *this;
	}

	Variant& operator=(const Color4F& v)
	{
		_type = TColor;
		_content = v;
		return *this;
	}

	Variant& operator=(const Vec2& v)
	{
		_type = TVec2;
		_content = v;
		return *this;
	}

	Variant& operator=(const Size& v)
	{
		_type = TSize;
		_content = v;
		return *this;
	}

#ifdef QT_GUI_LIB//lazy
	Variant& operator=(const QString& v)
	{
		*this = std::string(v.toUtf8());
		return *this;
	}

	Variant& operator=(const QSizeF& v)
	{
		_type = TSize;
		_content = Size(v.width(), v.height());
		return *this;
	}
	Variant& operator=(const QPointF& v)
	{
		_type = TVec2;
		_content = Vec2(v.x(), v.y());
		return *this;
	}

	Variant& operator=(const QColor& v)
	{
		_type = TColor;
		_content = Color4F(v.redF(), v.greenF(), v.blueF(), v.alphaF());
		return *this;
	}

	Variant& operator=(const QVariant& v)
	{
		if (v.type() == QVariant::SizeF)
			*this = v.toSizeF();
		else if (v.type() == QVariant::PointF)
			*this = v.toPointF();
		else if (v.type() == QVariant::Bool)
			*this = v.toBool();
		else if (v.type() == QVariant::Int)
			*this = v.toInt();
		else if (v.type() == QVariant::Double)
			*this = v.toDouble();
		else if (v.type() == QVariant::String)
			*this = v.toString();
		else if (v.type() == QVariant::Color)
			*this = v.value<QColor>();
		else
			xLog("Variant not support %s", v.typeName());
		return *this;
	}
#endif

	bool operator==(const Variant& rhs)
	{
		if (isNumber(_type) && isNumber(rhs._type))
			return (value<double>() == rhs.value<double>());

		if (this->_type != rhs._type) return false;

		if (_type == TBool) return (value<bool>() == rhs.value<bool>());
		if (_type == TVec2) return (value<Vec2>() == rhs.value<Vec2>());
		if (_type == TString) return (value<std::string>() == rhs.value<std::string>());
		if (_type == TColor) return (value<Color4F>() == rhs.value<Color4F>());
		if (_type == TSize) {//..
			const Size& s1 = value<Size>();
			const Size& s2 = rhs.value<Size>();
			return s1.width == s2.width && s1.height == s2.height;
		}

		return false;
	}

	Type getType() { return _type; }
#ifdef QT_GUI_LIB//lazy
	QVariant::Type getQType()
	{
		return typeToQtype(_type);
	}

	static QVariant::Type typeToQtype(Type tp)
	{
		if (isNumber(tp))
			return QVariant::Double;

		if (tp == TBool) return QVariant::Bool;
		if (tp == TVec2) return QVariant::PointF;
		if (tp == TString) return QVariant::String;
		if (tp == TColor) return QVariant::Color;
		if (tp == TSize) return QVariant::SizeF;

		return QVariant::Invalid;
	}
#endif

private:
	Type _type;
	Any _content;
};

#endif//_VARIANT_H_
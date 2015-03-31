#include "Varinat.h"

template<typename valuetype> 
valuetype Variant::value() const
{
	return *_content.castType<valuetype>();
}


template<>
float Variant::value<float>() const
{
	double v = *_content.castType<double>();
	return (float)v;
}

template<>
int Variant::value<int>() const
{
	int v = *_content.castType<double>();
	return (int)v;
}

#ifdef QT_GUI_LIB//lazy
template<>
QVariant Variant::value<QVariant>() const
{
	if (_type == TInt || _type == TFloat || _type == TDouble)
		return QVariant(value<double>());
	else if (_type == TBool)
		return QVariant(value<bool>());
	else if (_type == TString)
		return QVariant(value<std::string>().c_str());
	else if (_type == TVec2) {
		Vec2 v = value<Vec2>();
		return QVariant(QPointF(v.x, v.y));
	}
	else if (_type == TSize) {
		Size v = value<Size>();
		return QVariant(QSizeF(v.width, v.height));
	}
	else if (_type == TColor) {
		Color4F v = value<Color4F>();
		return QVariant(QColor(v.r * 255.0, v.g * 255.0, v.b * 255.0, v.a * 255.0));
	}

	return QVariant();
}
#endif
#ifndef _PROPERTY_DEF_H_
#define _PROPERTY_DEF_H_

#include "qvariant.h"
#include "../qtcocos2d.h"
#include "qtpropertybrowser.h"
#include "qtvariantproperty.h"
#include "qttreepropertybrowser.h"

USING_NS_CC;

class PropertyDef
{
public:
	static void setupProperties(const std::string& typeName, Node* instance, QtTreePropertyBrowser* browser, QtVariantPropertyManager* mgr);
	static void setPosition(Node* node, const QPoint& pos);
	static void setProperty(Node* node, QtProperty* prop, const QVariant& value);
};


#endif//_PROPERTY_DEF_H_

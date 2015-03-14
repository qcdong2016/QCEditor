#ifndef _PROPERTY_DEF_H_
#define _PROPERTY_DEF_H_

#include "qvariant.h"
#include "2d/CCNode.h"

typedef cocos2d::Node EditAble;

class Prop
{
public:
	virtual void get(const EditAble* classPtr, QVariant& dest) const = 0;
	virtual void set(EditAble* classPtr, const QVariant& src) = 0;
};

/// Description of an automatically serializable variable.
struct AttributeInfo
{

	AttributeInfo(const char* name, Prop* accessor, const QVariant& defaultValue)
	: _accessor(accessor)
	, _name(name)
	, _defaultValue(defaultValue)
	{
	}

	~AttributeInfo() {
		if (_accessor)
			delete _accessor;
	}

	QVariant _defaultValue;
	Prop* _accessor;
	std::string _name;
};

struct AttributeInfoMap
{
	void set(AttributeInfo* info)
	{
		infoMap[info->_name] = info;
	}

	AttributeInfo* get(const std::string& key)
	{
		return infoMap[key];
	}

	~AttributeInfoMap() 
	{
		for (auto& iter = infoMap.begin(); iter != infoMap.end(); iter++)
		{
			delete iter->second;
		}
	}

	std::map<std::string, AttributeInfo* > infoMap;
};




class PropertyDef
{
public:

	static void cocos2d_Node_properties(AttributeInfoMap& map);
};


#endif//_PROPERTY_DEF_H_
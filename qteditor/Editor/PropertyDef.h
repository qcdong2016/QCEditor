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

struct AttributeInfo
{
	AttributeInfo(const char* name, Prop* accessor, const QVariant& defaultValue)
		: _accessor(accessor)
		, _name(name)
		, _defaultValue(defaultValue)
		, _setMinimum(false)
		, _setMaximum(false)
		, _setStep(false)
	{
	}

	AttributeInfo(const char* name, Prop* accessor, const QVariant& defaultValue, const QVariant& minimum, const QVariant& maximum)
		: _accessor(accessor)
		, _name(name)
		, _defaultValue(defaultValue)
		, _setMinimum(true)
		, _setMaximum(true)
		, _setStep(false)
		, _minimum(minimum)
		, _maximum(maximum)
	{
	}

	AttributeInfo(const char* name, Prop* accessor, const QVariant& defaultValue, const QVariant& minimum, const QVariant& maximum, const QVariant& step)
		: _accessor(accessor)
		, _name(name)
		, _defaultValue(defaultValue)
		, _setMinimum(true)
		, _setMaximum(true)
		, _minimum(minimum)
		, _maximum(maximum)
		, _setStep(true)
		, _singleStep(step)
	{
	}

	AttributeInfo(const char* name, Prop* accessor, const QVariant& defaultValue, const QVariant& step)
		: _accessor(accessor)
		, _name(name)
		, _defaultValue(defaultValue)
		, _setMinimum(false)
		, _setMaximum(false)
		, _setStep(true)
		, _singleStep(step)
	{
	}

	~AttributeInfo() {
		if (_accessor)
			delete _accessor;
	}

	QVariant _defaultValue;
	Prop* _accessor;
	std::string _name;

	bool _setMinimum;
	bool _setMaximum;
	QVariant _minimum;
	QVariant _maximum;

	bool _setStep;
	QVariant _singleStep;
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
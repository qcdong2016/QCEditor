#ifndef _ATTRIBUTEACCESSOR_H_
#define _ATTRIBUTEACCESSOR_H_

#include "Varinat.h"
#include <string>
#include <list>
#include <map>

class AttributeAccessor
{
public:
	AttributeAccessor(const std::string& name) : _name(name) {}

	virtual void get(const Node* classPtr, Variant& dest) const = 0;
	virtual void set(Node* classPtr, const Variant& src) = 0;

	virtual void save(const Variant& value, std::string& out) = 0;
	virtual void read(const std::string& str, Variant& out) = 0;

	virtual const std::string& getName() { return _name; }

private:
	std::string _name;
};

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

template<typename T> struct AttributeTraitEnum
{
	/// Get function return type.
	typedef T ReturnType;
	/// Set function parameter type.
	typedef T ParameterType;
};

template <typename T, typename U, typename Trait> 
class AttributeAccessorImpl : public AttributeAccessor
{
public:

	typedef typename Trait::ReturnType(T::*getFunctionPtr)() const;
	typedef void (T::*setFunctionPtr)(typename Trait::ParameterType);
	typedef void(*toStringFuncPtr)(const U&, std::string&);
	typedef U(*formStringFuncPtr)(const std::string&);

	AttributeAccessorImpl(const std::string& name,
		getFunctionPtr getFunction, setFunctionPtr setFunction, toStringFuncPtr toStrF, formStringFuncPtr fromStrF)
		: AttributeAccessor(name)
		, _getFunc(getFunction)
		, _setFunc(setFunction)
		, _toStringFunc(toStrF)
		, _fromStringFunc(fromStrF)
	{}

	/// Invoke getter function.
	virtual void get(const Node* ptr, Variant& dest) const
	{
		assert(ptr);
		const T* classPtr = static_cast<const T*>(ptr);
		U value = (classPtr->*_getFunc)();
		dest = value;
	}

	/// Invoke setter function.
	virtual void set(Node* ptr, const Variant& value)
	{
		assert(ptr);
		T* classPtr = static_cast<T*>(ptr);
		(classPtr->*_setFunc)(value.value<U>());
	}

	virtual void save(const Variant& value, std::string& out)
	{
		_toStringFunc(value.value<U>(), out);
	}

	virtual void read(const std::string& str, Variant& out)
	{
		out = _fromStringFunc(str);
	}

	getFunctionPtr _getFunc;
	setFunctionPtr _setFunc;
	toStringFuncPtr _toStringFunc;
	formStringFuncPtr _fromStringFunc;
};


template <typename T, typename U, typename Trait> 
class AttributeAccessorHelper : public AttributeAccessor
{
public:

	typedef typename Trait::ReturnType(*getFunctionPtr)(const T*);
	typedef void(*setFunctionPtr)(T*, typename Trait::ParameterType);
	typedef void(*toStringFuncPtr)(const U&, std::string&);
	typedef U(*formStringFuncPtr)(const std::string&);

	AttributeAccessorHelper(const std::string& name, 
		getFunctionPtr getFunction, setFunctionPtr setFunction, toStringFuncPtr toStrF, formStringFuncPtr fromStrF)
		: AttributeAccessor(name)
		, _getFunc(getFunction)
		, _setFunc(setFunction)
		, _toStringFunc(toStrF)
		, _fromStringFunc(fromStrF)
	{}

	/// Invoke getter function.
	virtual void get(const Node* ptr, Variant& dest) const
	{
		assert(ptr);
		const T* classPtr = static_cast<const T*>(ptr);
		U value = _getFunc(classPtr);
		dest = value;
	}

	/// Invoke setter function.
	virtual void set(Node* ptr, const Variant& value)
	{
		assert(ptr);
		T* classPtr = static_cast<T*>(ptr);
		_setFunc(classPtr, value.value<U>());
	}

	virtual void save(const Variant& value, std::string& out)
	{
		_toStringFunc(value.value<U>(), out);
	}

	virtual void read(const std::string& str, Variant& out)
	{
		out = _fromStringFunc(str);
	}

	getFunctionPtr _getFunc;
	setFunctionPtr _setFunc;
	toStringFuncPtr _toStringFunc;
	formStringFuncPtr _fromStringFunc;
};



template <typename T, typename U>
class AttributeAccessorEnum : public AttributeAccessor
{
	typedef U(T::*getFunctionPtr)() const;
	typedef void (T::*setFunctionPtr)(U);
public:
	AttributeAccessorEnum(const std::string& name,
		getFunctionPtr getFunction, setFunctionPtr setFunction)
		: AttributeAccessor(name)
		, _getFunc(getFunction)
		, _setFunc(setFunction)
	{}

	/// Invoke getter function.
	virtual void get(const Node* ptr, Variant& dest) const
	{
		assert(ptr);
		const T* classPtr = static_cast<const T*>(ptr);
		U value = (classPtr->*_getFunc)();
		dest = (int)value;
	}

	/// Invoke setter function.
	virtual void set(Node* ptr, const Variant& value)
	{
		assert(ptr);
		T* classPtr = static_cast<T*>(ptr);
		(classPtr->*_setFunc)((U)value.value<int>());
	}

	virtual void save(const Variant& value, std::string& out)
	{
		StringUtil::toString(value.value<int>(), out);
	}

	virtual void read(const std::string& str, Variant& out)
	{
		out = StringUtil::parseValue<int>(str);
	}
	getFunctionPtr _getFunc;
	setFunctionPtr _setFunc;
};

struct EnumInfo
{
	const char* name;
	int value;
};

struct AAInfo
{
	Variant::Type type;

	AttributeAccessor* accessor;
	Variant maxi;
	Variant mini;
	Variant singleStep;
	Variant defaultValue;

	EnumInfo* enuminfo;
	int enuminfo_count;

	AAInfo(AttributeAccessor* accessor_, Variant::Type type_)
		: accessor(accessor_)
		, type(type_)
		, enuminfo(nullptr)
	{
	}

	AAInfo(AttributeAccessor* accessor_, Variant def = Variant(), Variant mx = Variant(), Variant mi = Variant(), Variant step = Variant())
		: accessor(accessor_)
		, defaultValue(def)
		, mini(mi)
		, maxi(mx)
		, singleStep(step)
		, enuminfo(nullptr)
	{
		type = defaultValue.getType();
	}

	AAInfo(AttributeAccessor* accessor_, Variant def, EnumInfo* info, int count)
		: accessor(accessor_)
		, defaultValue(def)
		, enuminfo(info)
		, enuminfo_count(count)
	{
		type = Variant::TInt;
	}

	~AAInfo() { delete accessor; }

	bool isEnum() { return enuminfo != nullptr; }

	void setValue(const Variant& mx, const Variant& mi, const Variant& step)
	{
		maxi = mx; mini = mi; singleStep = step;
	}
};

struct Constructor
{
	virtual Node* operator()() = 0;
};

template<typename T>
struct StaticConstructor : public Constructor
{
	typedef T* (*createFunctionPtr)();
	StaticConstructor(createFunctionPtr func) : ctor(func) {}

	virtual Node* operator()()
	{
		return ctor();
	}

	createFunctionPtr ctor;
};

struct AccessorGroup
{
	typedef std::list<AAInfo*> AAInfoList;
	void add(AAInfo* info) { infolist.push_back(info); }

	AccessorGroup(const std::string& nm, Constructor* ctr) : name(nm), ctor(ctr), parent(nullptr) {}
	~AccessorGroup()
	{
		delete ctor;
		//todo remove list elements;
	}

	AttributeAccessor* get(const std::string& name)
	{
		for (auto* info : infolist)
		{
			if (info->accessor->getName() == name)
				return info->accessor;
		}

		if (parent != nullptr)
			return parent->get(name);

		return nullptr;
	}

	AAInfoList infolist;
	std::string name;
	Constructor* ctor;
	AccessorGroup* parent;
};

class AAManager
{
public:
	AAManager();
	~AAManager();

	static const AAManager& getInstance();

	typedef std::map<std::string, AccessorGroup*> GroupMap;

	const GroupMap& getGroups() const { return _groups; }

	AccessorGroup* getGroup(const std::string& name) const;
 

private:
	GroupMap _groups;

	void releaseAll();
	void initAll();

	static AAManager _instance;
};

#endif//_ATTRIBUTEACCESSOR_H_
#ifndef _ATTRIBUTEACCESSOR_H_
#define _ATTRIBUTEACCESSOR_H_

#include "Varinat.h"
#include <string>

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


/// Template implementation of the attribute accessor invoke helper class.
template <typename T, typename U, typename Trait> class AttributeAccessorImpl : public AttributeAccessor
{
public:

	typedef typename Trait::ReturnType(T::*getFunctionPtr)() const;
	typedef void (T::*setFunctionPtr)(typename Trait::ParameterType);


	AttributeAccessorImpl(const std::string& name, getFunctionPtr getFunction, setFunctionPtr setFunction)
		: AttributeAccessor(name)
		, _getFunc(getFunction)
		, _setFunc(setFunction)
	{}

	void setFunction(getFunctionPtr getFunction, setFunctionPtr setFunction)
	{
		_getFunc = getFunction;
		_setFunc = setFunction;
	}

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
	}

	virtual void read(const std::string& str, Variant& out)
	{
	}

	/// Class-specific pointer to getter function.
	getFunctionPtr _getFunc;
	/// Class-specific pointer to setter function.
	setFunctionPtr _setFunc;
};



struct AttributeAccessorInfo
{
	AttributeAccessor* accessor;
	Variant maxi;
	Variant mini;
	Variant singleStep;
	Variant defaultValue;

	AttributeAccessorInfo(AttributeAccessor* accessor_, Variant def) : accessor(accessor_), defaultValue(def) {}
	~AttributeAccessorInfo() { delete accessor; }

	void setValue(const Variant& mx, const Variant& mi, const Variant& step)
	{
		maxi = mx; mini = mi; singleStep = step;
	}
};

class AAManager
{
public:
	typedef std::map<std::string, AttributeAccessorInfo*> AAInfoMap;
	AAManager();
	~AAManager();

	static const AAManager& getInstance();


	const AAInfoMap& getMap() const { return _map; }
private:

	void releaseAll();
	void initAll();


	AAInfoMap _map;

	static AAManager _instance;
};

#endif//_ATTRIBUTEACCESSOR_H_
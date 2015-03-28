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


template <typename T, typename U, typename Trait> 
class AttributeAccessorImpl : public AttributeAccessor
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


template <typename T, typename U, typename Trait> 
class AttributeAccessorHelper : public AttributeAccessor
{
public:

	typedef typename Trait::ReturnType(*getFunctionPtr)(const T*);
	typedef void(*setFunctionPtr)(T*, typename Trait::ParameterType);

	AttributeAccessorHelper(const std::string& name, getFunctionPtr getFunction, setFunctionPtr setFunction)
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
	}

	virtual void read(const std::string& str, Variant& out)
	{
	}

	getFunctionPtr _getFunc;
	setFunctionPtr _setFunc;
};

struct AAInfo
{
	AttributeAccessor* accessor;
	Variant maxi;
	Variant mini;
	Variant singleStep;
	Variant defaultValue;

	//lazy group
	std::string groupname;
	bool isGroup() { return groupname.length() != 0; }
	AAInfo(const std::string& name) : groupname(name) {}
	AAInfo(AttributeAccessor* accessor_, Variant def, Variant mx = Variant(), Variant mi = Variant(), Variant step = Variant())
		: accessor(accessor_)
		, defaultValue(def)
		, mini(mi)
		, maxi(mx)
		, singleStep(step)
	{}

	~AAInfo() { delete accessor; }

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
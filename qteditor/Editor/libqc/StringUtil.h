#ifndef _STRING_UTIL_
#define _STRING_UTIL_

#include <sstream>
#include <string>

namespace StringUtil
{
	template<typename T>
	inline std::string toString(T p)
	{
		std::ostringstream stream;
		stream << p;
		return stream.str();
	}

	inline const std::string& toString(const std::string& value)
	{
		return value;
	}

	inline std::string toString(bool b)
	{
		return b ? "true" : "false";
	}

	template<typename T1, typename T2>
	inline std::string toString(T1 p1, T2 p2)
	{
		std::ostringstream stream;
		stream << p1 << " " << p2;
		return stream.str();
	}

	template<typename T1, typename T2, typename T3>
	inline std::string toString(T1 p1, T2 p2, T3 p3)
	{
		std::ostringstream stream;
		stream << p1 << " " << p2 << " " << p3;
		return stream.str();
	}

	template<typename T1, typename T2, typename T3, typename T4>
	inline std::string toString(T1 p1, T2 p2, T3 p3, T4 p4)
	{
		std::ostringstream stream;
		stream << p1 << " " << p2 << " " << p3 << " " << p4;
		return stream.str();
	}

	template<typename T>
	inline T parseValue(const std::string& _value)
	{
		std::istringstream stream(_value);
		T result;
		stream >> result;
		if (stream.fail())
			return T();
		else
		{
			int item = stream.get();
			while (item != -1)
			{
				if (item != ' ' && item != '\t')
					return T();
				item = stream.get();
			}
		}
		return result;
	}

	template<>
	inline bool parseValue(const std::string& _value)
	{
		if (_value == "True" || _value == "true" || _value == "1")
			return true;
		return false;
	}

	template<typename T1, typename T2>
	inline T1 parseValueEx2(const std::string& _value)
	{
		T2 p1, p2;
		std::istringstream stream(_value);
		stream >> p1 >> p2;
		if (stream.fail())
			return T1();
		else
		{
			int item = stream.get();
			while (item != -1)
			{
				if (item != ' ' && item != '\t')
					return T1();
				item = stream.get();
			}
		}
		return T1(p1, p2);
	}

	template<typename T1, typename T2>
	inline T1 parseValueEx3(const std::string& _value)
	{
		T2 p1, p2, p3;
		std::istringstream stream(_value);
		stream >> p1 >> p2 >> p3;
		if (stream.fail())
			return T1();
		else
		{
			int item = stream.get();
			while (item != -1)
			{
				if (item != ' ' && item != '\t')
					return T1();
				item = stream.get();
			}
		}
		return T1(p1, p2, p3);
	}

	template<typename T1, typename T2>
	inline T1 parseValueEx4(const std::string& _value)
	{
		T2 p1, p2, p3, p4;
		std::istringstream stream(_value);
		stream >> p1 >> p2 >> p3 >> p4;
		if (stream.fail())
			return T1();
		else
		{
			int item = stream.get();
			while (item != -1)
			{
				if (item != ' ' && item != '\t')
					return T1();
				item = stream.get();
			}
		}
		return T1(p1, p2, p3, p4);
	}
}

#endif//_STRING_UTIL_
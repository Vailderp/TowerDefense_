#pragma once
#include <ctime>
#include <type_traits>
	/*
	 * Статический класс для получения времени
	 */

template<typename Type>
constexpr bool IsFloatType =
std::is_same<Type, float>::value ||
std::is_same<Type, double>::value ||
std::is_same<Type, long double>::value;

inline long double __fastcall operator "" _sec(const long double sec)
{
	return sec * 1000.0;
}

inline long double __fastcall  operator "" _ms(const long double ms)
{
	return ms;
}

inline long double __fastcall  operator "" _mcs(const long double mcs)
{
	return mcs / 1000.0;
}

inline long double __fastcall  operator "" _ns(const long double ns)
{
	return ns / 1000000.0;
}

template<typename Time>
class Clock
{
	static_assert(IsFloatType<Time>,
		"template's typename are not <float> or <double> or <long double>");

public:

	/*
	 * Получить время в секундах
	 */
	static Time seconds()
	{
		return static_cast<Time>(std::clock()) /
			static_cast<Time>(CLOCKS_PER_SEC);
	}

	/*
	 * Получить время в миллисекундах
	 */
	static Time milliseconds()
	{
		return seconds() * 1000;
	}

	static Time microseconds()
	{
		return seconds() * 1000000;
	}

	static Time nanoseconds()
	{
		return seconds() * 1000000000;
	}
};

/*
 * Таймер, можно очистить время
 */
class Timer
{
	

protected:
	long double *d_time_ = new long double(0);

public:

	Timer() 
	{
		*d_time_ = Clock<long double>::seconds();
	}

	compl Timer()
	{
		delete d_time_;
	}

	long double seconds() const
	{
		return static_cast<long double>(std::clock()) /
			static_cast<long double>CLOCKS_PER_SEC - *d_time_;
	}

	long double milliseconds() const
	{
		return seconds() * 1000;
	}

	long double microseconds() const
	{
		return seconds() * 1000000;
	}

	long double nanoseconds() const
	{
		return seconds() * 1000000000;
	}

	/*
	 * Сбросить время
	 */
	void clear() const
	{
		*d_time_ = static_cast<long double>(std::clock()) /
			static_cast<long double>CLOCKS_PER_SEC;
	}
};
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
template<typename Time>
class Timer
{
	static_assert(IsFloatType<Time>,
		"template's typename are not <float> or <double> or <long double>");

protected:
	Time d_time_;

public:

	Timer() :
		d_time_(Clock<Time>::seconds())
	{

	}

	Time seconds()
	{
		return static_cast<Time>(std::clock()) /
			static_cast<Time>CLOCKS_PER_SEC - d_time_;
	}

	Time milliseconds()
	{
		return seconds() * 1000;
	}

	Time microseconds()
	{
		return seconds() * 1000000;
	}

	Time nanoseconds()
	{
		return seconds() * 1000000000;
	}

	/*
	 * Сбросить время
	 */
	void clear()
	{
		d_time_ = static_cast<Time>(std::clock()) /
			static_cast<Time>CLOCKS_PER_SEC;
	}
};
#pragma once


template <typename T>
class ValueSource
{
public:
	virtual T GetCurrentValue () const = 0;
};


template <typename T>
class DynamicValueSource : public ValueSource<T>
{
public:
	virtual void Advance (float dt) = 0;
};



#pragma once


template <typename T>
class ValueSource
{
public:
	virtual T GetCurrentValue () const = 0;
};



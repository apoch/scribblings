#pragma once

#include "ValueSource.h"


class ValueSourceLinearAccumulator : public DynamicValueSource<float>
{
public:
	ValueSourceLinearAccumulator (float start, float velocity)
		: Value(start),
		  Velocity(velocity)
	{ }


	float GetCurrentValue () const override
	{
		return Value;
	}


	void Advance (float dt) override
	{
		Value += (Velocity * dt);
	}

private:
	float Value;
	float Velocity;
};


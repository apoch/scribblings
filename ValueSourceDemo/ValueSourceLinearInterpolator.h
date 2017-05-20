#pragma once

#include "ValueSource.h"


class ValueSourceLinearInterpolator : public ValueSource<float>
{
public:
	ValueSourceLinearInterpolator (float min, float max) :
		Min(min),
		Max(max),
		Value(min),
		Time(0.0f)
	{ }

	float GetCurrentValue () const override {
		return Value;
	}

	void SetTime (float t) {
		Time = t;

		if (Time < 0.0f)
			Time = 0.0f;

		if (Time > 1.0f)
			Time = 1.0f;

		Value = Min + (Max - Min) * Time;
	}

private:
	float Min;
	float Max;
	float Value;
	float Time;
};
// ValueSourceDemo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include "ValueSourceLinearInterpolator.h"


namespace ValueSourceDemo
{

	class MovingObject
	{
	public:
		void AttachPositionValueSource (ValueSource<float> * position)
		{
			Position = position;
		}

		void Render ()
		{
			std::cout << "Value-source object position: " << Position->GetCurrentValue() << std::endl;
		}

	private:
		ValueSource<float> * Position;
	};

}


namespace ClassicDesignDemo
{

	class MovingObject
	{
	public:
		MovingObject (float start, float velocity)
		{
			Position = start;
			Velocity = velocity;
		}

		void Advance (float dt)
		{
			Position += Velocity * dt;
		}

		void Render ()
		{
			std::cout << "Classic object position: " << Position << std::endl;
		}

	private:
		float Position;
		float Velocity;
	};

}


int main ()
{
	ValueSourceDemo::MovingObject vsobject;
	ValueSourceLinearInterpolator lerp(1.0f, 5.0f);
	vsobject.AttachPositionValueSource(&lerp);

	ClassicDesignDemo::MovingObject classicobject(1.0f, 4.0f);

	const float DT = 0.1f;
	float time = 0.0f;
	while (time <= 1.0f)
	{
		time += DT;

		lerp.SetTime(time);
		classicobject.Advance(DT);

		vsobject.Render();
		classicobject.Render();
	}


    return 0;
}


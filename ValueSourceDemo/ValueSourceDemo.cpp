//
// This is a compact little demo designed to illustrate an architectural
// application of dependency inversion. Objects moving through the small
// simulation world are updated in various ways to demonstrate basic but
// useful shifts in perspective on how data is piped around a program.
//
// More specific information can be found in the comments on each object
// type and in the supporting infrastructure code.
//

#include "stdafx.h"


#include "ValueSourceAccumulator.h"
#include "ValueSourceLinearInterpolator.h"


//
// Everything in this namespace relies on a concept of "dynamic value sources."
//
// See the ValueSource.h header for details on what this entails. The fundamental
// process of updating an object should look very familiar: the world should call
// into Advance() to update state, and then it can query that state to perform an
// operation like Render().
//
namespace DynamicValueSourceDemo
{

	//
	// A simple 1-axis game object. Moves over time.
	//
	// Note how we have to specifically "Advance" time here to move
	// the simulation forwards. This is a common game architectural
	// pattern. However, note that we don't store any data internal
	// to the object itself, just a pointer to something that will,
	// as needed, "feed" us that data.
	//
	class MovingObject
	{
	public:

		//
		// Decouple the position of our object from the object itself.
		// This is a useful way to allow any kind of motion to control
		// the object, such as a spline, spring oscillator, or even an
		// entirely custom piece of code (or external script)!
		//
		void AttachPositionValueSource (DynamicValueSource<float> * position)
		{
			Position = position;
		}

		//
		// Traditional game advancement routine, except instead of doing
		// any complex operations ourselves, we delegate to the attached
		// value source. Again, this allows any kind of controller to be
		// driving the movement of this object, with no strong coupling.
		//
		void Advance (float dt)
		{
			if (Position)
				Position->Advance(dt);
		}

		//
		// Display a representation of this game object on the console.
		//
		void Render ()
		{
			if (Position)
				std::cout << "Value-source object position: " << Position->GetCurrentValue() << std::endl;
		}

	private:
		DynamicValueSource<float> * Position = nullptr;
	};

}


//
// This is an example of how classical "reactive programming" can be done
// using the value source concept. Note the lack of specific update code.
// Instead, we drive the "data stream" separately, as illustrated by main
// below.
//
namespace ReactiveProgrammingDemo
{

	//
	// Again a simple game object. Note the lack of Advance().
	//
	class MovingObject
	{
	public:

		//
		// This function lets us decouple the data stream used to feed the
		// position of this object from the object itself. Note that time
		// is explicitly driven *outside* this object, not by an Advance()
		// loop. This allows for things like rewinding time!
		//
		void AttachPositionValueSource (ValueSource<float> * position)
		{
			Position = position;
		}

		//
		// Display our game object on the console.
		//
		void Render ()
		{
			if (Position)
				std::cout << "Reactive programming object position: " << Position->GetCurrentValue() << std::endl;
		}

	private:
		ValueSource<float> * Position = nullptr;
	};

}


//
// If the stuff above was too weird, here's a breath of fresh, sane air.
//
// This is exactly how most games would implement a moving object. You give
// the object some initial state, periodically advance the state by some time
// step, and then display the results at your leisure.
//
namespace ClassicDesignDemo
{

	//
	// Still just a simple game object.
	//
	class MovingObject
	{
	public:
		//
		// Note that we need to construct oursleves with some initial state.
		//
		MovingObject (float start, float velocity)
		{
			Position = start;
			Velocity = velocity;
		}

		//
		// Advancing pretty much looks like you'd expect.
		//
		void Advance (float dt)
		{
			Position += Velocity * dt;
		}

		//
		// And, as usual, here's our way to draw onto the console.
		//
		void Render ()
		{
			std::cout << "Classic object position: " << Position << std::endl;
		}

	private:
		float Position;
		float Velocity;
	};

}


//
// Here's the actual simulation implementation for our project.
//
// Note that we can intermingle all three design methods into a single
// update loop, and it's not hard to see that these objects could also
// interact with one another.
//
int main ()
{

	// Instantiate a game object using the "normal" way of doing things
	ClassicDesignDemo::MovingObject classicobject(1.0f, 4.0f);


	//
	// Set up a moving object powered by a dynamic value source.
	//
	// Note that we initialize the *value source* instead of the game
	// object, and then attach the two together. This is powerful. In
	// this code we use the same value source always, but imagine how
	// it would behave if we replaced value sources on the fly. Where
	// this object is currently controlled by a linear accumulator we
	// could just easily transfer it onto a spring, spline, or really
	// anything else. Drive it over the network, drive it from a data
	// stream on disk, use a scripting language to write AI for it...
	//
	DynamicValueSourceDemo::MovingObject dvsobject;
	ValueSourceLinearAccumulator movement(1.0f, 4.0f);
	dvsobject.AttachPositionValueSource(&movement);

	//
	// For comparison's sake, this is what a reactive programming
	// version of value sources looks like. Note that it looks an
	// awful lot like the explicitly stepped version, but there's
	// a few tradeoffs. RP lets you rewind time, for example, but
	// that's a heavier lift in a typical update-render loop kind
	// of architecture.
	//
	ReactiveProgrammingDemo::MovingObject rpobject;
	ValueSourceLinearInterpolator lerp(1.0f, 5.0f);		// Min and max instead of start and velocity
	rpobject.AttachPositionValueSource(&lerp);


	//
	// Now the actual update/present loop!
	//
	const float DT = 0.1f;
	float time = 0.0f;
	while (time <= 1.0f)
	{
		// Move forward the clock and display the current timestamp
		time += DT;
		std::cout << "Tick at " << time << std::endl;

		// Advance our value-source-driven objects
		classicobject.Advance(DT);
		dvsobject.Advance(DT);

		// Set the time for our RP-driven object
		lerp.SetTime(time);

		// Render everybody
		classicobject.Render();
		dvsobject.Render();
		rpobject.Render();
	}


    return 0;
}


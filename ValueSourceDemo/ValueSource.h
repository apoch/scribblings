#pragma once


//
// Instead of coding to an implementation of a float-typed value,
// this encourages coding to an interface. We don't retrieve data
// directly, but instead do it through an intermediate object - a
// value source - so that the underlying source of data can vary.
// For example, an NPC's position might be controlled by any of a
// number of data sources: static coordinates, pathing on a loop,
// seeking a particular target, or even network-replicated state.
//
template <typename T>
class ValueSource
{
public:
	virtual T GetCurrentValue () const = 0;
};


//
// If you don't like the reactive-programming model so much, you
// can just use a traditional update/present architecture. While
// there are tradeoffs for each, this is probably a more readily
// approachable architecture. Note that all we have to do is add
// an idea of "advancing" the state of the value source.
//
// It is perfectly acceptable to do nothing in Advance() and use
// only static values, for example. It is also acceptable to run
// the data through any number of indirections before it arrives
// in the value source, e.g. a network stack.
//
template <typename T>
class DynamicValueSource : public ValueSource<T>
{
public:
	virtual void Advance (float dt) = 0;
};



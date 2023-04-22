#pragma once

#include <math.h>
#include <limits>

//WG - Modified to a struct as classes generally indicate objects that live in the heap instead of the stack.
//They're also generally immutable unless unless passed explicitly by ref just by nature of the default copy constructor.
//In c# a class point would be a big nono.

struct Point2D 
{ 
public:
	float X;
	float Y;

	//Initialize, so we don't get garbage in unassigned variables.
	Point2D() :
		Point2D(0.f, 0.f)
	{ };

	Point2D(float x, float y) :
		X(x),
		Y(y)
	{ }

	//WG - No math libs mentioned in the test, easy to add the stuff I'm using though.
	//Note - there's no overflow protection in here.  I've previously worked with deterministic
	//fixed point math libs and so had to be careful of things like that.  They were good for platform
	//compatibility, but sucks for perf.

	Point2D operator/ (float denominator) const
	{
		float inverse = 1 / denominator;
		return Point2D(X * inverse, Y * inverse);
	}

	Point2D operator* (float product) const
	{
		return Point2D(X * product, Y * product);
	}

	Point2D operator- (Point2D difference) const
	{
		return Point2D(X - difference.X, Y - difference.Y);
	}

	Point2D operator+ (Point2D sum) const
	{
		return Point2D(X + sum.X, Y + sum.Y);
	}

	Point2D operator-() const
	{
		return Point2D(-X, -Y);
	}

	bool operator== (float compare)
	{
		return (X == compare) && (Y == compare);
	}

	bool operator== (Point2D compare)
	{
		return (X == compare.X) && (Y == compare.Y);
	}

	Point2D Perpendicular() const
	{
		return Point2D(Y, -X);
	}

	//WG - I assumed the orientation of the rect was normalized...
	//to be safe I force normalize it when creating a rect,
	//but I could actually ditch both the length & lengthsqr if
	//I didn't need the vector normalization or passed the buck 
	//on to the user of the api.

	Point2D Normalize() const
	{
		return Point2D(X, Y) / Length();
	}

	float LengthSqr() const
	{
		return (X * X) + (Y * Y);
	}

	float Length() const
	{
		//Needed the sqrt... 
		return sqrtf(LengthSqr());
	}
};
#include "OrientedRectTests.h"

#include "OrientedRect2D.h"
#include "Point2D.h"
#include "MathHelpers.h"

void OrientedRectTests::RunTests()
{
	//WG - I initially went down this crazy route of projecting points onto lines and checking ranges...
	//but wanted to get rid of the sqrt from the length calculations.  Also I felt like I was inheriting too
	//many math libs.  Sometimes you run into this artificial barrier in these tests where you know the
	//library will provide you with all the tools, but the point of the test is to demonstrate that you
	//know how they work & can design the algorithm yourself.  After doing a ton of line math & simplification
	//and still seeing unit tests failing, I took a break because I was feeling pretty brain mushy and
	//wasn't making progress on understanding why.

	//I ended up sleeping on this problem for a couple of days and then came back with this current solution, which
	//I'm much happier with.  It's way more efficient and elegant that what I had before.  To be honest
	//I've never had to implement these cases as the engines I've worked with already have robust physics libraries
	//but I'm glad I went through the motions. 

	OrientedRect2D DiagonalCentered(Point2D(-1.0f, 0.0f), Point2D(1.0f, 1.0f), Point2D(2.0f, 2.0f));

	TestCondition(MathHelpers::Contains(DiagonalCentered, Point2D()), "Point inside at origin");
	TestCondition(!MathHelpers::Contains(DiagonalCentered, Point2D(-1.0f, -1.0f)), "Point outside at -1,-1");
	TestCondition(MathHelpers::Contains(DiagonalCentered, Point2D(-1.0f, 0.0f)), "Inclusive bounds using position");

	OrientedRect2D OriginCenteredVertical(Point2D(0.0f, 0.0f), Point2D(1.0f, 0.0f), Point2D(2.0f, 2.0f));
	OrientedRect2D OverlappingVertical(Point2D(2.0f, 0.0f), Point2D(-1.0f, 0.0f), Point2D(5.0f, 1.0f));
	OrientedRect2D Encompassing(Point2D(-2.0f, -2.0f), Point2D(0.0f, 1.0f), Point2D(4.0f, 4.0f));
	OrientedRect2D Engulfed(Point2D(0.1f, 0.1f), Point2D(0.0f, -1.0f), Point2D(0.1f, 0.1f));
	OrientedRect2D Outside(Point2D(-0.6f, 0.9f), Point2D(-1.0f, 1.0f), Point2D(5.0f, 5.0f));

	TestCondition(MathHelpers::Intersects(DiagonalCentered, OriginCenteredVertical), "Single point inside bounds");
	TestCondition(MathHelpers::Intersects(DiagonalCentered, OverlappingVertical), "Intersecting area");
	TestCondition(MathHelpers::Intersects(DiagonalCentered, Encompassing), "Completely encompassing");
	TestCondition(MathHelpers::Intersects(DiagonalCentered, Engulfed), "Completely engulfed");
	TestCondition(!MathHelpers::Intersects(DiagonalCentered, Outside), "Non-intersecting");
}
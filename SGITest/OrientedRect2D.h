#pragma once

#include "Point2D.h"

//WG - The test said to create a class, but classes felt wrong for representing rectangles.
//I'm pretty sure this could be directly converted anyways with the keyword change.

struct OrientedRect2D
{
// WG - I know structs default to a public scope, but I usually find it better to be explicit.
public:
	Point2D Position;
	Point2D OrientationUnitVector;
	Point2D Size;

	OrientedRect2D() :
		Position(),
		OrientationUnitVector(),
		Size()
	{ };

	OrientedRect2D(Point2D position, Point2D orientationUnitVector, Point2D size) :
		Position(position),
		OrientationUnitVector(orientationUnitVector.Normalize()),
		Size(size)
	{ };

	OrientedRect2D(const OrientedRect2D& CopyRect) :
		OrientedRect2D(CopyRect.Position, CopyRect.OrientationUnitVector, CopyRect.Size)
	{ };

	//WG - The naming is the best for these functions.
	//They basically represent the offset from the position of the rect
	//to the 3 other points.
	Point2D CartesianLength() const
	{
		return (OrientationUnitVector * Size.X);
	}

	Point2D CartesianWidth() const
	{
		return (OrientationUnitVector.Perpendicular() * Size.Y);
	}

	Point2D CartesianHypotenuse() const
	{
		return CartesianLength() + CartesianWidth();
	}
};
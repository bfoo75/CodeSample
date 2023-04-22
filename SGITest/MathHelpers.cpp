#include "MathHelpers.h"

#include "Point2D.h"
#include "OrientedRect2D.h"

#include <math.h>

#define INCLUSIVE true

namespace MathHelpers
{
	//WG - This would actually be REALLY easy to extend to include parallelograms...
	//or I guess any quad depending on how many normals you wanted to provide.
	bool Contains(const OrientedRect2D& boundingRect, const Point2D& pointToCheck)
	{
		const Point2D& normal = boundingRect.OrientationUnitVector;
		Point2D diff = pointToCheck - boundingRect.Position;
		if (!IsSameDirection(normal, diff))
		{
			return false;
		}

		const Point2D& perpendicular = normal.Perpendicular();
		if (!IsSameDirection(perpendicular, diff))
		{
			return false;
		}

		diff = diff - boundingRect.CartesianHypotenuse();
		if (!IsSameDirection(-normal, diff) || !IsSameDirection(-perpendicular, diff))
		{
			return false;
		}

		return true;
	}

	//WG - Returns if point b is located with the 180 degree forward facing arc of point a... assuming a is at zero
	//and b is in cartesian coordinates relative to a.  Basically a dot product, only you don't care how far
	//away b is from a.
	bool IsSameDirection(const Point2D& a, const Point2D& b)
	{
		bool sameXHalf = signbit(a.X) == signbit(b.X);
		bool sameYHalf = signbit(a.Y) == signbit(b.Y);

		if (sameXHalf == sameYHalf)
		{
			//WG - If b is in the same quadrant as the unit vector, the dot will be positive.
			//WG - If b is in the opposite quadrant as the unit vector, the dot will be negative.
			return sameXHalf;
		}

		//Get the dot.
		float dot = a.X * b.X + a.Y * b.Y;
		if (dot == 0.f)
		{
			//WG - A perpendicular dot puts point b right on the edge of being in the same direction...
			//This would ultimately determine if b was on the bounds of a rect when doing the distance check.
			return INCLUSIVE;
		}

		return !signbit(dot);
	}

	//WG - Returns a visually identical rectangle, only the orientation slope is ensured to be positive & 
	//the position is adjusted to always be the left most point on the quad.  I couldn't really think of a better
	//name that 'allineate'.  I had toyed around with normalize, ordinate, regulate...  allineate seemed to be the
	//most accurate as it describes the act of aligning.  I dunno if I'd use this in a library though, the term seems
	//a little obscure for coders to recognize.
	OrientedRect2D Allineate(const OrientedRect2D& rect)
	{
		OrientedRect2D regulatedRect = rect;

		Point2D& regulatedOrientation = regulatedRect.OrientationUnitVector;
		Point2D& regulatedPosition = regulatedRect.Position;

		//WG - A positive unit vector along an axis will incorrectly identify the perpendicular
		//axis as being inside the same quadrant. This is due to 0 arbitrarily skirting
		//the boundary of negative and positive.  The explicit 1.f checks redistribute the
		//perpendicular orientations to their correct clockwise quadrant.

		//WG - I think there's actually a bug in here with the negative y check... 
		bool negX = signbit(regulatedOrientation.X) || regulatedOrientation.Y == -1.f;
		bool negY = signbit(regulatedOrientation.Y) || regulatedOrientation.X == -1.f;

		if (negX)
		{
			regulatedPosition = regulatedPosition + rect.CartesianLength();
		}

		if (negY)
		{
			regulatedPosition = regulatedPosition + rect.CartesianWidth();
			regulatedOrientation = -regulatedOrientation;
		}

		if (negX != negY)
		{
			regulatedOrientation = regulatedOrientation.Perpendicular();
		}
		
		return regulatedRect;
	}

	//WG - Checks if the left most 90 degree corner from a encloses the right most point on b.
	bool OpposingCornersFormSubrect(const OrientedRect2D& a, const OrientedRect2D& b)
	{
		const Point2D& orientation = a.OrientationUnitVector;
		Point2D diff = (b.Position + b.CartesianHypotenuse()) - a.Position;

		return IsSameDirection(orientation, diff)
			&& IsSameDirection(orientation.Perpendicular(), diff);
	}

	//WG - First makes sure the rects are both aligned to the same orientation so I can get the
	//opposing points on each rect.  Then checks that the opposing & facing corners form a rectangle.
	bool Intersects(const OrientedRect2D& a, const OrientedRect2D& b)
	{
		OrientedRect2D regulatedA = Allineate(a);
		OrientedRect2D regulatedB = Allineate(b);

		return OpposingCornersFormSubrect(regulatedA, regulatedB) && OpposingCornersFormSubrect(regulatedB, regulatedA);
	}
}

#undef INCLUSIVE
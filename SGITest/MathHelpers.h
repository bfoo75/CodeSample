#pragma once

struct OrientedRect2D;
struct Point2D;

namespace MathHelpers
{
	bool Contains(const OrientedRect2D& boundingRect, const Point2D& pointToCheck);
	bool Intersects(const OrientedRect2D& a, const OrientedRect2D& b);

	bool OpposingCornersFormSubrect(const OrientedRect2D& a, const OrientedRect2D& b);
	OrientedRect2D Allineate(const OrientedRect2D& rect);
	bool IsSameDirection(const Point2D& a, const Point2D& b);
}
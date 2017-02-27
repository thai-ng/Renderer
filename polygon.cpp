#include "primitives.h"

Polygon::Polygon(std::vector<Point>&& v, Rect* parent) : _vertices(v), _parent(parent)
{
	std::for_each(_vertices.begin(), _vertices.end(), [parent](Point& p) {p.parent = parent; });
}

std::vector<Point> Polygon::vertices() const
{
	return _vertices;
}

const Rect* Polygon::parent() const
{
	return _parent;
}

#include "primitives.h"

Triangle::Triangle(const std::array<Point, 3>& vertices, const Rect* parent) : _vertices(vertices), _parent(parent)
{
	std::for_each(_vertices.begin(), _vertices.end(), [parent](auto& vertex) {vertex.parent = parent; });
}

Triangle::Triangle(const Line& line1, const Line& line2)
{
	if (line1.connected(line2))
	{
		_vertices = line1.getTriangleVertices(line2);
		_parent = line1.p1.parent;
	}
	else
	{
		throw std::invalid_argument("Lines must connect.");
	}
}

std::array<Point, 3> Triangle::vertices() const
{
	return _vertices;
}

const Rect* Triangle::parent() const
{
	return _parent;
}
#include "PointGenerator.hpp"

#include <algorithm>
#include <cmath>

#include "lerp.hpp"

namespace PointGenerator
{


	// Position helpers
	enum class Octant
	{
		First,
		Second,
		Third,
		Fourth,
		Fifth,
		Sixth,
		Seventh,
		Eighth
	};

	Point toFirstOctant(Octant octant, Point p)
	{
		switch (octant)
		{
		case Octant::First:
		{
			return Point{ p.x, p.y, p.z, p.parent, p.color };
		} break;

		case Octant::Second:
		{
			return Point{ p.y, p.x, p.z, p.parent, p.color };
		} break;

		case Octant::Third:
		{
			return Point{ p.y, -p.x, p.z, p.parent, p.color };
		} break;

		case Octant::Fourth:
		{
			return Point{ -p.x, p.y, p.z, p.parent, p.color };
		} break;

		case Octant::Fifth:
		{
			return Point{ -p.x, -p.y, p.z, p.parent, p.color };
		} break;

		case Octant::Sixth:
		{
			return Point{ -p.y, -p.x, p.z, p.parent, p.color };
		} break;

		case Octant::Seventh:
		{
			return Point{ -p.y, p.x, p.z, p.parent, p.color };
		} break;

		case Octant::Eighth:
		{
			return Point{ p.x, -p.y, p.z, p.parent, p.color };
		} break;

		default:
		{
			throw std::invalid_argument("Invalid octant");
		}
		}
	}

	Point fromFirstOctant(Octant octant, Point p)
	{
		switch (octant)
		{
		case Octant::First:
		{
			return Point{ p.x, p.y, p.z, p.parent, p.color };
		} break;

		case Octant::Second:
		{
			return Point{ p.y, p.x, p.z, p.parent, p.color };
		} break;

		case Octant::Third:
		{
			return Point{ -p.y, p.x, p.z, p.parent, p.color };
		} break;

		case Octant::Fourth:
		{
			return Point{ -p.x, p.y, p.z, p.parent, p.color };
		} break;

		case Octant::Fifth:
		{
			return Point{ -p.x, -p.y, p.z, p.parent, p.color };
		} break;

		case Octant::Sixth:
		{
			return Point{ -p.y, -p.x, p.z, p.parent, p.color };
		} break;

		case Octant::Seventh:
		{
			return Point{ p.y, -p.x, p.z, p.parent, p.color };
		} break;

		case Octant::Eighth:
		{
			return Point{ p.x, -p.y, p.z, p.parent, p.color };
		} break;

		default:
		{
			throw std::invalid_argument("Invalid octant");
		}
		}
	}

	Octant getOctant(Point p)
	{
		auto aboveX = p.y > 0;
		auto rightY = p.x > 0;
		auto aboveDiag = std::abs(p.x) < std::abs(p.y);

		if (aboveX && rightY && !aboveDiag)
		{
			return Octant::First;
		}
		else if (aboveX && rightY && aboveDiag)
		{
			return Octant::Second;
		}
		else if (aboveX && !rightY && aboveDiag)
		{
			return Octant::Third;
		}
		else if (aboveX && !rightY && !aboveDiag)
		{
			return Octant::Fourth;
		}
		else if (!aboveX && !rightY && !aboveDiag)
		{
			return Octant::Fifth;
		}
		else if (!aboveX && !rightY && aboveDiag)
		{
			return Octant::Sixth;
		}
		else if (!aboveX && rightY && aboveDiag)
		{
			return Octant::Seventh;
		}
		else if (!aboveX && rightY && !aboveDiag)
		{
			return Octant::Eighth;
		}
		else
		{
			throw std::invalid_argument("Invalid octant");
		}
	}

	// Color helpers
	std::tuple<Lerp<int>, Lerp<int>, Lerp<int>> getColorLerp(const Point& point1, const Point& point2)
	{
		auto p1ColorChannels = point1.color.getColorChannels();
		auto p1Red = std::get<0>(p1ColorChannels);
		auto p1Green = std::get<1>(p1ColorChannels);
		auto p1Blue = std::get<2>(p1ColorChannels);

		auto p2ColorChannels = point2.color.getColorChannels();
		auto p2Red = std::get<0>(p2ColorChannels);
		auto p2Green = std::get<1>(p2ColorChannels);
		auto p2Blue = std::get<2>(p2ColorChannels);


		Lerp<int> redLerp(point1.x, point2.x, p1Red, p2Red);
		Lerp<int> greenLerp(point1.x, point2.x, p1Green, p2Green);
		Lerp<int> blueLerp(point1.x, point2.x, p1Blue, p2Blue);


		return std::make_tuple(redLerp, greenLerp, blueLerp);
	}

	Color getColorFromLerp(int colorIndex, const std::tuple<Lerp<int>, Lerp<int>, Lerp<int>>& colorLerps)
	{
		auto redLerp = std::get<0>(colorLerps);
		auto greenLerp = std::get<1>(colorLerps);
		auto blueLerp = std::get<2>(colorLerps);

		auto r = static_cast<unsigned char>(redLerp[colorIndex].second);
		auto g = static_cast<unsigned char>(greenLerp[colorIndex].second);
		auto b = static_cast<unsigned char>(blueLerp[colorIndex].second);

		return Color{ r, g, b };
	}

	std::vector<Point> generateLinePoints(const Point& p1, const Point & p2)
	{
		auto octant = getOctant(p2 - p1);
		auto point1 = toFirstOctant(octant, p1);
		auto point2 = toFirstOctant(octant, p2);

		auto colorLerps = getColorLerp(point1, point2);
		Lerp<decltype(point1.x)> posLerp(point1.x, point2.x, point1.y, point2.y);
		Lerp<decltype(point1.x)> zLerp(point1.x, point2.x, point1.z, point2.z);

		std::vector<Point> result;
		result.reserve(posLerp.size());
		auto currentIndex = 0;
		std::generate_n(std::back_inserter(result), 
						posLerp.size(),
						[&posLerp, &zLerp, &colorLerps, &currentIndex, octant]
						{
							auto point = posLerp[currentIndex];
							auto x = point.first;
							auto y = point.second;
							auto z = zLerp[currentIndex].second;
							auto newColor = getColorFromLerp(currentIndex, colorLerps);
							auto screenPoint = fromFirstOctant(octant, Point{ x, static_cast<int>(std::round(y)), static_cast<int>(std::round(z)), nullptr, newColor });
							++currentIndex;
							return screenPoint;
						});

		return result;
	}

	// Polygon helpers

	auto comparePoints = [](auto p1, auto p2)
	{
		if (p1.y == p2.y)
		{
			return p1.x < p2.x;
		}
		else
		{
			return p1.y < p2.y;
		}
	};

	double getXSlope(Line line)
	{
		// -
		if (line.p1.y == line.p2.y)
		{
			return 0.0;
		}
		else
		{
			// |
			if (line.p1.x == line.p2.x)
			{
				return 0.0;
			}
			else
			{
				return static_cast<double>(line.p2.x - line.p1.x) / static_cast<double>(line.p2.y - line.p1.y);
			}
		}
	}

	template <typename T, typename F>
	T sortVertices(const T& vertices, F comp)
	{
		T sortedVertices{ vertices };
		std::sort(sortedVertices.begin(), sortedVertices.end(), comp);
		return sortedVertices;
	}

	template<typename T, typename F>
	std::tuple<T, T> splitVertices(const T& vertices, F sortComp)
	{
		std::vector<Point> leftChain = { vertices.front(), vertices.back() };
		std::vector<Point> rightChain = { vertices.front(), vertices.back() };

		auto centerX = 0;
		for (auto point : vertices)
		{
			centerX += point.x;
		}
		centerX /= static_cast<int>(vertices.size());

		auto endIt = std::prev(vertices.end());
		for (auto currentIt = std::next(vertices.begin()); currentIt != endIt; currentIt = std::next(currentIt))
		{
			auto currentPoint = *currentIt;
			if (currentPoint.x <= centerX)
			{
				leftChain.push_back(currentPoint);
			}
			else
			{
				rightChain.push_back(currentPoint);
			}
		}

		std::sort(leftChain.begin(), leftChain.end(), sortComp);
		std::sort(rightChain.begin(), rightChain.end(), sortComp);
		return std::make_tuple(leftChain, rightChain);
	}

	std::vector<Point> generatePolygonPoints(const std::vector<Point>& points)
	{
		auto sortedVertices = sortVertices(points, comparePoints);
		auto topPoint = sortedVertices.front();
		auto bottomPoint = sortedVertices.back();

		auto vertexChains = splitVertices(sortedVertices, comparePoints);

		// Left line chain
		auto leftChain = std::get<0>(vertexChains);
		auto leftIter = std::next(leftChain.begin());
		Line leftLine{ leftChain.front(), *leftIter };
		Lerp<int> leftLerp(leftLine.p1.y, leftLine.p2.y, leftLine.p1.x, leftLine.p2.x);
		Lerp<int> leftZLerp(leftLine.p1.y, leftLine.p2.y, leftLine.p1.z, leftLine.p2.z);

		auto leftColorLerps = getColorLerp(leftLine.p1.flipped(), leftLine.p2.flipped());
		auto leftRedLerp = std::get<0>(leftColorLerps);
		auto leftGreenLerp = std::get<1>(leftColorLerps);
		auto leftBlueLerp = std::get<2>(leftColorLerps);

		auto leftCount = 0;

		// Right line chain
		auto rightChain = std::get<1>(vertexChains);
		auto rightIter = std::next(rightChain.begin());
		Line rightLine{ rightChain.front(), *rightIter };
		Lerp<int> rightLerp(rightLine.p1.y, rightLine.p2.y, rightLine.p1.x, rightLine.p2.x);
		Lerp<int> rightZLerp(rightLine.p1.y, rightLine.p2.y, rightLine.p1.z, rightLine.p2.z);

		auto rightColorLerps = getColorLerp(rightLine.p1.flipped(), rightLine.p2.flipped());
		auto rightRedLerp = std::get<0>(rightColorLerps);
		auto rightGreenLerp = std::get<1>(rightColorLerps);
		auto rightBlueLerp = std::get<2>(rightColorLerps);

		auto rightCount = 0;

		std::vector<Point> result;

		for (auto y = topPoint.y; y <= bottomPoint.y; ++y)
		{
			auto xl = leftLerp[leftCount].second;
			auto zl = leftZLerp[leftCount].second;
			auto r = static_cast<unsigned char>(leftRedLerp[leftCount].second);
			auto g = static_cast<unsigned char>(leftGreenLerp[leftCount].second);
			auto b = static_cast<unsigned char>(leftBlueLerp[leftCount].second);
			++leftCount;
			auto leftColor = Color(r, g, b);

			auto xr = rightLerp[rightCount].second;
			auto zr = rightZLerp[rightCount].second;

			r = static_cast<unsigned char>(rightRedLerp[rightCount].second);
			g = static_cast<unsigned char>(rightGreenLerp[rightCount].second);
			b = static_cast<unsigned char>(rightBlueLerp[rightCount].second);
			++rightCount;
			auto rightColor = Color(r, g, b);

			auto leftPoint = Point{ static_cast<int>(std::round(xl)), y, static_cast<int>(std::round(zl)), topPoint.parent, leftColor };
			auto rightPoint = Point{ static_cast<int>(std::round(xr)), y,  static_cast<int>(std::round(zr)), topPoint.parent, rightColor };

			auto linePoints = generateLinePoints(leftPoint, rightPoint);
			result.insert(result.end(), linePoints.begin(), linePoints.end());

			if (y >= leftLine.p2.y && leftLine.p2 != bottomPoint)
			{
				leftIter = std::next(leftIter);
				leftLine = Line{ leftLine.p2, *leftIter };
				leftLerp = Lerp<int>(leftLine.p1.y, leftLine.p2.y, leftLine.p1.x, leftLine.p2.x);
				leftZLerp = Lerp<int>(leftLine.p1.y, leftLine.p2.y, leftLine.p1.z, leftLine.p2.z);

				leftColorLerps = getColorLerp(leftLine.p1.flipped(), leftLine.p2.flipped());
				leftRedLerp = std::get<0>(leftColorLerps);
				leftGreenLerp = std::get<1>(leftColorLerps);
				leftBlueLerp = std::get<2>(leftColorLerps);

				leftCount = 0;
			}

			if (y >= rightLine.p2.y && rightLine.p2 != bottomPoint)
			{
				rightIter = std::next(rightIter);
				rightLine = Line{ rightLine.p2, *rightIter };
				rightLerp = Lerp<int>(rightLine.p1.y, rightLine.p2.y, rightLine.p1.x, rightLine.p2.x);
				rightZLerp = Lerp<int>(rightLine.p1.y, rightLine.p2.y, rightLine.p1.z, rightLine.p2.z);


				rightColorLerps = getColorLerp(rightLine.p1.flipped(), rightLine.p2.flipped());
				rightRedLerp = std::get<0>(rightColorLerps);
				rightGreenLerp = std::get<1>(rightColorLerps);
				rightBlueLerp = std::get<2>(rightColorLerps);
				rightCount = 0;
			}
		}

		return result;
	}

	std::vector<Point> generateWireframePoints(const std::vector<Point>& points)
	{
		auto sortedVertices = sortVertices(points, comparePoints);
		auto vertexChains = splitVertices(sortedVertices, comparePoints);

		auto leftChain = std::get<0>(vertexChains);
		auto currentPointLeft = *(leftChain.begin());

		std::vector<Point> result;

		std::for_each(std::next(leftChain.begin()), leftChain.end(), [&currentPointLeft, &result](auto point)
		{
			auto linePoints = generateLinePoints(currentPointLeft, point);
			result.insert(result.end(), linePoints.begin(), linePoints.end());
			currentPointLeft = point;
		});

		auto rightChain = std::get<1>(vertexChains);
		auto currentPointRight = *(rightChain.begin());
		std::for_each(std::next(rightChain.begin()), rightChain.end(), [&currentPointRight, &result](auto point)
		{
			auto linePoints = generateLinePoints(currentPointRight, point);
			result.insert(result.end(), linePoints.begin(), linePoints.end());
			currentPointRight = point;
		});

		return result;
	}
}

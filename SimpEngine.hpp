#pragma once

#include <stack>
#include <vector>

#include "Matrix.h"
#include "command.h"
#include "RenderingEngine.hpp"

class SimpEngine
{
	using CTM_t = Matrix<4, 4, double>;

	void runCommands(const std::vector<Command>& commands)
	{
		for (auto& command : commands)
		{
			switch (command.operation())
			{
				case Operation::Filled:
				{
					currentRenderMode = RenderingEngine::RenderMode::Filled;
				} break;

				case Operation::Wire:
				{
					currentRenderMode = RenderingEngine::RenderMode::Wireframe;
				} break;

				case Operation::OpenBrace:
				{
					TransformStack.push(std::move(CTM));
					CTM = CTM_t{ 1, 0, 0, 0,
								 0, 1, 0, 0,
						         0, 0, 1, 0,
								 0, 0, 0, 1 };
				} break;

				case Operation::CloseBrace:
				{
					if (!TransformStack.empty())
					{
						CTM = std::move(TransformStack.top());
						TransformStack.pop();
					}
				} break;

				case Operation::Scale:
				{
					auto params = std::get<Vector3>(command.parameters());
					auto scaleMatrix = CTM_t{ params[0], 0,			0,		    0, 	
										      0,         params[1], 0,			0,
										      0,		 0,			params[2],  0,
											  0,		 0,			0		 ,  1};
					CTM = scaleMatrix * CTM;
				} break;

				case Operation::Translate:
				{
					auto params = std::get<Vector3>(command.parameters());
					auto translationMatrix = CTM_t{ 1, 0, 0, params[0],
										            0, 1, 0, params[1],
										            0, 0, 1, params[2],
											        0, 0, 0, 1};
					CTM = translationMatrix * CTM;
				} break;

				case Operation::Rotate:
				{
					auto params = std::get<RotateParams>(command.parameters());
					auto rotationMatrix = getRotationMatrix(params.first, params.second);

					CTM = rotationMatrix * CTM;
				}

				case Operation::Line:
				{
					auto params = std::get<LineParams>(command.parameters());
					auto point1 = std::array<double, 4>{ params[0][0], params[0][1], params[0][2], 1 };
					auto point2 = std::array<double, 4>{ params[1][0], params[1][1], params[1][2], 1 };
					auto transformedPoint1 = CTM * point1;
					auto transformedPoint2 = CTM * point2;

					// Send to rendering engine to render
				} break;

				case Operation::Polygon:
				{
					auto params = std::get<PolygonParams>(command.parameters());
					auto point1 = std::array<double, 4>{ params[0][0], params[0][1], params[0][2], 1 };
					auto point2 = std::array<double, 4>{ params[1][0], params[1][1], params[1][2], 1 };
					auto point3 = std::array<double, 4>{ params[2][0], params[2][1], params[2][2], 1 };
					auto transformedPoint1 = CTM * point1;
					auto transformedPoint2 = CTM * point2;
					auto transformedPoint3 = CTM * point2;

					// Send to rendering engine to render
				}
			}
		}
	}
private:

	auto getRotationMatrix(const Axis& axis, int degree) const
	{
		auto radian = getRadianFromDegree(degree);
		switch (axis)
		{
			case Axis::X:
			{
				return CTM_t{ 1, 0,				   0,				  0,
							  0, std::cos(radian), -std::sin(radian), 0,
							  0, std::sin(radian), std::cos(radian),  0,
							  0, 0,				   0,				  1 };
			} break;

			case Axis::Y:
			{
				return CTM_t{ std::cos(radian),	 0, std::sin(radian), 0,
							  0,				 1, 0,				  0,
							  -std::sin(radian), 0, std::cos(radian), 0,
							  0,				 0,	0,				  1 };
			} break;
			
			case Axis::Z:
			default:
			{
				return CTM_t{ std::cos(radian), -std::sin(radian), 0, 0,
							  std::sin(radian), std::cos(radian),  0, 0,
							  0,				0,				   0, 0,
							  0,				0,				   0, 1 };
			} break;
		}
	}

	RenderingEngine::RenderMode currentRenderMode = RenderingEngine::RenderMode::Filled;
	CTM_t CTM;
	std::stack<CTM_t> TransformStack;
};
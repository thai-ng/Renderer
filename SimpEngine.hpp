#pragma once

#include <stack>
#include <vector>

#include "Matrix.h"
#include "command.h"
#include "RenderingEngine.hpp"

class SimpEngine
{
	using CTM_t = Matrix<3, 3, double>;
	SimpEngine(const std::vector<Command>& commands)
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
					CTM = CTM_t{ 1, 0, 0, 
								 0, 1, 0, 
						         0, 0, 1 };
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
					auto params = std::get<Vector3>(command.paramerters());
					auto scaleMatrix = CTM_t{ params[0], 0, 0,
										      0, params[1], 0,
										      0, 0, params[2] };
				} break;
			}
		}
	}
private:
	RenderingEngine::RenderMode currentRenderMode = RenderingEngine::RenderMode::Filled;
	CTM_t CTM;
	std::stack<CTM_t> TransformStack;
};
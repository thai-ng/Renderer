#pragma once

#include <stack>
#include <vector>

#include "command.hpp"
#include "Matrix.hpp"
#include "RenderingEngine.hpp"

class SimpEngine
{
public:
	using CTM_t = Matrix<4, 4, double>;
	SimpEngine(RenderEngine renderEngine) : _renderEngine(renderEngine) {}

	void runCommands(const std::vector<Command>& commands);
private:

	CTM_t getRotationMatrix(const Axis& axis, int degree) const;

	RenderEngine _renderEngine;
	RenderEngine::RenderMode currentRenderMode = RenderEngine::RenderMode::Filled;
	CTM_t CTM = CTM_t{ 1.0, 0.0, 0.0, 0.0,
					   0.0, 1.0, 0.0, 0.0,
					   0.0, 0.0, 1.0, 0.0,
					   0.0, 0.0, 0.0, 1.0 };

	std::stack<CTM_t> TransformStack;
};
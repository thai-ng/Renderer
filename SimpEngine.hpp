#pragma once

#include <stack>
#include <vector>

#include "CommonTypeAliases.hpp"
#include "command.hpp"
#include "RenderingEngine.hpp"

class SimpEngine
{
public:
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
	std::vector<Point4D> vertices;
	std::stack<std::vector<Point4D>> objFileVerticesStack;
};
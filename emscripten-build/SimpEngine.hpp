#pragma once

#include <stack>
#include <vector>

#include "CommonTypeAliases.hpp"
#include "command.hpp"
#include "RenderingEngine.hpp"
#include "Vertex.hpp"
#include "Face.hpp"

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


	CTM_t cameraCTMInv = CTM_t{ 1.0, 0.0, 0.0, 0.0,
								0.0, 1.0, 0.0, 0.0,
								0.0, 0.0, 1.0, 0.0,
								0.0, 0.0, 0.0, 1.0 };

	std::stack<CTM_t> TransformStack;

	std::vector<Vertex> vertices;
	std::stack<std::vector<Vertex>> objFileVerticesStack;

	std::vector<Face> faces;
	std::stack<std::vector<Face>> objFileFacesStack;

	std::vector<Point> normals;
	std::stack<std::vector<Point>> objFileNormalsStack;
};
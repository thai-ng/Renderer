#include "RenderingEngine.hpp"
#include "Matrix.hpp"
#include "PointGenerator.hpp"
#include "PointsRenderer.hpp"
#include "PointLighter.hpp"

//#include "lineRenderer.hpp"
//#include "polygonRenderer.hpp"

RenderEngine::RenderEngine(const Rect& viewPort, Drawable* drawSurface, const Color& maxColor) :
	_viewPort(viewPort),
	_drawSurface(drawSurface),
	redLerp(0, 200, std::get<0>(maxColor.getColorChannels()), 0),
	greenLerp(0, 200, std::get<1>(maxColor.getColorChannels()), 0),
	blueLerp(0, 200, std::get<2>(maxColor.getColorChannels()), 0),
	ambientColor(maxColor)
{
	zBuffer = Matrix2D<double>(_viewPort.width, std::vector<double>(_viewPort.height, zThreshold));

	auto viewPlaneWidth = 200.0;
	auto viewPlaneHeight = 200.0;

	auto scaleMatrix = Matrix<4, 4, double>{ viewPort.width / viewPlaneWidth, 0.0,								  0.0, 0.0,
											 0.0,							  -viewPort.height / viewPlaneHeight, 0.0, 0.0,
											 0.0,							  0.0,								  1.0, 0.0,
											 0.0,							  0.0,								  0.0, 1.0 };

	auto translationMatrix = Matrix<4, 4, double>{ 1.0, 0.0, 0.0, viewPort.width / 2.0,
												   0.0, 1.0, 0.0, viewPort.height / 2.0,
												   0.0, 0.0, 1.0, 0.0,
												   0.0, 0.0, 0.0, 1.0 };

	viewPortTransformationMatrix = viewPortTransformationMatrix * translationMatrix;
	viewPortTransformationMatrix = viewPortTransformationMatrix * scaleMatrix;
}

Point getFaceNormal(Polygon_t &cameraVertices)
{
	Plane_t face = { cameraVertices[0], cameraVertices[1], cameraVertices[2] };
	auto normal4D = normalize(getNormal(face));
	return Point{ normal4D.x, normal4D.y, normal4D.z };
}

void RenderEngine::RenderTriangle(const Polygon_t& triangle, RenderMode renderMode)
{
	if (std::all_of(triangle.begin(), triangle.end(), [this](auto& p) {return p.z >= _camera.near; }))
	{
		// Points in camera space
		// Clip to near (far)
		
		// Translate to screen space
		//auto cameraVertices = sortVertices(triangle);
		auto cameraVertices = triangle;
		std::vector<Point4D> vertices;
		vertices.resize(cameraVertices.size());
		std::transform(cameraVertices.begin(), cameraVertices.end(), vertices.begin(), [this](auto& p)
		{
			auto v = perspectiveTransformationMatrix * p.getVector();
			v = v / v[3];

			v = viewPortTransformationMatrix * v;

			if (p.normal.has_value())
			{
				return Point4D{ v[0], v[1], v[2], v[3], p.color, p.normal.value() };
			}
			else
			{
				return Point4D{ v[0], v[1], v[2], v[3], p.color };
			}
		});
		
		std::vector<Point4D> points;

		auto faceNormal = getFaceNormal(cameraVertices);
		auto centerPoint = getCenterPoint(cameraVertices);
		if (dot(normalize(centerPoint), faceNormal) > 0)
			return;

		// Flat
		if (currentLightingMethod == LightingMethod::Flat)
		{
			Point normal;
			// If any vertice has no normal
			if (std::any_of(vertices.begin(), vertices.end(), [](auto p) { return !p.normal.has_value(); }))
			{
				// Calculate face normal
				normal = getFaceNormal(cameraVertices);
			}
			// Average normal
			else
			{
				auto normal4D = normalize((cameraVertices[0].normal.value() + cameraVertices[1].normal.value() + cameraVertices[2].normal.value()) / 3);
				normal = Point{ normal4D.x, normal4D.y, normal4D.z };
			}

			// Lighting at center
			centerPoint.normal = normal;
			auto color = PointLighter::calculateLights(centerPoint, lights, ks, p);
			for (auto& v : vertices)
			{
				v.color = v.color * ambientColor + color;
			}

			// Raster
			if (renderMode == RenderMode::Filled)
			{
				points = std::move(PointGenerator::generatePolygonPoints(vertices));
			}
			else
			{
				points = std::move(PointGenerator::generateWireframePoints(vertices));
			}
		}
		// Gouraud
		else if (currentLightingMethod == LightingMethod::Gouraud)
		{
			// If any vertice has no normal
			if (std::any_of(vertices.begin(), vertices.end(), [](auto p) { return !p.normal.has_value(); }))
			{
				// Calculate face normal
				auto normal = getFaceNormal(cameraVertices);
				// Set normal for each vertex
				for (auto& v : vertices)
				{
					v.normal.emplace(std::move(normal));
				}
			}

			// calculate light at each vertex
			for (auto index = 0u; index < vertices.size(); ++index)
			{
				auto cameraVertex = cameraVertices[index];
				cameraVertex.normal.emplace(vertices[index].normal.value());
				vertices[index].color = vertices[index].color * ambientColor + PointLighter::calculateLights(cameraVertex, lights, ks, p);
			}

			//for (auto& v : vertices)
			//{
			//	v.color = v.color * ambientColor + PointLighter::calculateLights(v, lights, ks, p);
			//}

			// Raster
			if (renderMode == RenderMode::Filled)
			{
				points = std::move(PointGenerator::generatePolygonPoints(vertices));
			}
			else
			{
				points = std::move(PointGenerator::generateWireframePoints(vertices));
			}

		}
		// Phong
		else
		{
			// If any vertice has no normal
			if (std::any_of(vertices.begin(), vertices.end(), [](auto p) { return !p.normal.has_value(); }))
			{
				// Calculate face normal
				auto normal = getFaceNormal(cameraVertices);
				// Set normal for each vertex
				for (auto& v : vertices)
				{
					v.normal.emplace(std::move(normal));
				}
			}

			// Save camera space points
			/*for (auto& v : vertices)
			{
				v.cameraSpacePoint.emplace( v.x, v.y, v.z );
			}*/

			for (auto index = 0u; index < vertices.size(); ++index)
			{
				auto cameraVertex = cameraVertices[index];
				vertices[index].cameraSpacePoint.emplace(cameraVertex.x, cameraVertex.y, cameraVertex.x);
			}

			if (renderMode == RenderMode::Filled)
			{
				points = std::move(PointGenerator::generatePolygonPoints(vertices));
			}
			else
			{
				points = std::move(PointGenerator::generateWireframePoints(vertices));
			}

			// calculate light at each vertex
			for (auto& v : points)
			{
				auto cameraPoint = Point4D(v.cameraSpacePoint.value());
				cameraPoint.normal = v.normal;
				v.color = v.color * ambientColor + PointLighter::calculateLights(cameraPoint, lights, ks, p);
			}
		}


		if (depthSet)
		{
			PointLighter::calculateDepthShading(points, _depth);
		}

		PointsRenderer::renderPoints(points, _drawSurface, zBuffer, _viewPort, _camera);
	}
}

void RenderEngine::RenderFace(const Face& face, RenderMode renderMode)
{
	// Basic culling
	if (std::all_of(face.vertices.begin(), face.vertices.end(), [this](auto& p) {return p->location.z >= _camera.near && p->location.z <= _camera.far; }))
	{
		// If center point dot face normal positive, cull
		std::vector<Point4D> cameraVertices;
		cameraVertices.resize(face.vertices.size());
		std::transform(face.vertices.begin(), face.vertices.end(), cameraVertices.begin(), [](auto& v) {return v->location; });
		
		auto centerPoint = getCenterPoint(cameraVertices);
		if (dot(normalize(centerPoint), face.normal) > 0)
			return;

		// Generate projected points
		std::vector<Point4D> projectedVertices;
		projectedVertices.resize(face.vertices.size());
		std::transform(face.vertices.begin(), face.vertices.end(), projectedVertices.begin(), [this](auto& p)
		{
			auto v = perspectiveTransformationMatrix * p->location.getVector();
			v = v / v[3];

			v = viewPortTransformationMatrix * v;

			if (p->assignedNormal.has_value())
			{
				return Point4D{ v[0], v[1], v[2], v[3], p->location.color, p->assignedNormal.value() };
			}
			else
			{
				return Point4D{ v[0], v[1], v[2], v[3], p->location.color };
			}
		});
		
		std::vector<Point4D> points;

		switch (currentLightingMethod)
		{
			// If flat
			case LightingMethod::Flat:
			{
				//		If no assigned normal, use face normal
				Point normal;
				if (std::any_of(face.vertices.begin(), face.vertices.end(), [](auto& v) {return !v->assignedNormal.has_value(); }))
				{
					normal = Point{ face.normal.x, face.normal.y, face.normal.z };
				}
				// Otherwise average assigned normals
				else
				{
					auto normal4D = normalize((face.vertices[0]->assignedNormal.value() + face.vertices[1]->assignedNormal.value() + face.vertices[2]->assignedNormal.value()) / 3);
					normal = Point{ normal4D.x, normal4D.y, normal4D.z };
				}
				// Assign face normal to center point, calculate lighting
				centerPoint.normal = (normal);
				auto color = PointLighter::calculateLights(centerPoint, lights, ks, p);

				// Assign lighting to vertices
				for (auto& v : projectedVertices)
				{
					v.color = v.color * ambientColor + color;
				}

				// Rasterize
				if (renderMode == RenderMode::Filled)
				{
					points = std::move(PointGenerator::generatePolygonPoints(projectedVertices));
				}
				else
				{
					points = std::move(PointGenerator::generateWireframePoints(projectedVertices));
				}
			} break;

			// if Phong/Gouraud
			case LightingMethod::Gouraud:
			case LightingMethod::Phong:
			{
				// If no assigned normal, average the face normals at vertex
				if (std::any_of(face.vertices.begin(), face.vertices.end(), [](auto& v) {return !v->assignedNormal.has_value(); }))
				{
					for (auto i = 0u; i < face.vertices.size(); ++i)
					{
						auto normal = std::accumulate(face.vertices[i]->faceNormals.begin(), face.vertices[i]->faceNormals.end(), Point4D{ 0.0, 0.0, 0.0, 1.0 });
						//normal = normal / static_cast<double>(face.vertices.size());
						normal = normalize(normal);
						cameraVertices[i].normal = Point{ normal.x, normal.y, normal.z };
					}
				}
				// otherwise take the assigned normal
				else
				{
					for (auto i = 0u; i < face.vertices.size(); ++i)
					{
						cameraVertices[i].normal = face.vertices[i]->assignedNormal.value();
					}
				}

				// If Gouraud
				if (currentLightingMethod == LightingMethod::Gouraud)
				{
					// 	Calculate lighting at each vertex
					for (auto i = 0u; i < cameraVertices.size(); ++i)
					{
						auto color = PointLighter::calculateLights(cameraVertices[i], lights, ks, p);
						projectedVertices[i].color = color;
					}
					// Rasterize
					if (renderMode == RenderMode::Filled)
					{
						points = std::move(PointGenerator::generatePolygonPoints(projectedVertices));
					}
					else
					{
						points = std::move(PointGenerator::generateWireframePoints(projectedVertices));
					}
				}

				// If Phong
				else
				{
					// Assign camera vertex and normal to projected vertex
					for (auto i = 0u; i < cameraVertices.size(); ++i)
					{
						projectedVertices[i].cameraSpacePoint.emplace(cameraVertices[i].x, cameraVertices[i].y, cameraVertices[i].z);
						projectedVertices[i].normal = cameraVertices[i].normal;
						projectedVertices[i].color = Color(0.0);
					}

					// 	Rasterize
					if (renderMode == RenderMode::Filled)
					{
						points = std::move(PointGenerator::generatePolygonPoints(projectedVertices));
						
						// 	Calculate lighting at each rasterized points
						for (auto& v : points)
						{
							if (v.normal.has_value())
							{
								auto cameraPoint = Point4D(v.cameraSpacePoint.value());
								cameraPoint.normal = v.normal;
								v.color = v.color * ambientColor + PointLighter::calculateLights(cameraPoint, lights, ks, p);
							}
						}
					}
					else
					{
						// 	Calculate lighting at each vertex
						for (auto i = 0u; i < cameraVertices.size(); ++i)
						{
							auto color = PointLighter::calculateLights(cameraVertices[i], lights, ks, p);
							projectedVertices[i].color = color;
						}
						points = std::move(PointGenerator::generateWireframePoints(projectedVertices));
					}
					
				}

			} break;
		}

		if (depthSet)
		{
			PointLighter::calculateDepthShading(points, _depth);
		}

		PointsRenderer::renderPoints(points, _drawSurface, zBuffer, _viewPort, _camera);
	}
}

void RenderEngine::RenderLine(const Line_t& line)
{
	std::vector<Point> vertices;
	vertices.resize(line.size());
	std::transform(line.begin(), line.end(), vertices.begin(), [this](auto& p)
	{
		auto v = perspectiveTransformationMatrix * p.getVector();
		v = viewPortTransformationMatrix * v;
		// HACK		
		if (v[3] != 0)
		{
			v = v / v[3];
		}
		else
		{
			v = v / _camera.near;
		}

		return Point{ v[0], v[1], v[2], &this->_viewPort, p.color };
	});

	auto points = std::move(PointGenerator::generateLinePoints(vertices[0], vertices[1]));
	points.erase(std::remove_if(points.begin(), points.end(), [this](auto& p) { return  p.z > this->_camera.far || p.z < this->_camera.near; }), points.end());
	PointLighter::calculateAmbientLight(points, ambientColor);

	if (depthSet)
	{
		PointLighter::calculateDepthShading(points, _depth);
	}

	PointsRenderer::renderPoints(points, _drawSurface, zBuffer, _viewPort, _camera);
}

void RenderEngine::SetAmbientColor(const Color& color)
{
	ambientColor = color;
}

void RenderEngine::SetCamera(const Camera& camera)
{
	_camera = camera;
	auto viewPlaneWidth = _camera.xHigh - _camera.xLow;
	auto viewPlaneHeight = _camera.yHigh - _camera.yLow;

	auto scaleMatrix = Matrix<4, 4, double>{ _viewPort.width / viewPlaneWidth, 0.0,								   0.0, 0.0,
											 0.0,							  -_viewPort.height / viewPlaneHeight, 0.0, 0.0,
											 0.0,							   0.0,								   1.0, 0.0,
											 0.0,							   0.0,								   0.0, 1.0 };

	auto translationMatrix = Matrix<4, 4, double>{ 1.0, 0.0, 0.0, _viewPort.width / 2.0,
												   0.0, 1.0, 0.0, _viewPort.height / 2.0,
												   0.0, 0.0, 1.0, 0.0,
												   0.0, 0.0, 0.0, 1.0 };

	viewPortTransformationMatrix = CTM_t{ 1.0, 0.0, 0.0, 0.0,
										  0.0, 1.0, 0.0, 0.0,
										  0.0, 0.0, 1.0, 0.0,
										  0.0, 0.0, 0.0, 1.0 };

	viewPortTransformationMatrix = viewPortTransformationMatrix * translationMatrix;
	viewPortTransformationMatrix = viewPortTransformationMatrix * scaleMatrix;

	for (auto& r : zBuffer)
	{
		for (auto& e : r)
		{
			e = static_cast<int>(std::round(_camera.far + 1));
		}
	}

	nearPlane = { Point4D{ _camera.xLow, _camera.yLow, _camera.near, 1.0 },
				  Point4D{ _camera.xHigh, _camera.yLow, _camera.near, 1.0 },
				  Point4D{ _camera.xHigh, _camera.yHigh, _camera.near, 1.0 } };
}

void RenderEngine::SetDepth(const Depth& depth)
{
	depthSet = true;
	_depth = depth;
}

void RenderEngine::AddLight(const Light& light)
{
	lights.push_back(light);
}

void RenderEngine::SetLightingMethod(const LightingMethod& lightingMethod)
{
	currentLightingMethod = lightingMethod;
}

void RenderEngine::SetSpecularCoefficient(double value)
{
	ks = value;
}

void RenderEngine::SetSpecularExponent(double value)
{
	p = value;
}

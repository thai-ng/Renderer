#include "SimpEngine.hpp"
#include "Light.hpp"

void SimpEngine::runCommands(const std::vector<Command>& commands)
{
	for (auto& command : commands)
	{
		switch (command.operation())
		{
			case Command::Operation::Filled:
			{
				currentRenderMode = RenderEngine::RenderMode::Filled;
			} break;

			case Command::Operation::Wire:
			{
				currentRenderMode = RenderEngine::RenderMode::Wireframe;
			} break;

			case Command::Operation::OpenBrace:
			{
				TransformStack.push(CTM);

			} break;

			case Command::Operation::CloseBrace:
			{
				if (!TransformStack.empty())
				{
					CTM = std::move(TransformStack.top());
					TransformStack.pop();
				}
			} break;

			case Command::Operation::Scale:
			{
				auto params = std::get<Vector3>(command.parameters());
				auto scaleMatrix = CTM_t{ params[0], 0.0,		0.0,	   0.0,
										  0.0,       params[1], 0.0,	   0.0,
										  0.0,		 0.0,		params[2], 0.0,
										  0.0,		 0.0,		0.0		 , 1.0 };
				CTM = CTM * scaleMatrix;
			} break;

			case Command::Operation::Translate:
			{
				auto params = std::get<Vector3>(command.parameters());
				auto translationMatrix = CTM_t{ 1.0, 0.0, 0.0, params[0],
												0.0, 1.0, 0.0, params[1],
												0.0, 0.0, 1.0, params[2],
												0.0, 0.0, 0.0, 1.0 };
				CTM = CTM * translationMatrix;
			} break;

			case Command::Operation::Rotate:
			{
				auto params = std::get<RotateParams>(command.parameters());
				auto rotationMatrix = getRotationMatrix(params.first, params.second);

				CTM = CTM * rotationMatrix;
			} break;

			case Command::Operation::Line:
			{
				auto params = std::get<LineParams>(command.parameters());
				auto point1 = std::array<double, 4>{ params[0].x, params[0].y, params[0].z, 1 };
				auto point2 = std::array<double, 4>{ params[1].x, params[1].y, params[1].z, 1 };
				auto transformedPoint1 = cameraCTMInv * (CTM * point1);
				auto transformedPoint2 = cameraCTMInv * (CTM * point2);

				// Send to rendering engine to render
				_renderEngine.RenderLine(Line_t { Point4D {transformedPoint1, params[0].color}, Point4D{transformedPoint2, params[1].color} });
			} break;

			case Command::Operation::Polygon:
			{
				auto params = std::get<PolygonParams>(command.parameters());
				auto point1 = std::array<double, 4>{ params[0].x, params[0].y, params[0].z, 1 };
				auto point2 = std::array<double, 4>{ params[1].x, params[1].y, params[1].z, 1 };
				auto point3 = std::array<double, 4>{ params[2].x, params[2].y, params[2].z, 1 };
				auto transformedPoint1 = cameraCTMInv * (CTM * point1);
				auto transformedPoint2 = cameraCTMInv * (CTM * point2);
				auto transformedPoint3 = cameraCTMInv * (CTM * point3);

				// Send to rendering engine to render
				_renderEngine.RenderTriangle(Polygon_t{ Point4D{transformedPoint1, params[0].color},
														Point4D{transformedPoint2, params[1].color},
														Point4D{transformedPoint3, params[2].color} },
											 currentRenderMode);
			} break;

			case Command::Operation::Ambient:
			{
				auto color = std::get<Color>(command.parameters());
				_renderEngine.SetAmbientColor(color);
			} break;

			case Command::Operation::Camera:
			{
				auto params = std::get<CameraParams>(command.parameters());
				cameraCTMInv = invert(CTM);
				auto camera = Camera{ cameraCTMInv, params.xLow, params.xHigh, params.yLow, params.yHigh, params.near, params.far };
				_renderEngine.SetCamera(camera);
			} break;

			case Command::Operation::Depth:
			{
				auto params = std::get<DepthParams>(command.parameters());
				_renderEngine.SetDepth(Depth{ params.near, params.far, params.color });
			} break;

			case Command::Operation::VertexNormal:
			{
				normals.emplace_back(std::get<Vector3>(command.parameters()));
			} break;

			case Command::Operation::Vertex:
			{
				vertices.push_back(std::get<Point4D>(command.parameters()));
			} break;

			case Command::Operation::Face:
			{
				FaceParam params = std::get<FaceParam>(command.parameters());
				std::vector<Point4D> faceVertices;
				faceVertices.resize(params.size());
				std::transform(params.begin(), params.end(), faceVertices.begin(), [this](auto& vertex) 
				{
					// Vertex
					Point4D v;
					if (vertex[0] > 0)
					{
						v =  this->vertices[vertex[0] - 1];
					}
					else
					{
						v = this->vertices[this->vertices.size() + vertex[0]];
					}

					v = CTM * v;
					v = cameraCTMInv * v;

					// Normal
					if (vertex[2] != 0)
					{
						Point normal;
						if (vertex[2] > 0)
						{
							normal = this->normals[vertex[2] - 1];
						}
						else
						{
							normal = this->normals[this->normals.size() + vertex[2]];
						}
						v.normal.emplace(normal);
					}

					return v;
				});

				if (faceVertices.size() > 3)
				{
					for (auto i = 1u; i < faceVertices.size() - 1; ++i)
					{
						std::vector<Point4D> triangle;
						triangle.push_back(faceVertices[0]);
						triangle.push_back(faceVertices[i]);
						triangle.push_back(faceVertices[i + 1]);
					
						_renderEngine.RenderTriangle(triangle, currentRenderMode);
					}
				}
				else
				{
					_renderEngine.RenderTriangle(faceVertices, currentRenderMode);
				}

			} break;

			case Command::Operation::ObjectFile:
			{
				if (std::get<std::string>(command.parameters()) == "ENDOFOBJECTFILE"s)
				{ 
					if (!objFileVerticesStack.empty())
					{
						vertices = objFileVerticesStack.top();
						objFileVerticesStack.pop();
					}

					if (!objFileNormalsStack.empty())
					{
						normals = objFileNormalsStack.top();
						objFileNormalsStack.pop();
					}
				}
				else
				{
					objFileVerticesStack.push(vertices);
					vertices.clear();

					objFileNormalsStack.push(normals);
					normals.clear();
				}
			} break;

			case Command::Operation::Surface:
			{
				auto params = std::get<SurfaceParams>(command.parameters());
				_renderEngine.SetSpecularCoefficient(params[0]);
				_renderEngine.SetSpecularExponent(params[1]);
			} break;

			case Command::Operation::Light:
			{
				auto params = std::get<LightParams>(command.parameters());
				auto lightColor = Color::getDenormalizedColor(params[0], params[1], params[2]);
				auto lightPosition = CTM * Vector4_t{ 0, 0, 0, 1 };
				lightPosition = cameraCTMInv * lightPosition;
				auto light = Light{ lightPosition, lightColor, params[3], params[4] };
				_renderEngine.AddLight(light);
			} break;

			case Command::Operation::Phong:
			case Command::Operation::Gouraud:
			case Command::Operation::Flat:
			{
				_renderEngine.SetLightingMethod(std::get<LightingMethod>(command.parameters()));
			} break;
		}
	}
}

CTM_t SimpEngine::getRotationMatrix(const Axis& axis, int degree) const
{
	auto radian = -getRadianFromDegree(degree);
	switch (axis)
	{
		case Axis::X:
		{
			return CTM_t{ 1.0, 0.0,				 0.0,				0.0,
						  0.0, std::cos(radian), -std::sin(radian), 0.0,
						  0.0, std::sin(radian), std::cos(radian),  0.0,
						  0.0, 0.0,				 0.0,				1.0 };
		} break;

		case Axis::Y:
		{
			return CTM_t{ std::cos(radian),	 0.0, std::sin(radian), 0.0,
						  0.0,				 1.0, 0.0,				0.0,
						  -std::sin(radian), 0.0, std::cos(radian), 0.0,
						  0.0,				 0.0, 0.0,				1.0 };
		} break;

		case Axis::Z:
		default:
		{
			return CTM_t{ std::cos(radian), -std::sin(radian), 0.0, 0.0,
						  std::sin(radian), std::cos(radian),  0.0, 0.0,
						  0.0,				0.0,			   1.0, 0.0,
						  0.0,				0.0,			   0.0, 1.0 };
		} break;
	}
}
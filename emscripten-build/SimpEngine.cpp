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
				auto params = mpark::get<Vector3>(command.parameters());
				auto scaleMatrix = CTM_t{ params[0], 0.0,		0.0,	   0.0,
										  0.0,       params[1], 0.0,	   0.0,
										  0.0,		 0.0,		params[2], 0.0,
										  0.0,		 0.0,		0.0		 , 1.0 };
				CTM = CTM * scaleMatrix;
			} break;

			case Command::Operation::Translate:
			{
				auto params = mpark::get<Vector3>(command.parameters());
				auto translationMatrix = CTM_t{ 1.0, 0.0, 0.0, params[0],
												0.0, 1.0, 0.0, params[1],
												0.0, 0.0, 1.0, params[2],
												0.0, 0.0, 0.0, 1.0 };
				CTM = CTM * translationMatrix;
			} break;

			case Command::Operation::Rotate:
			{
				auto params = mpark::get<RotateParams>(command.parameters());
				auto rotationMatrix = getRotationMatrix(params.first, params.second);

				CTM = CTM * rotationMatrix;
			} break;

			case Command::Operation::Line:
			{
				auto params = mpark::get<LineParams>(command.parameters());
				auto point1 = std::array<double, 4>{ params[0].x, params[0].y, params[0].z, 1 };
				auto point2 = std::array<double, 4>{ params[1].x, params[1].y, params[1].z, 1 };
				auto transformedPoint1 = cameraCTMInv * (CTM * point1);
				auto transformedPoint2 = cameraCTMInv * (CTM * point2);

				// Send to rendering engine to render
				_renderEngine.RenderLine(Line_t { Point4D {transformedPoint1, params[0].color}, Point4D{transformedPoint2, params[1].color} });
			} break;

			case Command::Operation::Polygon:
			{
				auto params = mpark::get<PolygonParams>(command.parameters());
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
				auto color = mpark::get<Color>(command.parameters());
				_renderEngine.SetAmbientColor(color);
			} break;

			case Command::Operation::Camera:
			{
				auto params = mpark::get<CameraParams>(command.parameters());
				cameraCTMInv = invert(CTM);
				auto camera = Camera{ cameraCTMInv, params.xLow, params.xHigh, params.yLow, params.yHigh, params.near, params.far };
				_renderEngine.SetCamera(camera);
			} break;

			case Command::Operation::Depth:
			{
				auto params = mpark::get<DepthParams>(command.parameters());
				_renderEngine.SetDepth(Depth{ params.near, params.far, params.color });
			} break;

			case Command::Operation::VertexNormal:
			{
				normals.emplace_back(mpark::get<Vector3>(command.parameters()));
			} break;

			case Command::Operation::Vertex:
			{
				Vertex v;
				v.location = mpark::get<Point4D>(command.parameters());
				v.location = this->CTM * v.location;
				v.location = this->cameraCTMInv * v.location;

				vertices.push_back(v);
			} break;

			case Command::Operation::Face:
			{
				FaceParam params = mpark::get<FaceParam>(command.parameters());
				std::vector<Vertex*> faceVertices;
				faceVertices.reserve(params.size());

				std::vector<int> vertexIndices;
				vertexIndices.reserve(params.size());
				std::for_each(params.begin(), params.end(), [this, &faceVertices, &vertexIndices](auto& vertex)
				{
					// Vertex
					Vertex* v;
					std::size_t index;
					if (vertex[0] > 0)
					{
						index = vertex[0] - 1;
					}
					else
					{
						index = this->vertices.size() + vertex[0];
					}

					v = &this->vertices.at(index);
					faceVertices.push_back(v);
					vertexIndices.push_back(static_cast<int>(index));

					// Normal
					if (vertex[2] != 0)
					{
						Point normal;
						if (vertex[2] > 0)
						{
							normal = this->normals.at(vertex[2] - 1);
						}
						else
						{
							normal = this->normals.at(this->normals.size() + vertex[2]);
						}
						v->assignedNormal = normal;
					}

					return v;
				});

				//auto thesholdInRadian = getRadianFromDegree(45);
				//auto cosThreshold = std::cos(thesholdInRadian);

				if (faceVertices.size() > 3)
				{
					Face tempFace;
					tempFace.vertices = faceVertices;
					tempFace.vertexIndices = vertexIndices;
					auto normal = tempFace.getFaceNormal();
					for (auto v : tempFace.vertices)
					{
						/*if (v->faceNormals.size() > 0)
						{
							if (std::any_of(v->faceNormals.begin(), v->faceNormals.end(), [cosThreshold, &normal](auto& n) { return dot(normal, n) < cosThreshold; }))
							{
								v->faceNormals.push_back(normal);
							}
						}
						else
						{*/
							v->faceNormals.push_back(normal);
						//}
					}
					
					for (auto i = 1u; i < faceVertices.size() - 1; ++i)
					{
						Face face;
						face.vertices.push_back(faceVertices[0]);
						face.vertices.push_back(faceVertices[i]);
						face.vertices.push_back(faceVertices[i + 1]);
					
						face.vertexIndices.push_back(vertexIndices[0]);
						face.vertexIndices.push_back(vertexIndices[i]);
						face.vertexIndices.push_back(vertexIndices[i + 1]);

						face.normal = normal;
						faces.push_back(face);
						//_renderEngine.RenderTriangle(triangle, currentRenderMode);
					}

				}
				else
				{
					Face face;
					face.vertices = faceVertices;
					face.vertexIndices = vertexIndices;
					auto normal = face.getFaceNormal();
					for (auto v : face.vertices)
					{
						v->faceNormals.push_back(normal);
					}
					face.normal = normal;

					faces.push_back(face);
					//_renderEngine.RenderTriangle(faceVertices, currentRenderMode);
				}

			} break;

			case Command::Operation::ObjectFile:
			{
				if (mpark::get<std::string>(command.parameters()) == "ENDOFOBJECTFILE"s)
				{
					// Draw all the faces
					for (auto& f : faces)
					{
						f.vertices.clear();
						for (auto i : f.vertexIndices)
						{
							f.vertices.push_back(&vertices[i]);
						}
						_renderEngine.RenderFace(f, currentRenderMode);
					}

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

					if (!objFileFacesStack.empty())
					{
						faces = objFileFacesStack.top();
						objFileFacesStack.pop();
					}
				}
				else
				{
					objFileVerticesStack.push(vertices);
					vertices.clear();

					objFileNormalsStack.push(normals);
					normals.clear();

					objFileFacesStack.push(faces);
					faces.clear();
				}
			} break;

			case Command::Operation::Surface:
			{
				auto params = mpark::get<SurfaceParams>(command.parameters());
				_renderEngine.SetSpecularCoefficient(params[0]);
				_renderEngine.SetSpecularExponent(params[1]);
			} break;

			case Command::Operation::Light:
			{
				auto params = mpark::get<LightParams>(command.parameters());
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
				_renderEngine.SetLightingMethod(mpark::get<LightingMethod>(command.parameters()));
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
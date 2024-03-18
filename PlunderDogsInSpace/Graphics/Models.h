#pragma once
#include "Shaders.h"
#include <vector>
#include <unordered_map>

struct Vertex
{
	Vertex() : position(glm::vec3(0.f)), color(glm::vec3(0.f)), normal(glm::vec3(0.f)), texCoords(glm::vec2(0.f))
	{}
	Vertex(const glm::vec3& Position, const glm::vec3& Color, const glm::vec3& Normal, const glm::vec2& TexCoords) :
		position(Position), color(Color), normal(Normal), texCoords(TexCoords)
	{}
	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct Model
{
public:
	Model(std::vector<Vertex>&& Vertices, std::vector<unsigned int>&& Elements);
	Model(const Model& other);
	Model(Model&& other);
	void operator=(const Model& other)
	{
		vertices = other.vertices;
		elements = other.elements;
	}
	void operator=(Model&& other)
	{
		vertices = std::move(other.vertices);
		elements = std::move(other.elements);
	}
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> elements;
public:
	void BatchModel(const std::vector<glm::vec3>& batchPositions);
public:
	int batchAmount;
	std::vector<Vertex> batchedVertices;
	std::vector<unsigned int> batchedElements;
};

class GLModel
{
public:
	GLModel(Model&& Model);
	GLModel(const GLModel& other);
	GLModel(GLModel&& other);
public:
	void Render(GLShader& s);
	void SetUpMesh();
	void operator=(const GLModel& other)
	{
		m_model = other.m_model;
		VertexBufferObject = other.VertexBufferObject;
		VertexArrayObject = other.VertexArrayObject;
		ElementBufferObject = other.ElementBufferObject;
	}
	void operator=(GLModel&& other)
	{
		m_model = std::move(other.m_model);
		std::swap(VertexBufferObject, other.VertexBufferObject);
		std::swap(VertexArrayObject, other.VertexArrayObject);
		std::swap(ElementBufferObject, other.ElementBufferObject);
	}
protected:
	Model m_model;
protected:
	unsigned int VertexBufferObject, VertexArrayObject, ElementBufferObject;
};

struct GLModelLoading
{
public:
	void LoadBaseModels(std::unordered_map<unsigned int, GLModel>& Models);
private:
	void GenFace(std::vector<Vertex>& verts, std::vector<unsigned int>& elements, const glm::vec3& forward, const float forwardAmount, const glm::vec3& up, const float size, const int elementOffset = 0, const glm::vec3& positionOffset = glm::vec3(0.f), const glm::vec3& color = glm::vec3(1.f));
	void LoadPlane(std::unordered_map<unsigned int, GLModel>& Models, const unsigned int ID, const float Size = 1.f);
	void LoadBox(std::unordered_map<unsigned int, GLModel>& Models, const unsigned int ID, const float Size = 1.f);
	void LoadSphere(std::unordered_map<unsigned int, GLModel>& Models, const unsigned int ID, const float Size = 1.f, const int GraphicsLevel = 0);
};

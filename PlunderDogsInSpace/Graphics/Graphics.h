#pragma once
#include "Shaders.h"
#include <vector>
#include <iostream>
#include <unordered_map>

struct GLFWwindow;
struct GLShader;

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
};

class GLModel
{
public:
	GLModel(Model&& Model);
	GLModel(const GLModel& other);
	GLModel(GLModel&& other);
public:
	void Render(GLShader &s);
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

class Graphics
{
public:
	Graphics();
	Graphics(float ScreenWidth, float ScreenHeight);
public:
	virtual bool Init() = 0;
	virtual void SetCallbacks() = 0;
	virtual void ResizeScreen() = 0;
	glm::vec2 GetScreenDimensions() const
	{
		return glm::vec2(m_screenWidth, m_screenHeight);
	}
	virtual glm::vec2 GetMouseLocation() const = 0;
	virtual bool ShouldWindowClose() = 0;
	virtual void Clear() = 0;
	virtual void Render(const unsigned int ModelID, const unsigned int ShaderID, const bool Is3D = false, const glm::mat4& ModelXForm = glm::mat4()) = 0;
	virtual void Display() = 0;
	virtual void PollEvents() = 0;
	Camera& GetCamera() { return m_cam; }
	const Camera& GetConstCamera() const { return m_cam; }
protected:
	float m_screenWidth, m_screenHeight;
	Camera m_cam;
};

class NullGraphics : public Graphics
{
public:
	NullGraphics();
public:
	virtual bool Init() override final { return false; }
	virtual void SetCallbacks() override final {}
	virtual void ResizeScreen() override final {}
	virtual glm::vec2 GetMouseLocation() const override final { return glm::vec2(0.f); }
	virtual bool ShouldWindowClose() override final { return true; }
	virtual void Clear() override final {}
	virtual void Render(const unsigned int ModelID, const unsigned int ShaderID, const bool Is3D = false, const glm::mat4& ModelXForm = glm::mat4()) override final {}
	virtual void Display() override final {}
	virtual void PollEvents() override final {}
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

class GLGraphics : public Graphics
{
public:
	GLGraphics();
	GLGraphics(float ScreenWidth, float ScreenHeight);
	~GLGraphics();
public:
	virtual bool Init() override final;
	virtual void SetCallbacks() override final;
	virtual void ResizeScreen() override final;
	virtual glm::vec2 GetMouseLocation() const override final;
	virtual bool ShouldWindowClose() override final;
	virtual void Clear() override final;
	bool DoesModelExist(const int ModelID) const;
	virtual void Render(const unsigned int ModelID, const unsigned int ShaderID, const bool Is3D = false, const glm::mat4& ModelXForm = glm::mat4()) override final;
	virtual void Display() override final;
	virtual void PollEvents() override final;
	GLShader& GetShader(const int ID) { return m_shaders.at(ID); }
	inline bool ShaderExist(const int ID) { return m_shaders.find(ID) != m_shaders.end(); }
	GLFWwindow* GetWindow() const { return m_window; }
protected:
	GLFWwindow* m_window;
	GLShader m_shader;
	std::unordered_map<unsigned int, GLModel> m_models;
	std::unordered_map<unsigned int, GLShader> m_shaders;
};
#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/quaternion.hpp" 
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>
#include <unordered_map>

struct GLFWwindow;
struct GLShader;

class Camera
{
public:
	Camera();
public:
	glm::vec3 GetPos() const { return m_position; }
	void SetPos(const glm::vec3& NewPos) { m_position = NewPos; }
	glm::quat GetRotation() const { return m_rotation; }
	void SetRotation(const glm::quat& NewRot) { m_rotation = NewRot; }
	glm::vec3 GetViewDir() const 
	{
		glm::quat q(0.f, 0.f, 0.f, -1.f);
		q = m_rotation * q;
		q = q * glm::conjugate(m_rotation);
		return glm::vec3(q.x, q.y, q.z);
	}
	glm::vec3 GetViewDirOnScreen(const glm::vec2& ScreenSize, const glm::vec2& MousePosition) const
	{
		glm::quat q(0.f, 0.f, 0.f, -1.f);
		glm::quat rotator = m_rotation;

		float xRotation = m_fov * (MousePosition.y / ScreenSize.y);
		xRotation -= m_fov * 0.5;
		xRotation *= -1;

		float yRotation = m_fov * (MousePosition.x / ScreenSize.x);
		yRotation -= m_fov * 0.5;
		yRotation *= -1;

		rotator *= glm::quat(glm::vec3(glm::radians(xRotation), 0.f, 0.f));
		rotator *= glm::quat(glm::vec3(0.f, glm::radians(yRotation), 0.f));

		q = rotator * q;
		q = q * glm::conjugate(rotator);
		return glm::vec3(q.x, q.y, q.z);
	}
	float GetFOV() const
	{
		return m_fov;
	}
protected:
	glm::vec3 m_position;
	glm::quat m_rotation;
	float m_fov;
};

struct Vertex
{
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

struct GLUniformSetter
{
	void SetFloat(const int ID, const std::string& name, float value) const;
	inline void SetBool(const int ID, const std::string& name, bool value) const;
	inline void SetInt(const int ID, const std::string& name, int value) const;
	inline void SetVec3(const int ID, const std::string& name, const glm::vec3& value) const;
	inline void SetMat4(const int ID, const std::string& name, const glm::mat4& value) const;
};

struct GLShader
{
public:
	GLShader();
	GLShader(const char* VertexPath, const char* FragmentPath);
	GLShader(const GLShader& other);
	GLShader(GLShader&& other);
	inline unsigned int GetID() const { return ID; }
	unsigned int DebugLocation(const unsigned int ID, const char* name);
	~GLShader();
public:
	unsigned int ID;
public:
	void operator=(const GLShader& other) { ID = other.ID; }
	void operator=(GLShader&& other) { ID = other.ID; other.ID = UINT32_MAX; }
	void Use() const;
	void SetRender2D() const;
	void SetRender3D(const Camera& Cam) const;
public:
	GLUniformSetter m_uniforms;
private:
	void CheckCompileErrors(const unsigned int shader, const std::string&& type) const;
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
	void GenFace(std::vector<Vertex>& verts, std::vector<unsigned int>& elements, const glm::vec3& min, const glm::vec3& max, const glm::vec3& color, const glm::vec3& normal, unsigned int offset) const;
	void LoadPlane(std::unordered_map<unsigned int, GLModel>& Models);
	void LoadBox(std::unordered_map<unsigned int, GLModel>& Models);
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
	virtual void Render(const unsigned int ModelID, const unsigned int ShaderID, const bool Is3D = false, const glm::mat4& ModelXForm = glm::mat4()) override final;
	virtual void Display() override final;
	virtual void PollEvents() override final;
	GLShader& GetShader(const int ID) { return m_shaders.at(ID); }
	GLFWwindow* GetWindow() const { return m_window; }
protected:
	GLFWwindow* m_window;
	GLShader m_shader;
	std::unordered_map<unsigned int, GLModel> m_models;
	std::unordered_map<unsigned int, GLShader> m_shaders;
};
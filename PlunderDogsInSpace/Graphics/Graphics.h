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
	glm::vec3 GetPos() { return m_position; }
	glm::quat GetRotation() { return m_rotation; }
	glm::vec3 GetViewDir() { return glm::vec3(0, 0, 1) * m_rotation; }
protected:
	glm::vec3 m_position;
	glm::quat m_rotation;
};

struct Vertex
{
	Vertex(const glm::vec3& Position, const glm::vec3& Normal, const glm::vec3& Color, const glm::vec2& TexCoords) :
		position(Position), normal(Normal), color(Color), texCoords(TexCoords)
	{}
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texCoords;
};

struct Model
{
public:
	Model(std::vector<Vertex>&& Vertices, std::vector<unsigned int>&& Elements);
	Model(const Model& other);
	Model(Model&& other);
	friend class GLModel;
protected:
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
protected:
	Model m_model;
protected:
	unsigned int VertexBufferObject, VertexArrayObject, ElementBufferObject;
};

struct GLUniformSetter
{
	inline void SetFloat(const int ID, const std::string& name, float value) const;
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
	inline int GetID() const { return ID; }
	~GLShader();
public:
	unsigned int ID;
public:
	void operator=(const GLShader& other) { ID = other.ID; }
	void operator=(GLShader&& other) { ID = other.ID; other.ID = UINT32_MAX; }
	void Use();
	void SetRender2D();
	void SetRender3D(glm::vec3& CamPos);
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
	virtual bool ShouldWindowClose() = 0;
	virtual void Clear() = 0;
	virtual void Render(const unsigned int ID, const bool Is3D = false, const glm::mat4& ModelXForm = glm::mat4()) = 0;
	virtual void Display() = 0;
	virtual void PollEvents() = 0;
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
	virtual bool ShouldWindowClose() override final { return true; }
	virtual void Clear() override final {}
	virtual void Render(const unsigned int ID, const bool Is3D = false, const glm::mat4& ModelXForm = glm::mat4()) override final {}
	virtual void Display() override final {}
	virtual void PollEvents() override final {}
};

struct GLModelLoading
{
public:
	void LoadBaseModels(std::unordered_map<unsigned int, GLModel>& Models);
private:
	void LoadPlane(std::unordered_map<unsigned int, GLModel>& Models);
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
	virtual bool ShouldWindowClose() override final;
	virtual void Clear() override final;
	virtual void Render(const unsigned int ID, const bool Is3D = false, const glm::mat4& ModelXForm = glm::mat4()) override final;
	virtual void Display() override final;
	virtual void PollEvents() override final;
	GLShader GetShader() { return m_shader; }
protected:
	GLFWwindow* m_window;
	GLShader m_shader;
	std::unordered_map<unsigned int, GLModel> m_models;
	std::vector<GLShader> m_shaders;
};
#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm/glm.hpp"
#include <vector>
#include <iostream>

struct GLFWwindow;
struct GLShader;

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texCoords;
};

struct Model
{
public:
	Model(std::vector<Vertex> Vertices, std::vector<unsigned int> Elements);
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

struct GLShader
{
public:
	GLShader(const char* VertexPath, const char* FragmentPath);
	GLShader(const GLShader& other);
	GLShader(GLShader&& other);
	~GLShader();
public:
	unsigned int ID;
public:
	void Use();
	void SetFloat(const std::string& name, float value) const;
	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
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
	virtual void Display() = 0;
	virtual void PollEvents() = 0;
protected:
	float m_screenWidth, m_screenHeight;
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
	virtual void Display() override final {}
	virtual void PollEvents() override final {}
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
	virtual void Display() override final;
	virtual void PollEvents() override final;
protected:
	GLFWwindow* m_window;
	std::vector<GLModel> m_models;
};
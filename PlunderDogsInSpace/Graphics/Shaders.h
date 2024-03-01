#pragma once
#include "Camera.h"
#include <iostream>
#include <fstream>
#include <sstream>

struct GLUniformSetter
{
	void SetFloat(const int ID, const std::string& name, float value) const;
	bool UniformLocationExists(const int ID, const std::string& name) const;
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
#pragma once
#include "Models.h"

struct GLFWwindow;
struct GLShader;

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
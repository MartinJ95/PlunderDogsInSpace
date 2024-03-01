#include "Graphics.h"

static void glfwError(int id, const char* description)
{
	std::cout << description << std::endl;
}

Graphics::Graphics() : m_screenWidth(1280.f), m_screenHeight(720.f), m_cam()
{
}

Graphics::Graphics(float ScreenWidth, float ScreenHeight) : m_screenWidth(ScreenWidth), m_screenHeight(m_screenHeight), m_cam()
{
}

GLGraphics::GLGraphics() : Graphics(), m_window(nullptr), m_models(0)
{
}

GLGraphics::GLGraphics(float ScreenWidth, float ScreenHeight) : Graphics(ScreenWidth, ScreenHeight), m_window(nullptr), m_models(), m_shaders()
{
}

GLGraphics::~GLGraphics()
{
	if (m_window)
	{
		glfwDestroyWindow(m_window);
	}

	glfwTerminate();
}

bool GLGraphics::Init()
{
	glfwSetErrorCallback(&glfwError);

	if (!glfwInit())
		return false;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	m_window = glfwCreateWindow(m_screenWidth, m_screenHeight, "testing", nullptr, nullptr);
	if (!m_window)
	{
		return false;
	}

	glfwMakeContextCurrent(m_window);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	m_shaders.emplace(0, GLShader("default3DShader.vs", "default3DShader.fs"));
	m_shaders.emplace(1, GLShader("clickIndicator.vs", "clickIndicator.fs"));
	m_shaders.emplace(2, GLShader("HealthBarShader.vs", "HealthBarShader.fs"));
	m_shaders.emplace(3, GLShader("selectionCircle.vs", "selectionCircle.fs"));

	GLModelLoading modelLoader;

	modelLoader.LoadBaseModels(m_models);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return true;
}

void GLGraphics::SetCallbacks()
{
}

void GLGraphics::ResizeScreen()
{
}

glm::vec2 GLGraphics::GetMouseLocation() const
{
	double xPos, yPos;
	glfwGetCursorPos(m_window, &xPos, &yPos);
	return glm::vec2(xPos, yPos);
}

bool GLGraphics::ShouldWindowClose()
{
	return glfwWindowShouldClose(m_window);
}

void GLGraphics::Clear()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool GLGraphics::DoesModelExist(const int ModelID) const
{
	return m_models.find(ModelID) != m_models.end();
}

void GLGraphics::Render(const unsigned int ModelID, const unsigned int ShaderID, const bool Is3D, const glm::mat4& ModelXForm)
{
	//if (m_models.find(ModelID) == m_models.end())
		//return;

	if (Is3D)
		m_shaders.find(ShaderID)->second.m_uniforms.SetMat4(m_shaders.find(ShaderID)->second.GetID(), "model_xform", ModelXForm);

	m_models.find(ModelID)->second.Render(m_shader);
}

void GLGraphics::Display()
{
	glfwSwapBuffers(m_window);
}

void GLGraphics::PollEvents()
{
	glfwPollEvents();
}

NullGraphics::NullGraphics() : Graphics()
{
}
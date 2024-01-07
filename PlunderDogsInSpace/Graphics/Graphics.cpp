#include "Graphics.h"
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"
#include "glad/glad.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

static void glfwError(int id, const char* description)
{
	std::cout << description << std::endl;
}

Graphics::Graphics() : m_screenWidth(1280.f), m_screenHeight(720.f)
{
}

Graphics::Graphics(float ScreenWidth, float ScreenHeight) : m_screenWidth(ScreenWidth), m_screenHeight(m_screenHeight)
{
}

GLGraphics::GLGraphics() : Graphics()
{
}

GLGraphics::GLGraphics(float ScreenWidth, float ScreenHeight) : Graphics(ScreenWidth, ScreenHeight)
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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(m_screenWidth, m_screenHeight, "testing", nullptr, nullptr);
	if (!m_window)
	{
		return false;
	}

	glfwMakeContextCurrent(m_window);

	return true;
}

void GLGraphics::SetCallbacks()
{
}

void GLGraphics::ResizeScreen()
{
}

bool GLGraphics::ShouldWindowClose()
{
	return glfwWindowShouldClose(m_window);
}

void GLGraphics::Clear()
{
	glClearColor(0.2f, 0.2f, 0.6f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

GLShader::GLShader(const char* VertexPath, const char* FragmentPath) : ID(UINT32_MAX)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vertexStream;
	std::ifstream fragmentStream;
	vertexStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vertexStream.open(VertexPath);
		fragmentStream.open(FragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vertexStream.rdbuf();
		fShaderStream << fragmentStream.rdbuf();
		// close file handlers
		vertexStream.close();
		fragmentStream.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	// 2. compile shaders
	unsigned int vertex, fragment;
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	CheckCompileErrors(vertex, "VERTEX");
	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	CheckCompileErrors(fragment, "FRAGMENT");
	// shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	CheckCompileErrors(ID, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

GLShader::GLShader(const GLShader& other) : ID(other.ID)
{
}

GLShader::GLShader(GLShader&& other) : ID(other.ID)
{
	other.ID = UINT32_MAX;
}

GLShader::~GLShader()
{
	if (ID != UINT32_MAX)
	{
		glDeleteProgram(ID);
	}
}

void GLShader::Use()
{
	glUseProgram(ID);
}

void GLShader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void GLShader::SetBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void GLShader::SetInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void GLShader::CheckCompileErrors(const unsigned int shader, const std::string&& type) const
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}

#include "Graphics.h"
#include <string>
#include <fstream>
#include <sstream>

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

GLGraphics::GLGraphics() : Graphics(), m_window(nullptr)
{
}

GLGraphics::GLGraphics(float ScreenWidth, float ScreenHeight) : Graphics(ScreenWidth, ScreenHeight), m_window(nullptr)
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

GLModel::GLModel(Model&& Model) : m_model(std::move(Model)),
	VertexBufferObject(UINT32_MAX), VertexArrayObject(UINT32_MAX), ElementBufferObject(UINT32_MAX)
{

}

GLModel::GLModel(const GLModel& other) : m_model(other.m_model),
VertexBufferObject(other.VertexBufferObject), VertexArrayObject(other.VertexArrayObject), ElementBufferObject(other.ElementBufferObject)
{
}

GLModel::GLModel(GLModel&& other) : m_model(std::move(other.m_model)),
VertexBufferObject(other.VertexBufferObject), VertexArrayObject(other.VertexArrayObject), ElementBufferObject(other.ElementBufferObject)
{
	other.VertexBufferObject = UINT32_MAX;
	other.VertexArrayObject = UINT32_MAX;
	other.ElementBufferObject = UINT32_MAX;
}

void GLModel::Render(GLShader& s)
{
	glBindVertexArray(VertexArrayObject);
	// draw mesh
	glBindVertexArray(VertexArrayObject);
	glDrawElements(GL_TRIANGLES, m_model.elements.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void GLModel::SetUpMesh()
{
	glGenVertexArrays(1, &VertexArrayObject);
	glGenBuffers(1, &VertexBufferObject);
	glGenBuffers(1, &ElementBufferObject);

	glBindVertexArray(VertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBufferObject);

	glBufferData(GL_ARRAY_BUFFER, m_model.vertices.size() * sizeof(Vertex), &m_model.vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_model.elements.size() * sizeof(unsigned int),
		&m_model.elements[0], GL_STATIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	// vertex color
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	//vertex trexcoord
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

	glBindVertexArray(0);
}

Model::Model(std::vector<Vertex> Vertices, std::vector<unsigned int> Elements) :
	vertices(Vertices), elements(Elements)
{
}

Model::Model(const Model& other) :
	vertices(other.vertices), elements(other.elements)
{
}

Model::Model(Model&& other) :
	vertices(std::move(other.vertices)), elements(other.elements)
{
}

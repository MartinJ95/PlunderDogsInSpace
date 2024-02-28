#include "Shaders.h"

GLShader::GLShader() : ID(UINT32_MAX)
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

unsigned int GLShader::DebugLocation(const unsigned int ID, const char* name)
{
	return glGetUniformLocation(ID, name);
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

void GLShader::Use() const
{
	if (!glIsProgram(ID))
		return;

	glUseProgram(ID);
}

void GLUniformSetter::SetFloat(const int ID, const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

bool GLUniformSetter::UniformLocationExists(const int ID, const std::string& name) const
{
	return glGetUniformLocation(ID, name.c_str()) != -1;
}

void GLUniformSetter::SetBool(const int ID, const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void GLUniformSetter::SetInt(const int ID, const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void GLUniformSetter::SetVec3(const int ID, const std::string& name, const glm::vec3& value) const
{
	//if (glGetUniformLocation(ID, name.c_str()) == -1)
		//return;
	glUniform3f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z);
}

void GLUniformSetter::SetMat4(const int ID, const std::string& name, const glm::mat4& value) const
{
	/*if (glGetUniformLocation(ID, name.c_str()) == -1)
		return;*/
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, false, glm::value_ptr(value));
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

void GLShader::SetRender2D() const
{
	Use();
}

void GLShader::SetRender3D(const Camera& Cam) const
{
	Use();
	m_uniforms.SetVec3(ID, "cameraPosition", Cam.GetPos());
	m_uniforms.SetVec3(ID, "ambientLighting", glm::vec3(1.f, 1.f, 1.f));
	//width = 1280, height = 720, fov = 90
	glm::mat4 projection = glm::perspective(glm::radians(Cam.GetFOV()), 1280.f / 720.f, 0.1f, 100.0f);
	m_uniforms.SetMat4(ID, "projection", projection);
	glm::vec3 camPos = Cam.GetPos();
	glm::vec3 up(0.f, 1.f, 0.f);
	glm::vec3 camDir = Cam.GetViewDir();
	glm::mat4 view = glm::lookAt(camPos, camPos + camDir, up);
	//glm::mat4 view = glm::lookAt(camPos, glm::vec3(0.f, 0.f, 0.f), up);
	m_uniforms.SetMat4(ID, "view", view);
}
#include "Graphics.h"
#include <string>
#include <fstream>
#include <sstream>

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

void GLGraphics::Render(const unsigned int ModelID, const unsigned int ShaderID, const bool Is3D, const glm::mat4& ModelXForm)
{
	if (m_models.find(ModelID) == m_models.end())
		return;

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
	if (glGetUniformLocation(ID, name.c_str()) == -1)
		return;
	glUniform3f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z);
}

void GLUniformSetter::SetMat4(const int ID, const std::string& name, const glm::mat4& value) const
{
	if (glGetUniformLocation(ID, name.c_str()) == -1)
		return;
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
	// vertex color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	// vertex normal
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	//vertex trexcoord
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

	glBindVertexArray(0);
}

Model::Model(std::vector<Vertex>&& Vertices, std::vector<unsigned int>&& Elements) :
	vertices(Vertices), elements(Elements)
{
}

Model::Model(const Model& other) :
	vertices(other.vertices), elements(other.elements)
{
}

Model::Model(Model&& other) :
	vertices(std::move(other.vertices)), elements(std::move(other.elements))
{
}

Camera::Camera() : m_position(glm::vec3(0.f, 0.f, 0.f)), m_rotation(), m_fov(45.f)
{
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

void GLModelLoading::LoadBaseModels(std::unordered_map<unsigned int, GLModel>& Models)
{
	LoadPlane(Models);
	LoadBox(Models);
	LoadSphere(Models);
}

void GLModelLoading::GenFace(std::vector<Vertex>& verts, std::vector<unsigned int>& elements, const glm::vec3& forward, const float forwardAmount, const glm::vec3& up, const float size, const int elementOffset, const glm::vec3& positionOffset, const glm::vec3& color)
{
	glm::vec3 side = glm::normalize(glm::cross(forward, up));

	glm::vec3 appliedForward = forward * forwardAmount;
	glm::vec3 appliedSide = side * size;
	glm::vec3 appliedUp = up * size;

	verts.emplace_back(
		appliedForward + -appliedSide + appliedUp + positionOffset,
		color,
		forward,
		glm::vec2(0.f, 1.f)
	);

	verts.emplace_back(
		appliedForward + appliedSide + appliedUp + positionOffset,
		color,
		forward,
		glm::vec2(1.f, 1.f)
	);

	verts.emplace_back(
		appliedForward + -appliedSide + -appliedUp + positionOffset,
		color,
		forward,
		glm::vec2(0.f, 0.f)
	);

	verts.emplace_back(
		appliedForward + appliedSide + -appliedUp + positionOffset,
		color,
		forward,
		glm::vec2(1.f, 0.f)
	);

	elements.emplace_back(0+elementOffset);
	elements.emplace_back(1+elementOffset);
	elements.emplace_back(2+elementOffset);
	elements.emplace_back(2+elementOffset);
	elements.emplace_back(1+elementOffset);
	elements.emplace_back(3+elementOffset);
}

void GLModelLoading::LoadPlane(std::unordered_map<unsigned int, GLModel>& Models)
{
	std::vector<Vertex> verts;
	std::vector<unsigned int> elements;

	GenFace(verts, elements, glm::vec3(0.f, 1.f, 0.f), 0.f, glm::vec3(0.f, 0.f, 1.f), 1.f);

	Model m(std::move(verts), std::move(elements));

	GLModel glModel(std::move(m));

	glModel.SetUpMesh();

	Models.emplace(0, std::move(glModel));
}

void GLModelLoading::LoadBox(std::unordered_map<unsigned int, GLModel>& Models)
{
	std::vector<Vertex> verts;
	std::vector<unsigned int> elements;

	//top and bottom
	GenFace(verts, elements, glm::vec3(0.f, 1.f, 0.f), 1.f, glm::vec3(0.f, 0.f, 1.f), 1.f);
	GenFace(verts, elements, glm::vec3(0.f, -1.f, 0.f), 1.f, glm::vec3(0.f, 0.f, -1.f), 1.f, 4);

	//sides
	GenFace(verts, elements, glm::vec3(1.f, 0.f, 0.f), 1.f, glm::vec3(0.f, 1.f, 0.f), 1.f, 8);
	GenFace(verts, elements, glm::vec3(-1.f, 0.f, 0.f), 1.f, glm::vec3(0.f, 1.f, 0.f), 1.f, 12);

	GenFace(verts, elements, glm::vec3(0.f, 0.f, 1.f), 1.f, glm::vec3(0.f, 1.f, 0.f), 1.f, 16);
	GenFace(verts, elements, glm::vec3(0.f, 0.f, -1.f), 1.f, glm::vec3(0.f, 1.f, 0.f), 1.f, 20);

	Model m(std::move(verts), std::move(elements));

	GLModel glModel(std::move(m));

	glModel.SetUpMesh();

	Models.emplace(1, std::move(glModel));
}


void GLModelLoading::LoadSphere(std::unordered_map<unsigned int, GLModel>& Models)
{
	constexpr int details = 50;

	constexpr float thetaChange = 360.f / (float)details;

	std::vector<Vertex> vertices;
	vertices.reserve(2502);
	vertices.assign(2502, Vertex());
	
	std::vector<unsigned int> elements;

	vertices[0] = Vertex(glm::vec3(0.f, 1.f, 0.f), glm::vec3(1.f, 1.f, 1.f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(0.f, 1.f));

	vertices[1] = Vertex(glm::vec3(0.f, -1.f, 0.f), glm::vec3(1.f, 1.f, 1.f), glm::vec3(0.f, -1.f, 0.f), glm::vec2(0.f, 0.f));

	for (int i = 0; i < details; i++)
	{
		glm::vec3 initial = glm::vec3(0.f, 1.f, 0.f);
		
		float thetaChange1 = thetaChange * (i + 1);

		for (int j = 0; j < details; j++)
		{
			float thetaChange2 = thetaChange * (j + 1);
			//glm::quat q = glm::quat(glm::vec3(glm::radians(thetaChange1), glm::radians(thetaChange2), 0.f));
			//glm::quat q = glm::quat(glm::vec3(glm::radians(thetaChange1), 0.f, 0.f));

			glm::vec3 rotated = initial;
			rotated = rotated * glm::quat(glm::vec3(glm::radians(thetaChange1), 0.f, 0.f));
			rotated = rotated * glm::quat(glm::vec3(0.f, glm::radians(thetaChange2), 0.f));
			rotated = glm::normalize(rotated);

			vertices[2 + (details * i) + j] = Vertex(
				//glm::normalize(initial * q),
				rotated,
				glm::vec3(1.f, 1.f, 1.f),
				//glm::normalize((initial * q) - glm::vec3(0.f)),
				glm::normalize(rotated),
				glm::vec2(1.f - (1.f / details) * j, 1.f - (1.f / details) * i)
			);

			int offset = (j + (details * i) + 2);
			int secondOffset = j < details - 1 ? 1 : -j;

			if (i > 0 && i < details - 1)
			{
				elements.emplace_back(offset + secondOffset);
				elements.emplace_back(offset);
				elements.emplace_back(offset + secondOffset + details);

				elements.emplace_back(offset + secondOffset + details);
				elements.emplace_back(offset);
				elements.emplace_back(offset + details);
			}
			else if(i == 0)
			{
				
				elements.emplace_back(0);
				elements.emplace_back(offset);
				elements.emplace_back(offset + secondOffset);
				
			}
			else if (i < details - 1)
			{
				
				elements.emplace_back(offset + secondOffset);
				elements.emplace_back(offset);
				elements.emplace_back(1);
				
			}
		}
	}

	Model m(std::move(vertices), std::move(elements));

	GLModel glModel(std::move(m));

	glModel.SetUpMesh();

	Models.emplace(2, std::move(glModel));
}
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

void GLModelLoading::LoadBaseModels(std::unordered_map<unsigned int, GLModel>& Models)
{
	LoadPlane(Models, 0);
	LoadPlane(Models, 1, 2.f);
	LoadBox(Models, 2);
	LoadSphere(Models, 3);
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

void GLModelLoading::LoadPlane(std::unordered_map<unsigned int, GLModel>& Models, const unsigned int ID, const float Size)
{
	std::vector<Vertex> verts;
	std::vector<unsigned int> elements;

	GenFace(verts, elements, glm::vec3(0.f, 1.f, 0.f), 0.f, glm::vec3(0.f, 0.f, 1.f), Size);

	Model m(std::move(verts), std::move(elements));

	GLModel glModel(std::move(m));

	glModel.SetUpMesh();

	Models.emplace(ID, std::move(glModel));
}

void GLModelLoading::LoadBox(std::unordered_map<unsigned int, GLModel>& Models, const unsigned int ID, const float Size)
{
	std::vector<Vertex> verts;
	std::vector<unsigned int> elements;

	//top and bottom
	GenFace(verts, elements, glm::vec3(0.f, 1.f, 0.f), Size, glm::vec3(0.f, 0.f, 1.f), Size);
	GenFace(verts, elements, glm::vec3(0.f, -1.f, 0.f), Size, glm::vec3(0.f, 0.f, -1.f), Size, 4);

	//sides
	GenFace(verts, elements, glm::vec3(1.f, 0.f, 0.f), Size, glm::vec3(0.f, 1.f, 0.f), Size, 8);
	GenFace(verts, elements, glm::vec3(-1.f, 0.f, 0.f), Size, glm::vec3(0.f, 1.f, 0.f), Size, 12);

	GenFace(verts, elements, glm::vec3(0.f, 0.f, 1.f), Size, glm::vec3(0.f, 1.f, 0.f), Size, 16);
	GenFace(verts, elements, glm::vec3(0.f, 0.f, -1.f), Size, glm::vec3(0.f, 1.f, 0.f), Size, 20);

	Model m(std::move(verts), std::move(elements));

	GLModel glModel(std::move(m));

	glModel.SetUpMesh();

	Models.emplace(ID, std::move(glModel));
}


void GLModelLoading::LoadSphere(std::unordered_map<unsigned int, GLModel>& Models, const unsigned int ID, const float Size, const int GraphicsLevel)
{
	constexpr int details[]{ 50, 40, 30, 20, 10 };

	constexpr float thetaChange[]{ 360.f / (float)details[0], 360.f / (float)details[1], 360.f / (float)details[2], 360.f / (float)details[3], 360.f / (float)details[4] };

	std::vector<Vertex> vertices;
	vertices.reserve(2502);
	vertices.assign(2502, Vertex());
	
	std::vector<unsigned int> elements;

	vertices[0] = Vertex(glm::vec3(0.f, 1.f, 0.f), glm::vec3(1.f, 1.f, 1.f), glm::vec3(0.f, 1.f, 0.f), glm::vec2(0.f, 1.f));

	vertices[1] = Vertex(glm::vec3(0.f, -1.f, 0.f), glm::vec3(1.f, 1.f, 1.f), glm::vec3(0.f, -1.f, 0.f), glm::vec2(0.f, 0.f));

	for (int i = 0; i < details[GraphicsLevel]; i++)
	{
		glm::vec3 initial = glm::vec3(0.f, 1.f, 0.f);
		
		//pitch
		float thetaChange1 = (thetaChange[GraphicsLevel] * (i + 1))*0.5;

		for (int j = 0; j < details[GraphicsLevel]; j++)
		{
			//roll
			float thetaChange2 = thetaChange[GraphicsLevel] * (j + 1);
			//glm::quat q = glm::quat(glm::vec3(glm::radians(thetaChange1), glm::radians(thetaChange2), 0.f));
			//glm::quat q = glm::quat(glm::vec3(glm::radians(thetaChange1), 0.f, 0.f));

			glm::vec3 rotated = initial;
			rotated = rotated * glm::quat(glm::vec3(glm::radians(thetaChange1), 0.f, 0.f));
			rotated = rotated * glm::quat(glm::vec3(0.f, glm::radians(thetaChange2), 0.f));
			rotated = glm::normalize(rotated);

			vertices[2 + (details[GraphicsLevel] * i) + j] = Vertex(
				//glm::normalize(initial * q),
				rotated,
				glm::vec3(1.f, 1.f, 1.f),
				//glm::normalize((initial * q) - glm::vec3(0.f)),
				glm::normalize(rotated),
				glm::vec2(1.f - (1.f / details[GraphicsLevel]) * j, 1.f - (1.f / details[GraphicsLevel]) * i)
			);

			int offset = (j + (details[GraphicsLevel] * i) + 2);
			int secondOffset = j < details[GraphicsLevel] - 1 ? 1 : -j;

			if (i > 0 && i < details[GraphicsLevel] - 1)
			{
				//mid
				elements.emplace_back(offset + secondOffset);
				elements.emplace_back(offset);
				elements.emplace_back(offset + secondOffset + details[GraphicsLevel]);

				elements.emplace_back(offset + secondOffset + details[GraphicsLevel]);
				elements.emplace_back(offset);
				elements.emplace_back(offset + details[GraphicsLevel]);
			}
			else if(i == 0)
			{
				//top
				elements.emplace_back(0);
				elements.emplace_back(offset);
				elements.emplace_back(offset + secondOffset);

				//first mid
				elements.emplace_back(offset + secondOffset);
				elements.emplace_back(offset);
				elements.emplace_back(offset + secondOffset + details[GraphicsLevel]);

				elements.emplace_back(offset + secondOffset + details[GraphicsLevel]);
				elements.emplace_back(offset);
				elements.emplace_back(offset + details[GraphicsLevel]);
				
			}
			else if (i < details[GraphicsLevel] - 1)
			{
				//bottom
				elements.emplace_back(offset + secondOffset);
				elements.emplace_back(offset);
				elements.emplace_back(1);
				
			}
		}
	}

	Model m(std::move(vertices), std::move(elements));

	GLModel glModel(std::move(m));

	glModel.SetUpMesh();

	Models.emplace(ID, std::move(glModel));
}
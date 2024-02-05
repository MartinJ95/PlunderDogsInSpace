#version 400 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexColor;
layout (location = 2) in vec3 vertexNormal;
layout (location = 3) in vec2 vertexTexCoords;

out vec3 VertexPosition;

out vec2 TexCoord;
out vec3 Position;
out vec3 Normal;
out vec3 Color;
out vec4 gl_Position;

uniform mat4 model_xform;
uniform mat4 view;
uniform mat4 projection;
uniform sampler2D Texture;
uniform vec3 cameraPosition;
uniform vec3 ambientLighting;

void main()
{
	VertexPosition = vertexPosition;
	mat3 normalMatrix = transpose(inverse(mat3(model_xform)));
	gl_Position = projection * view * model_xform * vec4(vertexPosition, 1);
	//gl_Position = vec4(vertexPosition, 1);
	Position = vec4(model_xform * vec4(vertexPosition, 1.0)).xyz;
	TexCoord = vertexTexCoords;
	Color = vertexColor;
	Normal = normalMatrix * vertexNormal;
}
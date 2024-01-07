#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexColor;
layout (location = 2) in vec3 vertexNormal;
layout (location = 3) in vec2 vertexTexCoords;

out vec2 TexCoord;
out vec4 gl_Position;

uniform mat4 model_xform;

void main()
{
	gl_Position = model_xform * vec4(vertexPosition, 1);
	//gl_Position = vec4(vertexPosition, 1);
	TexCoord = vertexTexCoords;
}
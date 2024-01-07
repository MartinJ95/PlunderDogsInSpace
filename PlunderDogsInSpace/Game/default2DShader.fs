#version 330 core

in vec2 TexCoord;

uniform sampler2D Texture;

void main()
{
	//gl_FragColor = vec4(1, 0, 0, 1);
	gl_FragColor = texture(Texture, TexCoord);
}
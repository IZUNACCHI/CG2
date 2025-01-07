#version 330 core

in vec position;
in vec2 texCoord;

out vec2 TexCoord;

void main()
{
	TexCoord = texCoord;
	gl_Position = vec4(position, 0.0, 1.0);
};
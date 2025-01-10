#version 330 core

in vec3 position;
in vec2 texCoord;

out vec2 TexCoord;

uniform vec2 frameOffset;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
	TexCoord = texCoord + frameOffset;
	gl_Position = projection * view * model * vec4(position, 1.0);
	//gl_Position = model * vec4(position, 1.0);
};
#version 330 core

in vec2 TexCoord;

out vec4 outColor;

uniform sampler2D Texture;

void main()
{
	vec4 TexColor = texture(Texture, TexCoord);
	if(TexColor.r == 1.0f && TexColor.g == 0.0f && TexColor.b == 1.0f)
		discard;
	outColor = TexColor;
};
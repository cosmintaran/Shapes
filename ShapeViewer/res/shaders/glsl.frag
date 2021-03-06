#version 330 core

out vec4 o_Color;

in vec4 v_Color;
in vec2 v_TexCoords;
in float v_TexIndex;

uniform sampler2D u_Textures[32];

void main()
{
	int index = int(v_TexIndex);
	o_Color = v_Color;
	texture(u_Textures[index], v_TexCoords) * v_Color;
}
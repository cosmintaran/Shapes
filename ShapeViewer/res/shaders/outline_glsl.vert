
#version 330 core
layout(location = 0) in vec3 a_Position;

uniform mat4 u_ViewProjectionMatrix;
uniform vec4 u_Color;

out vec4 v_Color;



void main()
{
	v_Color = u_Color;
	gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
}


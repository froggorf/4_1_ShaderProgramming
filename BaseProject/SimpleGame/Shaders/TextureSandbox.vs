#version 330

in vec3 a_Position;
in vec2 a_Texture;

out vec2 v_Texpos;

uniform float u_Time;



void main()
{
	vec4 newPosition = vec4(a_Position, 1);
	gl_Position = newPosition;
	v_Texpos = a_Texture;
}

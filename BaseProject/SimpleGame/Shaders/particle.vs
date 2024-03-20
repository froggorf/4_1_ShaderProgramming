#version 330

in vec3 a_Position;

const vec3 c_StartPos = vec3(-1,0,0);
const vec3 c_Velocity = vec3(0.001,0,0);
uniform float u_Time;

void main()
{
	vec4 newPosition = vec4((a_Position + c_StartPos) + c_Velocity*u_Time, 1);
	gl_Position = newPosition;
}

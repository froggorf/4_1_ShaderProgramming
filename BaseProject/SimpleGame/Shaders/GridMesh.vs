#version 330

in vec3 a_Position;

uniform float u_Time;
const float c_PI = 3.141592;

out vec4 v_Color;

void Flag(){
	vec4 newPosition = vec4(a_Position, 1);
	float scaledValue = a_Position.x + 0.5;
	float period = 1.5;
	float sinInput = (scaledValue) * c_PI * 2 * period;	// 0pi~2pi

	float timeScale = 6;
	float sinValue =  sin(sinInput-u_Time*timeScale);

	float width = 0.5 * (1-scaledValue);

	newPosition.y = width* newPosition.y + scaledValue *0.5*sinValue * width;
	gl_Position = newPosition;
	v_Color = vec4((sinValue+1)/2);
}

void SphereMask(){
	float r = 1;
	float scale = 5;
	float newX = r*sin(a_Position.y*scale)*cos(a_Position.x*scale);
	float newY = r*sin(a_Position.y*scale)*sin(a_Position.x*scale);
	float newZ = r*cos(a_Position.y*scale);
	gl_Position = vec4(newX,newY,newZ,1);
	v_Color = vec4(1);
}


void main()
{
	SphereMask();
}


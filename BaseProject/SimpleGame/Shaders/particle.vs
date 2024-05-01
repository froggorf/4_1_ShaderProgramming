#version 330

in vec3 a_Position;

// == Line
const vec3 c_StartPos = vec3(-1,0,0);
const vec3 c_Velocity = vec3(2.0,0,0);
uniform float u_Time = 0;
uniform float u_Period = 2.0;
// == 

// == Circle
const float c_Pi = 3.141592;
// ==

// == Parabola
const vec3 c_ParaVelocity = vec3(2.0,2.0,0);
const vec2 c_2DGravity = vec2(0.0, -4.9);
// ==

// == Triangle
const vec2 c_TriPos[3] = vec2[](vec2(0.0,0.5), vec2(-0.5, -0.5),  vec2(0.5,-0.5));

void Line()
{
	float newTime = abs(fract(u_Time/u_Period)-0.5)*2.0;
	newTime = abs(fract(u_Time)-0.5)*2.0;
	vec4 newPosition = vec4((a_Position + c_StartPos) + c_Velocity*newTime, 1);
	gl_Position = newPosition;
}

void Circle()
{
	// 반시계
	//float newTime = fract(u_Time/u_Period) * c_Pi * 2.0;
	// 시계
	float newTime = -fract(u_Time/u_Period) * c_Pi * 2.0;
	vec2 trans = vec2(cos(newTime), sin(newTime));

	vec4 newPosition;
	newPosition.xy = a_Position.xy + trans;
	newPosition.zw = vec2(0,1);

	gl_Position = newPosition;

}

void Parabola()
{
	float newTime = fract(u_Time / u_Period);
	float t = newTime;
	float tt = t*t;
	vec4 newPosition;
	float transX = (a_Position.x + c_StartPos.x) 
					+ c_ParaVelocity.x * t
					+ 0.5 * c_2DGravity.x * tt;
	float transY = (a_Position.y + c_StartPos.y) 
					+ c_ParaVelocity.y * t
					+ 0.5 * c_2DGravity.y * tt;
					
	newPosition.xy = vec2(transX, transY);
	gl_Position = newPosition;
}

void Triangle()
{
	float Time = fract(u_Time/3)*3;
	vec3 newPos = a_Position;
	newPos.x = newPos.x 
				+ max(0, 1- floor(Time))	*	max(0, 1 + floor(Time))	*	mix(c_TriPos[0].x, c_TriPos[1].x, fract(Time))	// 0 ~ 1 초
				+ max(0, 2- floor(Time))	*	max(0, 0 + floor(Time))	*	mix(c_TriPos[1].x, c_TriPos[2].x, fract(Time))	// 1 ~ 2 초
				+ max(0, 3- floor(Time))	*	max(0,-1 + floor(Time))	*	mix(c_TriPos[2].x, c_TriPos[0].x, fract(Time))	// 2 ~ 3 초
				;
	newPos.y = newPos.y 
				+ max(0, 1- floor(Time))	*	max(0, 1 + floor(Time))	*	mix(c_TriPos[0].y, c_TriPos[1].y, fract(Time))	// 0 ~ 1 초
				+ max(0, 2- floor(Time))	*	max(0, 0 + floor(Time))	*	mix(c_TriPos[1].y, c_TriPos[2].y, fract(Time))	// 1 ~ 2 초
				+ max(0, 3- floor(Time))	*	max(0,-1 + floor(Time))	*	mix(c_TriPos[2].y, c_TriPos[0].y, fract(Time))	// 2 ~ 3 초
				;

	gl_Position = vec4(newPos ,1);
}

void Triangle2()
{
	// 3초로 fract
	float Time = fract(u_Time/3)*3;
	vec3 newPos = a_Position;

	// 현재 시간에 대한 int 변수
	int tttt = int(floor(Time));
	// 다음 시간
	int nexttttt = (tttt+1)%3;


	newPos.x += mix(c_TriPos[tttt].x, c_TriPos[nexttttt].x, fract(Time));
	newPos.y += mix(c_TriPos[tttt].y, c_TriPos[nexttttt].y, fract(Time));

	gl_Position = vec4(newPos ,1);
}


void Basic()
{
	vec4 newPosition = vec4(a_Position, 1);
	gl_Position = newPosition;
}

void main()
{
	//Line();
	//Circle();
	//Parabola();
	Triangle2();
	
}

#version 330

in vec3 a_Position;
in float a_StartTime;
in vec3 a_Velocity;
in float a_LifeTime;
in float a_Amp;
in float a_Period;
in float a_Value;

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
const vec2 c_2DGravity = vec2(0.0, -0.9);
// ==

// == Triangle

void Line()
{
	float newTime = abs(fract(u_Time/u_Period)-0.5)*2.0;

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

}

void Basic()
{
	vec4 newPosition = vec4(a_Position, 1);
	gl_Position = newPosition;
}
void Velocity()
{
	float t = u_Time - a_StartTime;
	vec4 newPosition;
	if(t > 0 )
	{
		t = fract(t/a_LifeTime) * a_LifeTime;
		newPosition = vec4(a_Position + fract(t)*a_Velocity, 1);
	
	}
	else
	{
		newPosition.x = -10000;
	}
	gl_Position = newPosition;
	
}

void SinShape()
{
	float t = u_Time - a_StartTime;
	vec4 newPosition = vec4(a_Position,1);
	float amp = a_Amp;
	float period = a_Period;

	


	if(t > 0 )
	{
		t = fract(t/a_LifeTime) * a_LifeTime;
		float tt = t*t;
		float value = a_Value * 2.0 * c_Pi;	
		float x = cos(value);
		float y = sin(value);

		newPosition.xy = newPosition.xy + vec2(x,y);

		vec2 newVel = a_Velocity.xy + c_2DGravity*t;

		// 진행방향의 법선벡터
		vec2 newDir = vec2(-newVel.y, newVel.x);
		newDir = normalize(newDir);

		newPosition.xy = newPosition.xy + a_Velocity.xy * t + 0.5 * c_2DGravity* tt;
		newPosition.xy = newPosition.xy + newDir* (t*0.1)*amp*sin(t*c_Pi*period);
	
	}
	else
	{
		newPosition.x = -10000;
	}
	gl_Position = newPosition;
}

void main()
{
	//Velocity();
	SinShape();
}

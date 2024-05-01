#version 330

in vec3 a_Position;
in float a_StartTime;
in vec3 a_Velocity;
in float a_LifeTime;
in float a_Amp;
in float a_Period;
in float a_Value;
in vec4 a_Color;

out vec4 v_Color;

uniform float u_Time = 0;
uniform float u_Period = 2.0;
uniform vec2 u_WindAcc = vec2(0,0);
uniform vec2 u_AttractPos = vec2(0,0);

// == Line
const vec3 c_StartPos = vec3(-1,0,0);
const vec3 c_Velocity = vec3(2.0,0,0);
// == 

// == Circle
const float c_Pi = 3.141592;
// ==

// == Parabola
const vec3 c_ParaVelocity = vec3(2.0,2.0,0);
const vec2 c_2DGravity = vec2(0.0, -0.9);
// ==

// == Triangle

void Velocity()
{
	float t = u_Time - a_StartTime;
	vec4 newPosition;
	if(t > 0 )
	{
		t = fract(t/a_LifeTime) * a_LifeTime;
		float tt =t*t;

		// t*a_Velocity + vec3(0.5*(c_2DGravity+u_WindAcc)*tt,0)
		newPosition = vec4(a_Position , 1);
		newPosition.xy = mix(vec2(newPosition.xy),u_AttractPos, fract(t/a_LifeTime));
	}
	else
	{
		newPosition.x = -10000;
	}
	gl_Position = newPosition;
	v_Color = a_Color;
	
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
		newPosition.xy = newPosition.xy + newDir* (t)*amp*sin(t*c_Pi*period);
	
	}
	else
	{
		newPosition.x = -10000;
	}
	gl_Position = newPosition;
	v_Color = a_Color;
}

void CircleShape()
{
	float t = u_Time - a_StartTime;
	vec4 newPosition = vec4(a_Position,1);
	float amp = a_Amp;
	float period = a_Period;

	if(t > 0 )
	{
		t = fract(t/a_LifeTime) * a_LifeTime;
		float tt = t*t;
		float value = a_StartTime * 2.0 * c_Pi;	
		//float x = cos(value*a_StartTime);
		//float y = sin(value*a_StartTime);
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
	v_Color = a_Color;
}

void HeartShapeCycle()
{
	float t = u_Time - a_StartTime;
	vec4 newPosition = vec4(a_Position,1);
	float amp = a_Amp;
	float period = a_Period;

	if(t > 0 )
	{
		t = fract(t/a_LifeTime) * a_LifeTime;

		float particleAlpha = 1 - t / a_LifeTime;
		float tt = t*t;
		float value = a_StartTime * 2.0 * c_Pi;	
		float x = 16*pow(sin(value),3);
		float y = 13*cos(value) - 5 * cos(2*value) - 2* cos(3*value) - cos(4*value);
		x*=0.05;
		y*=0.05;

		newPosition.xy = newPosition.xy + vec2(x,y);

		vec2 newVel = a_Velocity.xy + c_2DGravity*t;

		// 진행방향의 법선벡터
		vec2 newDir = vec2(-newVel.y, newVel.x);
		newDir = normalize(newDir);

		newPosition.xy = newPosition.xy + a_Velocity.xy * t + 0.5 * c_2DGravity* tt;
		newPosition.xy = newPosition.xy + newDir* (t*0.1)*amp*sin(t*c_Pi*period);
		v_Color = vec4(a_Color.rgb,particleAlpha);
	}
	else
	{
		newPosition.x = -10000;
		v_Color = a_Color;
	}
	gl_Position = newPosition;
}

void main()
{
	//SinShape();
	//Velocity();
	//CircleShape();
	//Triangle();
	//HeartShapeCycle();
}

#version 330

layout(location=0) out vec4 FragColor;

uniform float u_Time;

in vec4 v_Color;

const float c_PI = 3.141592;

void FillCircle(){
	vec4 newColor = vec4(1,1,1,1);
	float r = 0.5f;
	vec2 center = vec2(0.5,0.5);
	float dist = distance(v_Color.rg,center);
	if(dist < r){}
	else{
		newColor = vec4(0,0,0,0);
	}
	FragColor = newColor;
}

void Circle()
{
	vec4 newColor = vec4(1,1,1,1);
	float r = 0.5f;
	float width = 0.05f;
	vec2 center = vec2(0.5,0.5);
	float dist = distance(v_Color.rg,center);
	if(dist > r - width && dist < r) {}
	else{
		newColor = vec4(0,0,0,0);
	}	
	FragColor = newColor;

}

void Circles()
{
	float circleCount = 10;	
	vec2 circleCenter = vec2(0.5,0.5);	// 0 ~ 1

	float dist = distance(v_Color.rg,circleCenter);

	float input =c_PI*circleCount*4*dist + -u_Time*10;
	float sinValue = pow(sin(input),256);
	FragColor = vec4(sinValue);
}

void SinGraph()
{
	vec2 NewTexPos = vec2(v_Color.r * 2 * c_PI, v_Color.g*2 - 1);		// 0 ~ 2pi, -1~1
	float period = 1;
	float amp = 0.2;
	float speed = 3;
	//float sinValue = amp * sin(NewTexPos.x *period - u_Time);
	//float sinValue = amp * sin(NewTexPos.x *period - u_Time) - amp * sin(-u_Time);
	float sinValue = v_Color.r * amp * sin(NewTexPos.x *period - u_Time*speed);

	float width = 0.5;
	width = width*(1-v_Color.r);
	if(sinValue < NewTexPos.y + width && sinValue > NewTexPos.y - width )
	{
	FragColor = vec4((sinValue+1)/2);
	}
	else{
		//FragColor = vec4(0);
		discard;
	}

	

}

void main()
{
	SinGraph();
	//Circles();
}

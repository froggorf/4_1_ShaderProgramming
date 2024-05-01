#version 330

layout(location=0) out vec4 FragColor;

uniform sampler2D uTexSampler;

in vec2 v_Texpos;

void Mirror()
{
	vec2 newTexPos = v_Texpos;	
	//if(newTexPos.y<0.5) newTexPos.y*=-1;
	newTexPos.y = 1 - abs(newTexPos.y*2-1);
	//newTexPos.y = 
	FragColor = texture(uTexSampler,newTexPos);
}

void BGR()
{
	vec2 newTexPos = v_Texpos;	

	newTexPos.x = fract(v_Texpos.x*3);
	newTexPos.y = v_Texpos.y /3 + (2- floor(v_Texpos.x*3))/3;
	//newTexPos.y *=0.33;
	FragColor = texture(uTexSampler,newTexPos);
}

void BGRColumn()
{
	float tx = v_Texpos.x;

	float ty = fract(v_Texpos.y*3)/3 + ((2- floor(v_Texpos.y*3)))/3;

	FragColor = texture(uTexSampler,vec2(tx,ty));
}

void RRR()
{
	float tx = v_Texpos.x*2 + (1- floor(v_Texpos.y*2))*0.5;
	float ty = v_Texpos.y*2;

	FragColor = texture(uTexSampler,vec2(tx,ty));
}

void P6()
{
	float tx = v_Texpos.x*2;
	float ty = v_Texpos.y*2 + ( floor(v_Texpos.x*2))*0.5;

	FragColor = texture(uTexSampler,vec2(tx,ty));
}

void main()
{
	//FragColor = vec4(1,1,1,1);
	//FragColor = vec4(v_Texpos,0,1);
	//FragColor = texture(uTexSampler,v_Texpos);
	//Mirror();
	P6();
}




//{
//float tx = fract(0.5*(floor(v_Texpos.y * 2) + 1)) +fract(v_Texpos.x*2) ;
//	float ty = fract(v_Texpos.y * 2);
//	vec2 newTexPos = vec2(tx,ty);
//	FragColor = texture(uTexSampler,newTexPos);
//}
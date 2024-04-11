#include "stdafx.h"
#include "Renderer.h"

Renderer::Renderer(int windowSizeX, int windowSizeY)
{
	Initialize(windowSizeX, windowSizeY);
}


Renderer::~Renderer()
{
}

void Renderer::Initialize(int windowSizeX, int windowSizeY)
{
	//Set window size
	m_WindowSizeX = windowSizeX;
	m_WindowSizeY = windowSizeY;

	//Load shaders
	m_SolidRectShader = CompileShaders("./Shaders/SolidRect.vs", "./Shaders/SolidRect.fs");
	m_ParticleShader = CompileShaders("./Shaders/particle.vs", "./Shaders/particle.fs");
	m_CloudShader = CompileShaders("./Shaders/Cloud.vs", "./Shaders/Cloud.fs");
	//Create VBOs
	CreateVertexBufferObjects();

	// Create Particle Cloud
	CreateParticleCloud(100000);

	if (m_SolidRectShader > 0 && m_VBORect > 0)
	{
		m_Initialized = true;
	}
}

bool Renderer::IsInitialized()
{
	return m_Initialized;
}

void Renderer::CreateVertexBufferObjects()
{
	float rect[]
		=
	{
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, -1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, //Triangle1
		-1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f,  1.f / m_WindowSizeX, 1.f / m_WindowSizeY, 0.f, 1.f / m_WindowSizeX, -1.f / m_WindowSizeY, 0.f, //Triangle2
	};

	glGenBuffers(1, &m_VBORect);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);

	float vertices[] = {
		0.0f,0.0f,0.0f, 1.0f,0.0f,0.0f,1.0f,1.0f,0.0f,
	};
	glGenBuffers(1, &m_TESTVBO);	//ID 만들기
	glBindBuffer(GL_ARRAY_BUFFER, m_TESTVBO);	//작업대 위에 array 올리기
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //바인딩된 VBO에 실제 데이터 넣기

	float size = 0.05;
	float particleVertices[] = {
		-size, -size, 0,	size,-size,0,	size,size,0,
		size,size,0,	-size,size,0,		-size,-size,0
	};
	glGenBuffers(1, &m_ParticleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(particleVertices), particleVertices, GL_STATIC_DRAW);
}

void Renderer::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
	//���̴� ������Ʈ ����
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = pShaderText;
	GLint Lengths[1];
	Lengths[0] = strlen(pShaderText);
	//���̴� �ڵ带 ���̴� ������Ʈ�� �Ҵ�
	glShaderSource(ShaderObj, 1, p, Lengths);

	//�Ҵ�� ���̴� �ڵ带 ������
	glCompileShader(ShaderObj);

	GLint success;
	// ShaderObj �� ���������� ������ �Ǿ����� Ȯ��
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];

		//OpenGL �� shader log �����͸� ������
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
		printf("%s \n", pShaderText);
	}

	// ShaderProgram �� attach!!
	glAttachShader(ShaderProgram, ShaderObj);
}

bool Renderer::ReadFile(char* filename, std::string *target)
{
	std::ifstream file(filename);
	if (file.fail())
	{
		std::cout << filename << " file loading failed.. \n";
		file.close();
		return false;
	}
	std::string line;
	while (getline(file, line)) {
		target->append(line.c_str());
		target->append("\n");
	}
	return true;
}

GLuint Renderer::CompileShaders(char* filenameVS, char* filenameFS)
{
	GLuint ShaderProgram = glCreateProgram(); //�� ���̴� ���α׷� ����

	if (ShaderProgram == 0) { //���̴� ���α׷��� ����������� Ȯ��
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vs, fs;

	//shader.vs �� vs ������ �ε���
	if (!ReadFile(filenameVS, &vs)) {
		printf("Error compiling vertex shader\n");
		return -1;
	};

	//shader.fs �� fs ������ �ε���
	if (!ReadFile(filenameFS, &fs)) {
		printf("Error compiling fragment shader\n");
		return -1;
	};

	// ShaderProgram �� vs.c_str() ���ؽ� ���̴��� �������� ����� attach��
	AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

	// ShaderProgram �� fs.c_str() �����׸�Ʈ ���̴��� �������� ����� attach��
	AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	//Attach �Ϸ�� shaderProgram �� ��ŷ��
	glLinkProgram(ShaderProgram);

	//��ũ�� �����ߴ��� Ȯ��
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);

	if (Success == 0) {
		// shader program �α׸� �޾ƿ�
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error linking shader program\n" << ErrorLog;
		return -1;
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		std::cout << filenameVS << ", " << filenameFS << " Error validating shader program\n" << ErrorLog;
		return -1;
	}

	glUseProgram(ShaderProgram);
	std::cout << filenameVS << ", " << filenameFS << " Shader compiling is done."<<std::endl;

	return ShaderProgram;
}

void Renderer::DrawSolidRect(float x, float y, float z, float size, float r, float g, float b, float a)
{
	float newX, newY;

	GetGLPosition(x, y, &newX, &newY);

	//Program select
	glUseProgram(m_SolidRectShader);
	
	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Trans"), newX, newY, 0, size);
	glUniform4f(glGetUniformLocation(m_SolidRectShader, "u_Color"), r, g, b, a);

	int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBORect);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);
	

	glDisableVertexAttribArray(attribPosition);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Renderer::GetGLPosition(float x, float y, float *newX, float *newY)
{
	*newX = x * 2.f / m_WindowSizeX;
	*newY = y * 2.f / m_WindowSizeY;
}

void Renderer::CreateParticleCloud(int numParticles)
{
	float centerX, centerY;
	centerX = 0.0f;
	centerY = 0.0f;
	float size = 0.005f;
	int particleCount = numParticles;
	int vertexCount = numParticles * 6;
	int floatCount = vertexCount * (3+3+1+1+1+1+1+4);
									// x, y, z, vx, vy, vz, StartTime, LifeTime, amp, period, value, r,g,b,a

	float* vertices = NULL;
	vertices = new float[floatCount];

	// 버텍스 vx vy vz 추가
	int index = 0;
	for(int i=0; i<particleCount; ++i)
	{
		float velocityScale = 0.2f;
		float StartTime = ((float)rand() / (float)RAND_MAX) *5;
		float LifeTime = ((float)rand() / (float)RAND_MAX) * 5 + 2.0f;
		
		centerX = 0.0f;//((float)rand() / (float)RAND_MAX) * 2.f - 1.f;
		centerY = 0.0f;//((float)rand() / (float)RAND_MAX) * 2.f - 1.f;

		float vx = 0.0f;//(((float)rand() / (float)RAND_MAX)*2.0f - 1.0f)*velocityScale;
		float vy = -0.1f;//(((float)rand() / (float)RAND_MAX)*2.0f - 1.0f)*velocityScale;
		float vz = 0.0f;

		float amp = (((float)rand() / (float)RAND_MAX)-0.5f)*2.f;
		float period = ((float)rand() / (float)RAND_MAX);

		float value = ((float)rand() / (float)RAND_MAX);

		float r, g, b, a;
		r = ((float)rand() / (float)RAND_MAX);
		g = ((float)rand() / (float)RAND_MAX);
		b = ((float)rand() / (float)RAND_MAX);
		a = ((float)rand() / (float)RAND_MAX);

		vertices[index++] = centerX-size;
		vertices[index++] = centerY-size;
		vertices[index++] = 0.0f;
		vertices[index++] = vx;
		vertices[index++] = vy;
		vertices[index++] = vz;
		vertices[index++] = StartTime;
		vertices[index++] = LifeTime;
		vertices[index++] = amp;
		vertices[index++] = period;
		vertices[index++] = value;
		vertices[index++] = r;
		vertices[index++] = g;
		vertices[index++] = b;
		vertices[index++] = a;


		vertices[index++] = centerX+size;
		vertices[index++] = centerY+size;
		vertices[index++] = 0.0f;
		vertices[index++] = vx;
		vertices[index++] = vy;
		vertices[index++] = vz;
		vertices[index++] = StartTime;
		vertices[index++] = LifeTime;
		vertices[index++] = amp;
		vertices[index++] = period;
		vertices[index++] = value;
		vertices[index++] = r;
		vertices[index++] = g;
		vertices[index++] = b;
		vertices[index++] = a;

		vertices[index++] = centerX-size;
		vertices[index++] = centerY+size;
		vertices[index++] = 0.0f;
		vertices[index++] = vx;
		vertices[index++] = vy;
		vertices[index++] = vz;
		vertices[index++] = StartTime;
		vertices[index++] = LifeTime;
		vertices[index++] = amp;
		vertices[index++] = period;
		vertices[index++] = value;
		vertices[index++] = r;
		vertices[index++] = g;
		vertices[index++] = b;
		vertices[index++] = a;

		vertices[index++] = centerX - size;
		vertices[index++] = centerY - size;
		vertices[index++] = 0.0f;
		vertices[index++] = vx;
		vertices[index++] = vy;
		vertices[index++] = vz;
		vertices[index++] = StartTime;
		vertices[index++] = LifeTime;
		vertices[index++] = amp;
		vertices[index++] = period;
		vertices[index++] = value;
		vertices[index++] = r;
		vertices[index++] = g;
		vertices[index++] = b;
		vertices[index++] = a;

		vertices[index++] = centerX + size;
		vertices[index++] = centerY - size;
		vertices[index++] = 0.0f;
		vertices[index++] = vx;
		vertices[index++] = vy;
		vertices[index++] = vz;
		vertices[index++] = StartTime;
		vertices[index++] = LifeTime;
		vertices[index++] = amp;
		vertices[index++] = period;
		vertices[index++] = value;
		vertices[index++] = r;
		vertices[index++] = g;
		vertices[index++] = b;
		vertices[index++] = a;

		vertices[index++] = centerX + size;
		vertices[index++] = centerY + size;
		vertices[index++] = 0.0f;
		vertices[index++] = vx;
		vertices[index++] = vy;
		vertices[index++] = vz;
		vertices[index++] = StartTime;
		vertices[index++] = LifeTime;
		vertices[index++] = amp;
		vertices[index++] = period;
		vertices[index++] = value;
		vertices[index++] = r;
		vertices[index++] = g;
		vertices[index++] = b;
		vertices[index++] = a;
		
	}

	glGenBuffers(1, &m_ParticleCloudVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * floatCount, vertices, GL_STATIC_DRAW);
	m_ParticleCloudVertexCount = vertexCount;

	delete[] vertices;
}


void Renderer::DrawTEST()
{
	//Program select
	glUseProgram(m_SolidRectShader);
	
	int attribPosition = glGetAttribLocation(m_SolidRectShader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_TESTVBO);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, 3);
	
	glDisableVertexAttribArray(attribPosition);
	

}



void Renderer::DrawParticle()
{
	//Program select
	GLuint shader = m_ParticleShader;
	glUseProgram(shader);

	static float time = 0;
	time += 0.012;
	glUniform1f(glGetUniformLocation(shader, "u_Time"), time);

	glUniform1f(glGetUniformLocation(shader, "u_Period"), 1);


	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleVBO);
	glVertexAttribPointer(attribPosition, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(attribPosition);


}



void Renderer::DrawParticleCloud()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//Program select
	GLuint shader = m_CloudShader;
	glUseProgram(shader);
	GLuint stride = sizeof(float) * (3 + 3 + 1 + 1 + 1 + 1 + 1 + 4);
	// x, y, z, vx, vy, vz, StartTime, LifeTime, amp, period, value, r,g,b,a;


	static float time = 0;
	time += 0.012;
	glUniform1f(glGetUniformLocation(shader, "u_Time"), time);
	glUniform1f(glGetUniformLocation(shader, "u_Period"), 1);
	glUniform2f(glGetUniformLocation(shader, "u_WindAcc"), -1.0f,0.0f);


	int attribPosition = glGetAttribLocation(shader, "a_Position");
	glEnableVertexAttribArray(attribPosition);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glVertexAttribPointer(
		attribPosition, 
		3, GL_FLOAT, GL_FALSE, 
		stride,
		0);

	int StartTimeAttribPos = glGetAttribLocation(shader, "a_StartTime");
	glEnableVertexAttribArray(StartTimeAttribPos);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glVertexAttribPointer(
		StartTimeAttribPos, 
		1, GL_FLOAT, GL_FALSE, 
		stride,
		(GLvoid*)(sizeof(float)*6));

	int VelocityAttribPos = glGetAttribLocation(shader, "a_Velocity");
	glEnableVertexAttribArray(VelocityAttribPos);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glVertexAttribPointer(
		VelocityAttribPos,
		3, GL_FLOAT, GL_FALSE,
		stride,
		(GLvoid*)(sizeof(float) * 3));

	int LifeTimeAttribPos = glGetAttribLocation(shader, "a_LifeTime");
	glEnableVertexAttribArray(LifeTimeAttribPos);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glVertexAttribPointer(
		LifeTimeAttribPos,
		1, GL_FLOAT, GL_FALSE,
		stride,
		(GLvoid*)(sizeof(float) * 7));

	int AmpAttribPos = glGetAttribLocation(shader, "a_Amp");
	glEnableVertexAttribArray(AmpAttribPos);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glVertexAttribPointer(
		AmpAttribPos,
		1, GL_FLOAT, GL_FALSE,
		stride,
		(GLvoid*)(sizeof(float) * 8));

	int PeriodAttribPos = glGetAttribLocation(shader, "a_Period");
	glEnableVertexAttribArray(PeriodAttribPos);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glVertexAttribPointer(
		PeriodAttribPos,
		1, GL_FLOAT, GL_FALSE,
		stride,
		(GLvoid*)(sizeof(float) * 9));

	int ValueAttribPos = glGetAttribLocation(shader, "a_Value");
	glEnableVertexAttribArray(ValueAttribPos);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glVertexAttribPointer(
		ValueAttribPos,
		1, GL_FLOAT, GL_FALSE,
		stride,
		(GLvoid*)(sizeof(float) * 10));

	int rgbaAttribPos = glGetAttribLocation(shader, "a_Color");
	glEnableVertexAttribArray(rgbaAttribPos);
	glBindBuffer(GL_ARRAY_BUFFER, m_ParticleCloudVBO);
	glVertexAttribPointer(
		rgbaAttribPos,
		4, GL_FLOAT, GL_FALSE,
		stride,
		(GLvoid*)(sizeof(float) * 11));

	glDrawArrays(GL_TRIANGLES, 0, m_ParticleCloudVertexCount);

	glDisableVertexAttribArray(attribPosition);

	glDisable(GL_BLEND);
}

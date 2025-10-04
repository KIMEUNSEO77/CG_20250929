#define _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "Shape.h"

void make_vertexShaders();
void make_fragmentShaders();

GLuint make_shaderProgram();
GLvoid drawScene();
GLvoid Reshape(int w, int h);

GLint width = 1000, height = 800;
GLuint shaderProgramID;
GLuint vertexShader;
GLuint fragmentShader;

int dragIdx = -1;      // 현재 드래그 중인 인덱스
float mouseX, mouseY;  // 마우스 좌표
bool dragging = false; // 드래그 중인지 여부

char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;
	fptr = fopen(file, "rb");
	if (!fptr)
		return NULL;
	fseek(fptr, 0, SEEK_END);
	length = ftell(fptr);
	buf = (char*)malloc(length + 1);
	fseek(fptr, 0, SEEK_SET);
	fread(buf, length, 1, fptr);
	fclose(fptr);
	buf[length] = 0;
	return buf;
	// Open file for reading 
	// Return NULL on failure 
	// Seek to the end of the file 
	// Find out how many bytes into the file we are 
	// Allocate a buffer for the entire length of the file and a null terminator 
	// Go back to the beginning of the file 
	// Read the contents of the file in to the buffer 
	// Close the file 
	// Null terminator 
	// Return the buffer 
}

// 마우스 클릭한 좌표 정규화
void PixelTrans(int px, int py, float& nx, float& ny)
{
	float w = (float)glutGet(GLUT_WINDOW_WIDTH);
	float h = (float)glutGet(GLUT_WINDOW_HEIGHT);
	nx = (px / w) * 2.0f - 1.0f;
	ny = 1.0f - (py / h) * 2.0f;
}

// 마우스 좌표 화면 밖으로 못 나가게
void MouseInside(Shape& s)
{
	/*
	float w = (r.width * r.scale) * 0.5f;
	float h = (r.height * r.scale) * 0.5f;

	if (r.posX < -1.0f + w) r.posX = -1.0f + w;
	if (r.posX > 1.0f - w) r.posX = 1.0f - w;
	if (r.posY < -1.0f + h) r.posY = -1.0f + h;
	if (r.posY > 1.0f - h) r.posY = 1.0f - h;
	*/
}

int GetShapeIdx(float mx, float my)
{
	for (int i = 0; i < shapes.size(); i++)
	{
		Shape& shape = shapes[i];
		for (int j = 0; j < shape.vertices.size(); j++)
		{
			glm::vec3 v = shape.vertices[j];
			float dx = mx - v.x;
			float dy = my - v.y;
			float dist = sqrt(dx * dx + dy * dy);
			if (dist < 0.05f) // 클릭 허용 범위
			{
				return i;
			}
		}
	}
	return -1;
}

void Mouse(int button, int state, int x, int y)
{
	float nx, ny;  // 마우스가 클릭한 좌표를 정규화
	PixelTrans(x, y, nx, ny);

	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			dragging = true;
			mouseX = nx;
			mouseY = ny;
			int hit = GetShapeIdx(nx, ny);
			if (hit != -1) dragIdx = hit;
			else dragging = false;
		}
		else if (state == GLUT_UP)
		{
			dragging = false;
			dragIdx = -1;
		}
	}
}

void MouseDrag(int x, int y)
{
	if (!dragging) return;

	float nx, ny;
	PixelTrans(x, y, nx, ny);

}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'q':
		exit(0);
		break;
	}
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("Tesk_13");

	glewExperimental = GL_TRUE;
	glewInit();

	for (int i = 0; i < 3; i++)
	{
		AddLine();
		AddTriangle();
		AddRect();
		AddPentagon();
		AddPoint();
	}

	make_vertexShaders();
	make_fragmentShaders();
	shaderProgramID = make_shaderProgram();

	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(MouseDrag);
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

void make_vertexShaders()
{
	GLchar* vertexSource;
	vertexSource = filetobuf("vertex.glsl");
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "Error: vertex shader            \n" << errorLog << std::endl;
		return;
	}
}

void make_fragmentShaders()
{
	GLchar* fragmentSource;
	fragmentSource = filetobuf("fragment.glsl");
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: frag_shader            \n" << errorLog << std::endl;
		return;
	}
}

GLuint make_shaderProgram()
{
	GLint result;
	GLchar* errorLog = NULL;
	GLuint shaderID;
	shaderID = glCreateProgram();

	glAttachShader(shaderID, vertexShader);
	glAttachShader(shaderID, fragmentShader);

	glLinkProgram(shaderID);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shaderID, 512, NULL, errorLog);
		std::cerr << "ERROR: shader program          \n" << errorLog << std::endl;
		return false;
	}
	glUseProgram(shaderID);
	return shaderID;
}

GLvoid drawScene()
{
	GLfloat rColor, gColor, bColor;
	rColor = gColor = 1.0;
	bColor = 1.0;
	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgramID);
	GLint locColor = glGetUniformLocation(shaderProgramID, "uColor");

	for (int i = 0; i < shapes.size(); i++)
	{
		Shape& shape = shapes[i];
		glBindVertexArray(shape.VAO);
		glUniform4fv(locColor, 1, glm::value_ptr(shape.colors[0]));

		std::vector<glm::vec3> drawVerts = shape.vertices;

		glBindVertexArray(shape.VAO);
		glUniform4fv(locColor, 1, glm::value_ptr(shape.colors[0]));

		switch (shape.type)
		{
		case LINE:
			glLineWidth(2.0f);
			glDrawArrays(GL_LINES, 0, shape.vertices.size());
			break;
		case TRIANGLE:
			glDrawArrays(GL_TRIANGLES, 0, shape.vertices.size());
			break;
		case RECTANG:
			glDrawElements(GL_TRIANGLES, shape.indices.size(), GL_UNSIGNED_INT, 0);
			break;
		case PENTAGON:
			glDrawElements(GL_TRIANGLES, shape.indices.size(), GL_UNSIGNED_INT, 0);
			break;
		case HOLL:
			glDrawElements(GL_TRIANGLES, shape.indices.size(), GL_UNSIGNED_INT, 0);
			break;
		}
		glBindVertexArray(0);
	}

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}
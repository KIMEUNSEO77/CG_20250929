#define _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <glew.h>
#include <freeglut.h>
#include <freeglut_ext.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <random>

// 점, 삼각형, 사각형, 오각형, 점
enum ShapeType { LINE, TRIANGLE, RECTANG, PENTAGON, HOLL };

struct Shape
{
	ShapeType type;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec4> colors;
	std::vector<GLuint> indices;
	GLuint VAO = 0, VBO = 0, EBO = 0;
	int pointCounts = 0;   // 꼭짓점 개수
	bool moving = false;   // 애니메이션 중인지
};

std::vector<Shape> shapes;

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

float randFloat(float a, float b)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(a, b);
	return dis(gen);
}

void UpdateShape(Shape& shape)
{
	if (!shape.VAO) glGenVertexArrays(1, &shape.VAO);
	if (!shape.VBO) glGenBuffers(1, &shape.VBO);
	if (!shape.EBO && !shape.indices.empty()) glGenBuffers(1, &shape.EBO);

	glBindVertexArray(shape.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, shape.VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, shape.EBO);

	glBufferData(GL_ARRAY_BUFFER, shape.vertices.size() * sizeof(glm::vec3), shape.vertices.data(), GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, shape.indices.size() * sizeof(GLuint), shape.indices.data(), GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// line
void AddLine()
{
	Shape line;
	line.type = LINE;

	float x1 = randFloat(-1.0f, 0.7f);
	float y1 = randFloat(-0.9f, 0.9f);
	float r, g, b;
	r = randFloat(0.0f, 1.0f);
	g = randFloat(0.0f, 1.0f);
	b = randFloat(0.0f, 1.0f);

	line.vertices =
	{
		glm::vec3(x1, y1, 0.0f), glm::vec3(x1 + 0.2f, y1, 0.0f)
	};
	line.colors =
	{
		glm::vec4(r, g, b, 1.0f)
	};
	UpdateShape(line);
	shapes.push_back(line);
}

// triangle
void AddTriangle()
{
	Shape triangle;
	triangle.type = TRIANGLE;

	float x1 = randFloat(-0.6f, 0.6f);
	float y1 = randFloat(-0.9f, 0.6f);
	float x2 = x1 - 0.1f;
	float x3 = x1 + 0.1f;
	float y2 = y1 - 0.1f;

	float r, g, b;
	r = randFloat(0.0f, 1.0f);
	g = randFloat(0.0f, 1.0f);
	b = randFloat(0.0f, 1.0f);

	triangle.vertices =
	{
		glm::vec3(x1, y1, 0.0f), glm::vec3(x2, y2, 0.0f),
		glm::vec3(x3, y2, 0.0f)
	};
	triangle.colors =
	{
		glm::vec4(r, g, b, 1.0f)
	};
	UpdateShape(triangle);
	shapes.push_back(triangle);
}

// rect
void AddRect()
{
	Shape rect;
	rect.type = RECTANG;
	float x1 = randFloat(-0.9f, 0.6f);
	float y1 = randFloat(-0.6f, 0.9f);
	float x2 = x1 + 0.2f;
	float y2 = y1;
	float x3 = x1 + 0.2f;
	float y3 = y1 - 0.2f;
	float x4 = x1;
	float y4 = y1 - 0.2f;
	float r, g, b;
	r = randFloat(0.0f, 1.0f);
	g = randFloat(0.0f, 1.0f);
	b = randFloat(0.0f, 1.0f);

	rect.vertices =
	{
		glm::vec3(x1, y1, 0.0f), glm::vec3(x2, y2, 0.0f),
		glm::vec3(x3, y3, 0.0f), glm::vec3(x4, y4, 0.0f)
	};
	rect.colors =
	{
		glm::vec4(r, g, b, 1.0f)
	};
	rect.indices =
	{
		0, 1, 2,
		2, 3, 0
	};
	UpdateShape(rect);
	shapes.push_back(rect);
}

// Pentagon
void AddPentagon()
{
	Shape pentagon;
	pentagon.type = PENTAGON;

	float x1 = randFloat(-0.7f, 0.7f);
	float y1 = randFloat(-0.9f, 0.6f);
	float x2 = x1 - 0.1f;
	float y2 = y1 - 0.1f;
	float x3 = x1 + 0.1f;
	float y3 = y1 - 0.1f;
	float x4 = x1 - 0.05f;
	float y4 = y1 - 0.2f;
	float x5 = x1 + 0.05f;
	float y5 = y1 - 0.2f;
	float r, g, b;
	r = randFloat(0.0f, 1.0f);
	g = randFloat(0.0f, 1.0f);
	b = randFloat(0.0f, 1.0f);
	pentagon.vertices =
	{
		glm::vec3(x1, y1, 0.0f), glm::vec3(x2, y2, 0.0f),
		glm::vec3(x3, y3, 0.0f),
		glm::vec3(x4, y4, 0.0f), glm::vec3(x5, y5, 0.0f)
	};
	pentagon.colors =
	{
		glm::vec4(r, g, b, 1.0f)
	};
	pentagon.indices =
	{
		0, 1, 3,
		0, 2, 4,
		0, 3, 4
	};
	UpdateShape(pentagon);
	shapes.push_back(pentagon);
}

// 점
void AddPoint()
{
	Shape rect;
	rect.type = HOLL;
	float x1 = randFloat(-0.9f, 0.6f);
	float y1 = randFloat(-0.6f, 0.9f);
	float x2 = x1 + 0.02f;
	float y2 = y1;
	float x3 = x1 + 0.02f;
	float y3 = y1 - 0.02f;
	float x4 = x1;
	float y4 = y1 - 0.02f;
	float r, g, b;
	r = randFloat(0.0f, 1.0f);
	g = randFloat(0.0f, 1.0f);
	b = randFloat(0.0f, 1.0f);

	rect.vertices =
	{
		glm::vec3(x1, y1, 0.0f), glm::vec3(x2, y2, 0.0f),
		glm::vec3(x3, y3, 0.0f), glm::vec3(x4, y4, 0.0f)
	};
	rect.colors =
	{
		glm::vec4(r, g, b, 1.0f)
	};
	rect.indices =
	{
		0, 1, 2,
		2, 3, 0
	};
	UpdateShape(rect);
	shapes.push_back(rect);
}

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
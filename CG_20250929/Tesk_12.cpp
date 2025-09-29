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

void make_vertexShaders();
void make_fragmentShaders();

GLuint make_shaderProgram();
GLvoid drawScene();
GLvoid Reshape(int w, int h);

GLint width = 1000, height = 800;
GLuint shaderProgramID;
GLuint vertexShader;
GLuint fragmentShader;
GLuint axisVAO = 0, axisVBO = 0;   // line 용 vao, vbo

enum ShapeType { LINE, TRIANGLE, RECTANG, PENTAGON };

struct Shape
{
	ShapeType type;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec4> colors;
	std::vector<GLuint> indices;
	GLuint VAO = 0, VBO = 0, EBO = 0;
};

std::vector<Shape> shapes;
bool space = true;

// 사분면 구분 선 그리기
void InitCenterCross()
{
	const float verts[] = {
		-1.0f,  0.0f, 0.0f,   1.0f,  0.0f, 0.0f,   // 가로선: (-1,0) -> (1,0)
		 0.0f, -1.0f, 0.0f,   0.0f,  1.0f, 0.0f    // 세로선: (0,-1) -> (0,1)
	};

	glGenVertexArrays(1, &axisVAO);
	glGenBuffers(1, &axisVBO);

	glBindVertexArray(axisVAO);
	glBindBuffer(GL_ARRAY_BUFFER, axisVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
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
	line.vertices = 
	{
		glm::vec3(-0.7f, 0.3f, 0.0f), glm::vec3(-0.3f, 0.7f, 0.0f)
	};
	line.colors = 
	{
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
	};
	UpdateShape(line);
	shapes.push_back(line);
}

// triangle
void AddTriangle()
{
	Shape triangle;
	triangle.type = TRIANGLE;
	triangle.vertices =
	{
		glm::vec3(0.55f, 0.7f, 0.0f), glm::vec3(0.3f, 0.2f, 0.0f), glm::vec3(0.8f, 0.2f, 0.0f)
	};
	triangle.colors =
	{
		glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)
	};
	UpdateShape(triangle);
	shapes.push_back(triangle);
}

// rect
void AddRect()
{
	Shape rect;
	rect.type = RECTANG;
	rect.vertices =
	{
		glm::vec3(-0.7f, -0.3f, 0.0f), glm::vec3(-0.3f, -0.3f, 0.0f), glm::vec3(-0.3f, -0.7f, 0.0f), glm::vec3(-0.7f, -0.7f, 0.0f)
	};
	rect.colors =
	{
		glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)
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
	pentagon.vertices =
	{
		glm::vec3(0.5f, -0.2f, 0.0f), glm::vec3(0.3f, -0.4f, 0.0f), glm::vec3(0.7f, -0.4f, 0.0f),
		glm::vec3(0.4f, -0.6, 0.0f), glm::vec3(0.6f, -0.6, 0.0f)
	};
	pentagon.colors =
	{
		glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)
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

void Reset()
{
	shapes.clear();
	space = true;
	InitCenterCross();
	AddLine();
	AddTriangle();
	AddRect();
	AddPentagon();
}

// line -> triangle
void LineToTriangle(Shape& line)
{
	if (line.type != LINE || line.vertices.size() != 2) return;

	glm::vec3 start = line.vertices[0];
	glm::vec3 end = line.vertices[1];
	glm::vec3 mid = (start + end) / 2.0f + glm::vec3(0.0f, 0.3f, 0.0f);
	line.type = TRIANGLE;
	line.vertices = { start, end, mid };
	line.indices.clear();
	UpdateShape(line);
}

// triangle -> rect
void TriangleToRect(Shape& triangle)
{
	if (triangle.type != TRIANGLE || triangle.vertices.size() != 3) return;
	glm::vec3 v0 = triangle.vertices[0];
	glm::vec3 v1 = triangle.vertices[1];
	glm::vec3 v2 = triangle.vertices[2];
	glm::vec3 v3 = triangle.vertices[0] - ((v2 - v1) / 2.0f);
	glm::vec3 v4 = triangle.vertices[0] + ((v2 - v1) / 2.0f);
	triangle.type = RECTANG;
	triangle.vertices = { v0, v1, v2, v3, v4 };
	triangle.indices =
	{
		0, 1, 3,
		0, 1, 2,
		0, 2, 4
	};
	UpdateShape(triangle);
}

// rect -> pentagon
void RectToPentagon(Shape& rect)
{
	if (rect.type != RECTANG || rect.vertices.size() != 4) return;
	glm::vec3 v0 = rect.vertices[0];
	glm::vec3 v1 = rect.vertices[1];
	glm::vec3 v2 = rect.vertices[2] + glm::vec3(-0.1f, 0.1f, 0.0f);
	glm::vec3 v3 = rect.vertices[3] + glm::vec3(0.1f, 0.1f, 0.0f);
	glm::vec3 v4 = (v0 + v1) / 2.0f + glm::vec3(0.0f, 0.2f, 0.0f);
	rect.type = PENTAGON;
	rect.vertices = { v0, v1, v2, v3, v4 };
	rect.indices =
	{
		0, 3, 4,
		1, 2, 4,
		2, 3, 4
	};
	UpdateShape(rect);
}

// pentagon -> line
void PentagonToLine(Shape& pentagon)
{
	if (pentagon.type != PENTAGON || pentagon.vertices.size() != 5) return;
	glm::vec3 v0 = pentagon.vertices[0];
	glm::vec3 v1 = pentagon.vertices[2];
	pentagon.type = LINE;
	pentagon.vertices = { v0, v1 };
	pentagon.indices.clear();
	UpdateShape(pentagon);
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'l':
		LineToTriangle(shapes[0]);
		break;
	case 't':
		TriangleToRect(shapes[1]);
		break;
	case 'r':
		RectToPentagon(shapes[2]);
		break;
	case 'p':
		PentagonToLine(shapes[3]);
		break;
	case 'a':
		Reset();
		break;
	case 'q':
		exit(0);
		break;
	}
	glutPostRedisplay();
}

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("Tesk_12");

	glewExperimental = GL_TRUE;
	glewInit();

	make_vertexShaders();
	make_fragmentShaders();
	shaderProgramID = make_shaderProgram();

	InitCenterCross();
	AddLine();
	AddTriangle();
	AddRect();
	AddPentagon();

	glutKeyboardFunc(Keyboard);
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
	glUseProgram(shaderProgramID);
	GLint locColor = glGetUniformLocation(shaderProgramID, "uColor");

	// 사분면 선 그리기
	if (space)
	{
		glUniform4f(locColor, 0.0f, 0.0f, 0.0f, 1.0f);   // 검정색
		glBindVertexArray(axisVAO);
		glLineWidth(1.0f);
		glDrawArrays(GL_LINES, 0, 4);
		glBindVertexArray(0);
	}

	for (int i = 0; i < shapes.size(); i++)
	{
		Shape& shape = shapes[i];
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
		}
		glBindVertexArray(0);
	}

	glDrawArrays(GL_TRIANGLES, 0, 3);
	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}
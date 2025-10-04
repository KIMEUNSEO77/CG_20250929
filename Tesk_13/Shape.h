#pragma once
#include <vector>
#include <glew.h>
#include <freeglut.h>
#include <freeglut_ext.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
	float posX, posY;      // 중심 좌표
};

extern std::vector<Shape> shapes;

float randFloat(float a, float b);
void UpdateShape(Shape& shape);
void AddLine();
void AddTriangle();
void AddRect();
void AddPentagon();
void AddPoint();


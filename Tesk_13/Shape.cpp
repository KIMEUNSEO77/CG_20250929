#include "Shape.h"
#include <random>

std::vector<Shape> shapes;

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

	line.posX = randFloat(-0.8f, 0.8f);
	line.posY = randFloat(-0.9f, 0.9f);
	float r, g, b;
	r = randFloat(0.0f, 1.0f);
	g = randFloat(0.0f, 1.0f);
	b = randFloat(0.0f, 1.0f);

	line.vertices =
	{
		glm::vec3(line.posX - 0.1f, line.posY, 0.0f), glm::vec3(line.posX + 0.1f, line.posY, 0.0f)
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
	Shape t;
	t.type = TRIANGLE;

	t.posX = randFloat(-0.8f, 0.8f);
	t.posY = randFloat(-0.7f, 0.7f);

	float r, g, b;
	r = randFloat(0.0f, 1.0f);
	g = randFloat(0.0f, 1.0f);
	b = randFloat(0.0f, 1.0f);

	t.vertices =
	{
		glm::vec3(t.posX, t.posY + 0.1f, 0.0f), glm::vec3(t.posX - 0.1f, t.posY - 0.1f, 0.0f),
		glm::vec3(t.posX + 0.1f, t.posY - 0.1f, 0.0f)
	};
	t.colors =
	{
		glm::vec4(r, g, b, 1.0f)
	};
	UpdateShape(t);
	shapes.push_back(t);
}

// rect
void AddRect()
{
	Shape rect;
	rect.type = RECTANG;

	rect.posX = randFloat(-0.8f, 0.8f);
	rect.posY = randFloat(-0.8f, 0.8f);

	float r, g, b;
	r = randFloat(0.0f, 1.0f);
	g = randFloat(0.0f, 1.0f);
	b = randFloat(0.0f, 1.0f);

	rect.vertices =
	{
		glm::vec3(rect.posX - 0.1f, rect.posY + 0.1f, 0.0f), glm::vec3(rect.posX + 0.1f, rect.posY + 0.1f, 0.0f),
		glm::vec3(rect.posX + 0.1f, rect.posY - 0.1f, 0.0f), glm::vec3(rect.posX - 0.1f, rect.posY - 0.1f, 0.0f)
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

	pentagon.posX = randFloat(-0.8f, 0.8f);
	pentagon.posY = randFloat(-0.8f, 0.8f);

	float r, g, b;
	r = randFloat(0.0f, 1.0f);
	g = randFloat(0.0f, 1.0f);
	b = randFloat(0.0f, 1.0f);
	pentagon.vertices =
	{
		glm::vec3(pentagon.posX, pentagon.posY + 0.1f, 0.0f),
		glm::vec3(pentagon.posX - 0.1f, pentagon.posY, 0.0f), glm::vec3(pentagon.posX + 0.1f, pentagon.posY, 0.0f),
		glm::vec3(pentagon.posX - 0.05f, pentagon.posY - 0.1f, 0.0f), glm::vec3(pentagon.posX + 0.05f, pentagon.posY - 0.1f, 0.0f)
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

// Á¡
void AddPoint()
{
	Shape rect;
	rect.type = HOLL;
	rect.posX = randFloat(-0.9f, 0.9f);
	rect.posY = randFloat(-0.9f, 0.9f);
	float r, g, b;
	r = randFloat(0.0f, 1.0f);
	g = randFloat(0.0f, 1.0f);
	b = randFloat(0.0f, 1.0f);

	rect.vertices =
	{
		glm::vec3(rect.posX - 0.01f, rect.posY + 0.01f, 0.0f), glm::vec3(rect.posX + 0.01f, rect.posY + 0.01f, 0.0f),
		glm::vec3(rect.posX + 0.01f, rect.posY - 0.01f, 0.0f), glm::vec3(rect.posX - 0.01f, rect.posY - 0.01f, 0.0f)
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
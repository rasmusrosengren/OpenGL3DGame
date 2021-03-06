#include "RawModel.h"

/* PUBLIC METHODS */

RawModel::RawModel(const std::vector<float>& vertices, const std::vector<float>& texCoords, const std::vector<float>& normals, const std::vector<int>& indices)
{
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	storeDataInAttribList(0, 3, vertices);
	storeDataInAttribList(1, 2, texCoords);
	storeDataInAttribList(2, 3, normals);

	bindIndicesBuffer(indices);

	m_vertexCount = indices.size();
}

RawModel::RawModel(const std::vector<float>& vertices, const std::vector<float>& texCoords, const std::vector<float>& normals, const std::vector<float>& tangents, const std::vector<int>& indices)
{ 
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	storeDataInAttribList(0, 3, vertices);
	storeDataInAttribList(1, 2, texCoords);
	storeDataInAttribList(2, 3, normals);
	storeDataInAttribList(3, 3, tangents);

	bindIndicesBuffer(indices);

	m_vertexCount = indices.size();
}

RawModel::RawModel(const std::vector<float>& vertices, GLint dimensions)
{
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	storeDataInAttribList(0, dimensions, vertices);

	m_vertexCount = vertices.size() / dimensions;
}

RawModel::RawModel(const std::vector<float>& vertices, const std::vector<float>& texCoords)
{
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	storeDataInAttribList(0, 2, vertices);
	storeDataInAttribList(1, 2, texCoords);

	m_vertexCount = vertices.size() / 2;
}


void RawModel::bind() const
{
	glBindVertexArray(m_vao);
}

void RawModel::unbind() const
{
	glBindVertexArray(0);
}

const int RawModel::getVertexCount() const
{
	return m_vertexCount;
}

const GLuint RawModel::getID() const
{
	return m_vao;
}

/* PRIVATE METHODS */

void RawModel::bindIndicesBuffer(const std::vector<int>& indices)
{
	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);
}

void RawModel::storeDataInAttribList(int attributePosition, int size, const std::vector<float>& data)
{
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(attributePosition, size, GL_FLOAT, GL_FALSE, 0, nullptr);
}



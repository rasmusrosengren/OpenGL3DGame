#pragma once

#include <GL\glew.h>

#include <string>
#include <vector>
#include <fstream>

#include "Model.h"

const std::vector<std::string> explode(const std::string& s, const char& c)
{
	std::string buff { "" };
	std::vector<std::string> v;

	for (auto n : s)
	{
		if (n != c) buff += n; else
			if (n == c && buff != "")
			{
				v.push_back(buff); buff = "";
			}
	}
	if (buff != "") v.push_back(buff);
	return v;
}

class Loader
{
private:
	void storeDataInAttribList(int attribNum, int size, const std::vector<float>& data)
	{
		unsigned int vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data.front(), GL_STATIC_DRAW);
		glVertexAttribPointer(attribNum, size, GL_FLOAT, GL_FALSE, 0, nullptr);
	}

	void bindIndicesBuffer(const std::vector<int>& indices)
	{
		unsigned int ibo;
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices.front(), GL_STATIC_DRAW);
	}
public:
	Model loadToVao(const std::vector<float>& vertices, const std::vector<float>& texCoords, const std::vector<float>& normals, const std::vector<int>& indices)
	{
		unsigned int vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		storeDataInAttribList(0, 3, vertices);
		storeDataInAttribList(1, 2, texCoords);
		storeDataInAttribList(2, 3, normals);

		bindIndicesBuffer(indices);

		return Model(vao, indices.size());
	}

	Model loadObj(const std::string& filePath)
	{
		std::ifstream stream("res/objects/" + filePath + ".obj");
		std::string line;
		std::vector<float> vertices;
		std::vector<glm::vec2> texCoords;
		std::vector<glm::vec3> normals;
		std::vector<int> indices;
		std::vector<float> outTexCoords;
		std::vector<float> outNormals;

		bool first = true;
		double startTime = glfwGetTime();



		while (getline(stream, line))
		{
			std::vector<std::string> currentLine { explode(line, ' ') };
			if (line.substr(0, 2) == "v ")
			{
				vertices.emplace_back(::atof(currentLine[1].c_str()));
				vertices.emplace_back(::atof(currentLine[2].c_str()));
				vertices.emplace_back(::atof(currentLine[3].c_str()));
			}
			else if (line.substr(0, 3) == "vt ")
			{
				texCoords.emplace_back(glm::vec2(::atof(currentLine[1].c_str()), ::atof(currentLine[2].c_str())));
			}
			else if (line.substr(0, 3) == "vn ")
			{
				normals.emplace_back(glm::vec3(::atof(currentLine[1].c_str()), ::atof(currentLine[2].c_str()), ::atof(currentLine[3].c_str())));
			}
			else if (line.substr(0, 2) == "f ")
			{
				if (first)
				{
					first = false;
					outTexCoords.resize(vertices.size() * 2 / 3);
					outNormals.resize(vertices.size());
				}
				std::vector<std::string> vertex1 = explode(currentLine[1], '/');
				std::vector<std::string> vertex2 = explode(currentLine[2], '/');
				std::vector<std::string> vertex3 = explode(currentLine[3], '/');
				processVertex(vertex1, indices, texCoords, normals, outTexCoords, outNormals);
				processVertex(vertex2, indices, texCoords, normals, outTexCoords, outNormals);
				processVertex(vertex3, indices, texCoords, normals, outTexCoords, outNormals);
			}
		}
		std::cout << "[INFO] Loading wavefront model \"" << filePath.c_str() << "\" in " << glfwGetTime() - startTime << " seconds" << std::endl;

		return loadToVao(vertices, outTexCoords, outNormals, indices);
	}

	void processVertex(
		const std::vector<std::string>& vertexData,
		std::vector<int>& indices,
		const std::vector<glm::vec2>& texCoords,
		const std::vector<glm::vec3>& normals,
		std::vector<float>& outTexCoords,
		std::vector<float>& outNormals
	)
	{
		int currentVertexPointer = ::atoi(vertexData[0].c_str()) - 1;
		indices.push_back(currentVertexPointer);

		glm::vec2 currentTex = texCoords.at(::atoi(vertexData[1].c_str()) - 1);
		outTexCoords.at(currentVertexPointer * 2) = currentTex.x;
		outTexCoords.at(currentVertexPointer * 2 + 1) = 1 - currentTex.y;

		glm::vec3 currentNorm = normals.at(::atoi(vertexData[2].c_str()) - 1);
		outNormals.at(currentVertexPointer * 3) = currentNorm.x;
		outNormals.at(currentVertexPointer * 3 + 1) = currentNorm.y;
		outNormals.at(currentVertexPointer * 3 + 2) = currentNorm.z;
	}
};
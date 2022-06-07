#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "OBJLoader.hpp"

struct Vertex
{
public:
	Vertex(const glm::vec3& pos, const glm::vec2& texCoord, const glm::vec3& normal)
	{
		this->pos = pos;
		this->texCoord = texCoord;
		this->normal = normal;
	}

	glm::vec3* GetPos() { return &pos; }
	glm::vec2* GetTexCoord() { return &texCoord; }
	glm::vec3* GetNormal() { return &normal; }

private:
	glm::vec3 pos;
	glm::vec2 texCoord;
	glm::vec3 normal;
};

enum MeshBufferPositions
{
	POSITION_VB,
	TEXCOORD_VB,
	TEXINDEX_VB,
	NORMAL_VB,
	INDEX_VB
};

class Mesh
{
public:
	Mesh();
	//Mesh(const std::string& fileName);
	Mesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);

	void LoadModel(std::string const &file);
	void Draw() const;

	virtual ~Mesh();
protected:
private:
	static const unsigned int NUM_BUFFERS = 5;
	void operator=(const Mesh& mesh) {}

	std::string filename;

	void InitMesh(const IndexedModel& model);
	void initTextures(std::string const &mtlFileName, IndexedModel const &model);

	GLuint m_vertexArrayObject;
	GLuint m_vertexArrayBuffers[NUM_BUFFERS];
	unsigned int m_numIndices;

	GLuint tex;
};
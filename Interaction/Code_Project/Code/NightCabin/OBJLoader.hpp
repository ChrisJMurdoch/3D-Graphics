
#pragma once

#include <GL/glew.h>			// Add library to extend OpenGL to newest version
#include <GLM/glm.hpp>			// Add helper maths library
#include <string>
#include <map>
#include <vector>
using namespace std;

// Hold OpenGL reference values
struct OBJIndex
{
	unsigned int vertexIndex;
	unsigned int uvIndex;
	unsigned int normalIndex;

	bool operator<(const OBJIndex& r) const { return vertexIndex < r.vertexIndex; }
};


// Create a structure to hold an indexed model (OBJ file)
class IndexedModel
{
public:
	std::map<int, std::string> materialMapping;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> texCoords;
	std::vector<unsigned int> texIndices;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;
	void CalcNormals();
};


class OBJModel
{
public:
	std::vector<OBJIndex> OBJIndices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<float> texIndices;
	std::vector<glm::vec3> normals;
	bool hasUVs;
	bool hasNormals;

	OBJModel(const string& fileName);

	IndexedModel ToIndexedModel();

private:
	unsigned int material = 0;
	std::map<std::string, int> materials;
	std::map<int, std::string> materialMapping;
	void setMaterial(std::string &name);
	void writeMaterial(std::string &token);

	unsigned int FindLastVertexIndex(const std::vector<OBJIndex*>& indexLookup, const OBJIndex* currentIndex, const IndexedModel& result);
	void CreateOBJFace(const std::string& line);

	glm::vec2 ParseOBJVec2(const std::string& line);
	glm::vec3 ParseOBJVec3(const std::string& line);
	OBJIndex ParseOBJIndex(const std::string& token, bool* hasUVs, bool* hasNormals);
};
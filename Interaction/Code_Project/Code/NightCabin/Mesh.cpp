#include "mesh.hpp"
//#include "util.h"
//#include "debugTimer.h"
#include <map>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdlib.h>

#include "stb_image.h"

static inline std::vector<std::string> SplitString(const std::string &s, char delim);

Mesh::Mesh() {

}

/*Mesh::Mesh(const std::string& fileName)
{
	
	InitMesh(OBJModel(fileName).ToIndexedModel());
}
*/

void Mesh::LoadModel(std::string const &file)
{
	IndexedModel model = OBJModel(file+"/model.obj").ToIndexedModel();
	InitMesh(model);
	initTextures(file, model);
}

std::map<std::string, std::string> materialTextureFiles(std::string const &mtlFileName)
{
	static std::string const defaultFile = std::string("BrushedIron01_4K_BaseColor.png");
	std::ifstream file;
	file.open(mtlFileName.c_str());
	std::string line;
	std::map<std::string, std::string> matMap;
	std::string mat = std::string("NoMat");
	bool gettingMat = true;
	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);

			unsigned int lineLength = (unsigned int)line.length();

			if (lineLength < 2)
				continue;

			std::vector<std::string> tokens =SplitString(std::string(line), ' ');
			if (tokens[0]=="newmtl")
			{
				if (!gettingMat)
					matMap[mat] = defaultFile;
				mat = tokens[1];
				gettingMat = false;
			} else if (tokens[0]=="map_Kd")
			{
				matMap[mat] = tokens[1];
				gettingMat = true;
			}
		}
	}
	else
	{
		std::cerr << "Unable to open " << mtlFileName << std::endl;
	}
	return matMap;
}

// With help from https://ferransole.wordpress.com/2014/06/09/array-textures/
void Mesh::initTextures(std::string const &mtlFolder, IndexedModel const &model)
{
	std::string mtlFileName = mtlFolder+"/model.mtl";

	// Map materials to their texture files
	std::map<std::string, std::string> matFiles = materialTextureFiles(mtlFileName);

	// Create texture array
	tex = 0;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D_ARRAY, tex);
	glTexStorage3D( GL_TEXTURE_2D_ARRAY, 1, GL_RGB8, 1024, 1024, matFiles.size() );

	// Generate and assign textures to the array
	std::cout << std::endl << "Loading material textures..." << std::endl;
	for (int i=0; i<model.materialMapping.size(); i++)
	{
		// Get material name and file from ID
		std::string matName = model.materialMapping.find(i)->second;
		std::string matFile = matFiles[matName];
		std::cout << i << " ";

		// Load image
		int width, height, nrChannels;

		unsigned char *data1 = stbi_load((mtlFolder+"/"+matFile).c_str(), &width, &height, &nrChannels, 0);

		// Upload data to the GPU
		glTexSubImage3D( GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, 1024, 1024, 1, GL_RGB, GL_UNSIGNED_BYTE, data1 );
	}
	std::cout << std::endl << "Done..." << std::endl << std::endl;

	// Set basic option
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Mesh::InitMesh(const IndexedModel& model)
{
	m_numIndices = (unsigned int) model.indices.size();

	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);

	glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.positions[0]) * model.positions.size(), &model.positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.texCoords[0]) * model.texCoords.size(), &model.texCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXINDEX_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.texIndices[0]) * model.texIndices.size(), &model.texIndices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_UNSIGNED_INT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.normals[0]) * model.normals.size(), &model.normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_VB]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(model.indices[0]) * model.indices.size(), &model.indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

Mesh::Mesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices)
{
	IndexedModel model;

	for (unsigned int i = 0; i < numVertices; i++)
	{
		model.positions.push_back(*vertices[i].GetPos());
		model.texCoords.push_back(*vertices[i].GetTexCoord());
		model.normals.push_back(*vertices[i].GetNormal());
	}

	for (unsigned int i = 0; i < numIndices; i++)
		model.indices.push_back(indices[i]);

	InitMesh(model);
}

Mesh::~Mesh()
{
	glDeleteBuffers(NUM_BUFFERS, m_vertexArrayBuffers);
	glDeleteVertexArrays(1, &m_vertexArrayObject);
}

void Mesh::Draw() const
{
	glBindTexture(GL_TEXTURE_2D_ARRAY, tex);

	glBindVertexArray(m_vertexArrayObject);

	glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0);
	//glDrawElementsBaseVertex(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0, 0);

	glBindVertexArray(0);
}

static inline std::vector<std::string> SplitString(const std::string &s, char delim)
{
	std::vector<std::string> elems;

	const char *cstr = s.c_str();
	unsigned int strLength = (unsigned int)s.length();
	unsigned int start = 0;
	unsigned int end = 0;

	while (end <= strLength)
	{
		while (end <= strLength)
		{
			if (cstr[end] == delim)
				break;
			end++;
		}

		elems.push_back(s.substr(start, end - start));
		start = end + 1;
		end = start;
	}

	return elems;
}
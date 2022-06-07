#pragma once

#include "Mesh.hpp"
#include <GLM/glm.hpp>

class Instance
{
private:
	Mesh const &mesh;
	GLuint const prog;
	glm::vec3 position;
	float yaw;
public:
	Instance(Mesh const &mesh, GLuint const prog);
	void render(glm::mat4 const &view, glm::mat4 const &projection, glm::vec3 const &camera) const;
	void move(glm::vec3 const &move);
	void moveTo(glm::vec3 const &move);
	void animate(glm::vec3 const &a, glm::vec3 const &b, float interpolant);
	void print() const;
};


#include <GLM/gtx/transform.hpp>
#include <iostream>

#include "instance.hpp"

Instance::Instance(Mesh const &mesh, GLuint const prog) : mesh(mesh), prog(prog), position(0.0f), yaw(0)
{

}

void Instance::render(glm::mat4 const &view, glm::mat4 const &projection, glm::vec3 const &camera) const
{
	glUseProgram(prog);

	glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
	model = glm::rotate(model, yaw, glm::vec3(0.0f, 1.0f, 0.0f));

	glUniformMatrix4fv(glGetUniformLocation(prog, "model_matrix"), 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(prog, "view_matrix"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(prog, "proj_matrix"), 1, GL_FALSE, &projection[0][0]);
	glUniform3f(glGetUniformLocation(prog, "view_pos"), camera.x, camera.y, camera.z);

	mesh.Draw();
}

void Instance::move(glm::vec3 const &move)
{
	position += move;
}

void Instance::moveTo(glm::vec3 const &move)
{
	position = move;
}

void Instance::animate(glm::vec3 const &a, glm::vec3 const &b, float interpolant)
{
	position = a + (b-a) * interpolant;
}

void Instance::print() const
{
	std::cout << position.x << ", " << position.y << ", " << position.z << std::endl;
}

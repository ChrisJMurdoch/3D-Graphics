
#include <string>
#include <vector>

#include <iostream>
#include <fstream>
#include <sstream>

#include "shader.hpp"

std::string readShader(std::string const &name);
bool checkErrorShader(GLuint shader);

GLuint loadProgram(std::string const &vert, std::string const &frag)
{
	GLuint program = glCreateProgram();

	std::string vs_text = readShader("shaders/vs_model.glsl"); const char *vs_source = vs_text.c_str();
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vs_source, NULL);
	glCompileShader(vs);
	if (checkErrorShader(vs))
		return 0;
	glAttachShader(program, vs);

	std::string fs_text = readShader("shaders/fs_model.glsl"); const char *fs_source = fs_text.c_str();
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fs_source, NULL);
	glCompileShader(fs);
	if (checkErrorShader(fs))
		return 0;
	glAttachShader(program, fs);

	glLinkProgram(program);
	glUseProgram(program);

	return program;
}

std::string readShader(std::string const &name) {
	std::string vs_text;
	std::ifstream vs_file(name);

	std::string vs_line;
	if (vs_file.is_open()) {

		while (getline(vs_file, vs_line)) {
			vs_text += vs_line;
			vs_text += '\n';
		}
		vs_file.close();
	}
	return vs_text;
}

bool checkErrorShader(GLuint shader) {
	// Get log lenght
	GLint maxLength;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

	// Init a string for it
	std::vector<GLchar> errorLog(maxLength);

	if (maxLength > 1) {
		// Get the log file
		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

		std::cout << "--------------Shader compilation error-------------\n";
		std::cout << errorLog.data();
	}

	return maxLength > 1;
}

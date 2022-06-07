// 3D Graphics and Animation - Main Template
// This uses Visual Studio 2019 and other libraries - see notes in code and linker files.
// Last Changed 05/10/2021

#pragma comment(linker, "/NODEFAULTLIB:MSVCRT")
#define GLM_ENABLE_EXPERIMENTAL
#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

#include <GL/glew.h>			// Add library to extend OpenGL to newest version
#include <GLFW/glfw3.h>			// Add library to launch a window
#include <GLM/glm.hpp>			// Add helper maths library
#include <GLM/gtx/transform.hpp>

#include <stb_image.h>			// Add library to load images for textures

#include <vector>
#include <math.h>

#include "Instance.hpp"
#include "Mesh.hpp"				// Simplest mesh holder and OBJ loader - can update more - from https://github.com/BennyQBD/ModernOpenGLTutorial
#include "shader.hpp"


// MAIN FUNCTIONS
void setupRender();
void startup();
void update(GLfloat currentTime);
void render(GLfloat currentTime);
void endProgram();

// HELPER FUNCTIONS OPENGL
void hintsGLFW();
string readShader(string name);
bool checkErrorShader(GLuint shader);
void errorCallbackGLFW(int error, const char* description);
void debugGL();
static void APIENTRY openGLDebugCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const GLvoid* userParam);

// CALLBACK FUNCTIONS FOR WINDOW
void onResizeCallback(GLFWwindow* window, int w, int h);
void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void onMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void onMouseMoveCallback(GLFWwindow* window, double x, double y);
void onMouseWheelCallback(GLFWwindow* window, double xoffset, double yoffset);


// VARIABLES
GLFWwindow*		window;											// Keep track of the window
int				windowWidth = 640;				
int				windowHeight = 480;
bool			running = true;									// Are we still running?
glm::mat4		proj_matrix;									// Projection Matrix
float           aspect = (float)windowWidth / (float)windowHeight;
float			fovy = 45.0f;
bool			keyStatus[1024];
GLfloat			deltaTime = 0.0f;
GLfloat			lastTime = 0.0f;
GLuint			program;
GLint			proj_location;
glm::vec3		modelPosition;
glm::vec3		modelRotation;

glm::vec3		cameraPosition = glm::vec3(0.0f, 4.75f, 1.5f);
glm::vec3		cameraDirection = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3		worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
float const sens=0.1f, speed=0.04f;

Mesh static_scene, workbench_vice, toolbox_drawer;
std::vector<Instance> instances;

bool flashlight = false;

float yaw=-90, pitch=0;

//  From https://learnopengl.com/Getting-started/Camera
void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
	static float lastX=xpos, lastY=ypos;
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos; lastY = ypos;

	// Camera rotation
	yaw += sens * xoffset;
	pitch += sens * yoffset;
	pitch = min(max(pitch, -89.9f), 89.9f);
}

inline float t(float freq)
{
	return ( std::sin(glfwGetTime()*freq) + 1.0f ) / 2.0f;
}

int main()
{
	if (!glfwInit()) {							// Checking for GLFW
		cout << "Could not initialise GLFW...";
		return 0;
	}

	glfwSetErrorCallback(errorCallbackGLFW);	// Setup a function to catch and display all GLFW errors.

	hintsGLFW();								// Setup glfw with various hints.		

												// Start a window using GLFW
	string title = "My OpenGL Application";

	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	// Fullscreen
	// windowWidth = mode->width; windowHeight = mode->height;
	// window = glfwCreateWindow(windowWidth, windowHeight, title.c_str(), glfwGetPrimaryMonitor(), NULL); // fullscreen
	// Normal
	window = glfwCreateWindow(windowWidth, windowHeight, title.c_str(), NULL, NULL);

	if (!window) {								// Window or OpenGL context creation failed
		cout << "Could not initialise GLFW...";
		endProgram();
		return 0;
	}

	glfwMakeContextCurrent(window);				// making the OpenGL context current

												// Start GLEW (note: always initialise GLEW after creating your window context.)
	glewExperimental = GL_TRUE;					// hack: catching them all - forcing newest debug callback (glDebugMessageCallback)
	GLenum errGLEW = glewInit();
	if (GLEW_OK != errGLEW) {					// Problems starting GLEW?
		cout << "Could not initialise GLEW...";
		endProgram();
		return 0;
	}

	debugGL();									// Setup callback to catch openGL errors.	

												// Setup all the message loop callbacks.
	glfwSetWindowSizeCallback(window, onResizeCallback);		// Set callback for resize
	glfwSetKeyCallback(window, onKeyCallback);					// Set Callback for keys
	glfwSetMouseButtonCallback(window, onMouseButtonCallback);	// Set callback for mouse click
	glfwSetCursorPosCallback(window, onMouseMoveCallback);		// Set callback for mouse move
	glfwSetScrollCallback(window, onMouseWheelCallback);		// Set callback for mouse wheel.
																//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);	// Set mouse cursor. Fullscreen
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	setupRender();								// setup some render variables.
	startup();									// Setup all necessary information for startup (aka. load texture, shaders, models, etc).

	do {										// run until the window is closed
		GLfloat currentTime = (GLfloat)glfwGetTime();		// retrieve timelapse
		deltaTime = currentTime - lastTime;		// Calculate delta time
		lastTime = currentTime;					// Save for next frame calculations.
		glfwPollEvents();						// poll callbacks
		update(currentTime);					// update (physics, animation, structures, etc)
		render(currentTime);					// call render function.

		glfwSwapBuffers(window);				// swap buffers (avoid flickering and tearing)

		running &= (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE);	// exit if escape key pressed
		running &= (glfwWindowShouldClose(window) != GL_TRUE);
	} while (running);

	endProgram();			// Close and clean everything up...

	cout << "\nPress any key to continue...\n";
	cin.ignore(); cin.get(); // delay closing console to read debugging errors.

	return 0;
}

void errorCallbackGLFW(int error, const char* description) {
	cout << "Error GLFW: " << description << "\n";
}

void hintsGLFW() {
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);			// Create context in debug mode - for debug message callback
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
}

void endProgram() {
	glfwMakeContextCurrent(window);		// destroys window handler
	glfwTerminate();	// destroys all windows and releases resources.
}

void setupRender() {
	glfwSwapInterval(1);	// Ony render when synced (V SYNC)

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glEnable(GL_MULTISAMPLE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_STEREO, GL_FALSE);
}

void startup() {

	program = loadProgram("shaders/vs_model.glsl", "shaders/fs_model.glsl");

	static_scene.LoadModel("models/static_scene");
	instances.push_back(Instance(static_scene, program));

	workbench_vice.LoadModel("models/bench_vice");
	instances.push_back(Instance(workbench_vice, program));

	toolbox_drawer.LoadModel("models/toolbox_drawer");
	instances.push_back(Instance(toolbox_drawer, program));

	// Start from the centre
	modelPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	modelRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	
	// A few optimizations.
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// Calculate proj_matrix for the first time.
	aspect = (float)windowWidth / (float)windowHeight;
	proj_matrix = glm::perspective(glm::radians(fovy), aspect, 0.1f, 1000.0f);
}

void update(GLfloat currentTime) {

	// From https://learnopengl.com/Getting-started/Camera
	cameraDirection.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraDirection.y = sin(glm::radians(pitch));
	cameraDirection.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraDirection = glm::normalize(cameraDirection);

	glm::vec3 cameraRight = speed * glm::normalize(glm::cross(worldUp, cameraDirection));
	glm::vec3 flatCameraForward = speed * glm::normalize(glm::cross(worldUp, cameraRight));

	if (keyStatus[GLFW_KEY_W])	cameraPosition -= flatCameraForward;
	if (keyStatus[GLFW_KEY_S])	cameraPosition += flatCameraForward;
	if (keyStatus[GLFW_KEY_A])	cameraPosition += cameraRight;
	if (keyStatus[GLFW_KEY_D])	cameraPosition -= cameraRight;

	static bool holding = false;
	if (keyStatus[GLFW_KEY_F])
	{
		if (!holding)
		{
			flashlight ^= true;
			holding = true;
		}
	}
	else
	{
		holding = false;
	}

	// Animation
	static glm::vec3 const drawer_in=glm::vec3(-0.254155f, 0.0f, -0.47317f), drawer_out=glm::vec3(0.0155001, 0.0f, 0.0346896);
	instances[2].animate( drawer_in, drawer_out, t(1.0f) );

	static glm::vec3 const vice_in=glm::vec3(-0.729422f, 0.0f, -0.0120949f), vice_out=glm::vec3(0.0f, 0.0f, 0.0f);
	instances[1].animate(vice_in, vice_out, t(0.5f));
}

void render(GLfloat currentTime) {

	glViewport(0, 0, windowWidth, windowHeight);

	// Use our shader programs
	glUseProgram(program);

	// Clear colour buffer
	glm::vec4 backgroundColor = glm::vec4(0.05f, 0.05f, 0.1f, 1.0f);
	glClearBufferfv(GL_COLOR, 0, &backgroundColor[0]);
	glUniform3f(glGetUniformLocation(program, "background_colour"), backgroundColor.x, backgroundColor.y, backgroundColor.z);

	// Clear deep buffer
	static const GLfloat one = 1.0f; glClearBufferfv(GL_DEPTH, 0, &one);

	// Enable blend
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Setup camera
	glm::vec3 cameraTarget = cameraPosition + cameraDirection;
	glm::vec3 cameraUp = worldUp;
	glm::mat4 viewMatrix = glm::lookAt( cameraPosition, cameraTarget, cameraUp );

	// Do some translations, rotations and scaling
	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), modelPosition);
	modelMatrix = glm::rotate(modelMatrix, modelRotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, modelRotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f, 0.2f, 0.2f));

	glm::mat4 mv_matrix = viewMatrix * modelMatrix;

	// Flashlight
	glUniform1i(glGetUniformLocation(program, "flashlight"), (int)flashlight);

	for (Instance const &instance : instances)
	{
		instance.render(viewMatrix, proj_matrix, cameraPosition);
	}
}

void onResizeCallback(GLFWwindow* window, int w, int h) {
	windowWidth = w;
	windowHeight = h;

	if (windowWidth > 0 && windowHeight > 0) { // Avoid issues when minimising window - it gives size of 0 which fails division.
		aspect = (float)w / (float)h;
		proj_matrix = glm::perspective(glm::radians(fovy), aspect, 0.1f, 1000.0f);
	}
}

void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) keyStatus[key] = true;
	else if (action == GLFW_RELEASE) keyStatus[key] = false;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void onMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {

}

void onMouseMoveCallback(GLFWwindow* window, double x, double y) {
	int mouseX = static_cast<int>(x);
	int mouseY = static_cast<int>(y);
}

static void onMouseWheelCallback(GLFWwindow* window, double xoffset, double yoffset) {
	int yoffsetInt = static_cast<int>(yoffset);
}

void debugGL() {
	//Output some debugging information
	cout << "VENDOR: " << (char *)glGetString(GL_VENDOR) << endl;
	cout << "VERSION: " << (char *)glGetString(GL_VERSION) << endl;
	cout << "RENDERER: " << (char *)glGetString(GL_RENDERER) << endl;

	// Enable Opengl Debug
	//glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback((GLDEBUGPROC)openGLDebugCallback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true);
}

static void APIENTRY openGLDebugCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const GLvoid* userParam) {

	if (severity==GL_DEBUG_SEVERITY_NOTIFICATION)
		return; // Skip notifications

	cout << "---------------------opengl-callback------------" << endl;
	cout << "Message: " << message << endl;
	cout << "type: ";
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		cout << "ERROR";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		cout << "DEPRECATED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		cout << "UNDEFINED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		cout << "PORTABILITY";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		cout << "PERFORMANCE";
		break;
	case GL_DEBUG_TYPE_OTHER:
		cout << "OTHER";
		break;
	}
	cout << " --- ";

	cout << "id: " << id << " --- ";
	cout << "severity: ";
	switch (severity) {
	case GL_DEBUG_SEVERITY_LOW:
		cout << "LOW";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		cout << "MEDIUM";
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		cout << "HIGH";
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		cout << "NOTIFICATION";
	}
	cout << endl;
	cout << "-----------------------------------------" << endl;
}

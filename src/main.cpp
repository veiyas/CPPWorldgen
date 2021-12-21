//#include<vld.h>

#include <iostream>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "cube.hpp"
#include "shaderprogram.hpp"
#include "world.hpp"

//Initialize glfw, glad and create a window ready for rendering
GLFWwindow* initOGL();

//Handle input
void processInput(GLFWwindow* window, float deltaTime);

//Load and compile shader
ShaderProgram loadShaderProgram(const std::string& shaderName);

//Called by glfw
void mouse_callback(GLFWwindow* window, double xpos = 0, double ypos = 0);

//Necessary evil global variables
//Debug controls
float scale = 1.f;
float scaleSpeed = 30.f;

//Camera stuff
constexpr float camSpeed = 50.f;
glm::vec3 camPos = glm::vec3(0.f, 0.f, 3.f);
glm::vec3 camFront = glm::vec3(0.f, 0.f, -1.f);
glm::vec3 camUp = glm::vec3(0.f, 1.f, 0.f);

float pitch = 0.f;
float yaw = -90.f;
float lastX = 0.f;
float lastY = 0.f;
glm::vec3 direction;

//Perspective stuff
float vfov = 45;
float aspect;
float nearPlane = 1.f;
float farPlane = 10000.f;

//
World theWorld;

constexpr size_t worldLength= 200;
constexpr size_t worldWidth= 200;
constexpr size_t worldHeight = 70;


int main()
{

	/********************************************************
							Graphics
	********************************************************/
	GLFWwindow* window = initOGL();
	assert(window != nullptr);
	const GLFWvidmode* vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	ShaderProgram shaderProgram = loadShaderProgram("cube");

	auto vLocation = glGetUniformLocation(shaderProgram.getProgramId(), "V");
	auto pLocation = glGetUniformLocation(shaderProgram.getProgramId(), "P");	
	auto timeLocation = glGetUniformLocation(shaderProgram.getProgramId(), "time");
	auto widthLocation = glGetUniformLocation(shaderProgram.getProgramId(), "worldWidth");
	auto lengthLocation = glGetUniformLocation(shaderProgram.getProgramId(), "worldLength");
	auto heightLocation = glGetUniformLocation(shaderProgram.getProgramId(), "worldHeight");
	auto blockSizeLocation = glGetUniformLocation(shaderProgram.getProgramId(), "blockSizeOffset");
	
	aspect = (static_cast<float>(vidmode->width) / vidmode->height);

	glm::mat4 mMat{ 1.f };
	glm::mat4 vMat{ 1.f };
	glm::mat4 pMat{ 1.f };

	shaderProgram.bind();

	glEnable(GL_DEPTH_TEST); // Use the Z buffer
	glEnable(GL_CULL_FACE);  // Use back face culling
	glEnable(GL_MULTISAMPLE);
	glCullFace(GL_BACK);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //Wireframe

	/********************************************************
							Test Area
	********************************************************/
	theWorld = World{worldLength, worldWidth, worldHeight};

	glUniform1ui(widthLocation, worldWidth);
	glUniform1ui(lengthLocation, worldLength);
	glUniform1ui(heightLocation, worldHeight);
	glUniform1f(blockSizeLocation, Cube::getBlockSizeOffset());

	/********************************************************
							Bookkeeping
	********************************************************/
	float currentTime = 0.0;
	float deltaTime = 0.0;
	std::string fpsString;
	while (!glfwWindowShouldClose(window))
	{
		processInput(window, deltaTime);

		//Timing stuff
		fpsString = "FPS: " + std::to_string(1.0 / deltaTime);
		glfwSetWindowTitle(window, fpsString.c_str());
		currentTime = glfwGetTime();

		glUniform1f(timeLocation, currentTime);

		//Update camera
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		camFront = glm::normalize(direction);

		mMat = glm::scale(mMat, glm::vec3(1.f, scale, 1.f));
		vMat = glm::lookAt(camPos, camPos + camFront, camUp);
		pMat = glm::perspective(glm::radians(vfov), aspect, nearPlane, farPlane);

		//Copy matrices into shader and reset values
		glUniformMatrix4fv(vLocation, 1, GL_FALSE, glm::value_ptr(vMat));
		glUniformMatrix4fv(pLocation, 1, GL_FALSE, glm::value_ptr(pMat));
		scale = 1.0f;

		// Set the clear color and depth, and clear the buffers for drawing
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/********************************************************
								Render Area
		********************************************************/
		theWorld.render();

		// Swap buffers, i.e. display the image and prepare for next frame.
		glfwSwapBuffers(window);
		// Poll events
		glfwPollEvents();

		deltaTime = glfwGetTime() - currentTime;
	}
	shaderProgram.unbind();

	glfwDestroyWindow(window);
	glfwTerminate();
	_CrtDumpMemoryLeaks();
	return 0;
}

void processInput(GLFWwindow* window, float deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camPos += deltaTime * camSpeed * camFront;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camPos -= deltaTime * camSpeed * camFront;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camPos -= glm::normalize(glm::cross(camFront, camUp)) * deltaTime * camSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camPos += glm::normalize(glm::cross(camFront, camUp)) * deltaTime * camSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		camPos += deltaTime * camSpeed * camUp;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		camPos -= deltaTime * camSpeed * camUp;
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		theWorld.recreateWorld(worldLength, worldWidth, worldHeight);
		//theWorld = World{ worldLength, worldWidth, worldHeight };
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		scale -= scaleSpeed * deltaTime;
	}
}

ShaderProgram loadShaderProgram(const std::string& shaderName)
{
	//Find shader file
	auto searcher = std::filesystem::current_path();
	auto extRoot = searcher.root_path();
	std::string path;

	while (searcher.has_parent_path() && searcher != extRoot)
	{
		if (std::filesystem::exists(searcher / "rootdir.txt"))
		{
			path = searcher.string();
			break;
		}
		else
			searcher = searcher.parent_path();
	}
	path += "/src/" + shaderName;
	std::string vert, frag;

	//Read shaders into strings
	std::ifstream in_vert{ path + "vert.glsl" };
	std::ifstream in_frag{ path + "frag.glsl" };

	if (in_vert.good() && in_frag.good()) {
		vert = std::string(std::istreambuf_iterator<char>(in_vert), {});
		frag = std::string(std::istreambuf_iterator<char>(in_frag), {});
	}
	else
	{
		std::cout << "ERROR OPENING SHADER FILE: " + shaderName;
	}
	in_vert.close(); in_frag.close();

	//Compile shaders
	ShaderProgram sp{};

	sp.addShaders(vert, frag);
	sp.createAndLinkShaderProgram();

	return sp;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f; // change this value to your liking
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	camFront = glm::normalize(front);
}

void window_resized(GLFWwindow* window, int width, int height)
{
	aspect = (static_cast<float>(width) / height);
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);
}

GLFWwindow* initOGL()
{	
	
	glfwInit();
	if (!glfwInit()) {
		std::cerr << "GLFW was not initialized properly" << '\n';
		exit(-1);
	}
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	const GLFWvidmode* vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	const int wWidth = vidmode->width / 1.2;
	const int wHeight = vidmode->height / 1.2;
	GLFWwindow* window = glfwCreateWindow(wWidth, wHeight, "CPPWorldgen", nullptr, nullptr);

	if (!window)
	{
		glfwTerminate();
		return nullptr;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetWindowSizeCallback(window, window_resized);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return nullptr;
	}

	glViewport(0, 0, wWidth, wHeight); 
	std::cout << glGetString(GL_VERSION) << '\n';

	int nr_extensions = 0;
	glGetIntegerv(GL_NUM_EXTENSIONS, &nr_extensions);
	return window;
}

#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>
#include <vector>

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	//define the window
	int width = 800;
	int height = 600;

	GLFWwindow* window = glfwCreateWindow(width, height, "Ectoplasme", nullptr, nullptr);

	//main loop like in opengl
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}
	
	// clean up
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

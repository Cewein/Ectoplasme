#include <iostream>
#include <GLFW/glfw3.h>

int main()
{
	// safety check to see if glfw works
	if (!glfwInit())
	{
		std::cerr << "Could not initialize GLFW!" << std::endl;
		return 1;
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	// define the window
	int width = 800;
	int height = 600;
	GLFWwindow *window = glfwCreateWindow(width, height, "Ectoplasme", nullptr, nullptr);

	//safety check to see if the window is open
	if (!window)
	{
		std::cerr << "Could not open window!" << std::endl;
		glfwTerminate();
		return 1;
	}

	// main loop like in opengl
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}

	// clean up
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
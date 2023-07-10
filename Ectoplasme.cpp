#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>

#include <vulkan/vulkan.h>

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	//define the window
	int width = 800;
	int height = 600;
	GLFWwindow* window = glfwCreateWindow(width, height, "Ectoplasme", nullptr, nullptr);

	//create vulkan instance to use the api
	VkInstance vkInstance;
	VkApplicationInfo appInfo{};

	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Ectoplasme";
	appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
	appInfo.pEngineName = "ShinyCore";
	appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	//create info for the vulkan instance
	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	unsigned int glfwExtensionCount = 0;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	createInfo.enabledExtensionCount = glfwExtensionCount;
	createInfo.ppEnabledExtensionNames = glfwExtensions;
	createInfo.enabledExtensionCount = 0;

	VkResult result = vkCreateInstance(&createInfo, nullptr, &vkInstance);

	if(result != VK_SUCCESS) throw std::runtime_error("Vulkan instant failed to be created");

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

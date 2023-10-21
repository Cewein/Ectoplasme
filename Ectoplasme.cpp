#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>
#include <vector>

#include <vulkan/vulkan.h>

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	//define the window
	int width = 800;
	int height = 600;

	GLFWwindow* window = glfwCreateWindow(width, height, "Ectoplasme", nullptr, nullptr);

	//setup validation layer trough macro

	const std::vector<const char*> validationLayers = 
	{
		"VK_LAYER_KHRONOS_validation"
	};

#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif // NDEBUG

	//create vulkan instance to use the api
	VkInstance vkInstance;
	VkApplicationInfo appInfo{};

	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO; 
	appInfo.pApplicationName = "Ectoplasme";
	appInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
	appInfo.pEngineName = "ShinyCore";
	appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	//check the validation layer for the instance
	unsigned int layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	//define as true since no layer can be requested
	//then it must pass
	bool layerFound = true;

	//check if the layers requested are present
	for (int i = 0; i < validationLayers.size(); i++)
	{
		layerFound = false;

		for (int j = 0; j < availableLayers.size(); j++)
		{
			//compart the two layer name to see if the match
			if (strcmp(validationLayers[i], availableLayers[j].layerName) == 0)
			{
				layerFound = true;
				break;
			}
		}

		if (!layerFound) break;
	}

	if (enableValidationLayers && !layerFound) throw std::runtime_error("Validation layers requested, but not available!");

	//create info for the vulkan instance
	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	unsigned int glfwExtensionCount = 0;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	createInfo.enabledExtensionCount = glfwExtensionCount;
	createInfo.ppEnabledExtensionNames = glfwExtensions;
	createInfo.enabledExtensionCount = 0;

	//add info about the validation layer
	if (enableValidationLayers)
	{
		createInfo.enabledLayerCount = unsigned int(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else createInfo.enabledLayerCount = 0;
	
	VkResult result = vkCreateInstance(&createInfo, nullptr, &vkInstance);

	if(result != VK_SUCCESS) throw std::runtime_error("Vulkan instant failed to be created");

	//instance 

	//main loop like in opengl
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}
	
	// clean up
	vkDestroyInstance(vkInstance, nullptr);
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

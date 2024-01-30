#include <iostream>
#include <GLFW/glfw3.h>
#include <webgpu/webgpu.h>
#include <cassert>

WGPUAdapter requestAdapter(WGPUInstance instance, WGPURequestAdapterOptions const * options)
{
	struct UserData
	{
		WGPUAdapter adapter = nullptr;
		bool requestEnded = false;
	};

	UserData userData;

	//the next function is a lambda function.
	//it can be replace a a normal C function outside of the scope of this function
	auto onAdapterRequestEnded = [](WGPURequestAdapterStatus status, WGPUAdapter adapter, char const * message, void * pUserData)
	{
		UserData& userData = *reinterpret_cast<UserData*>(pUserData);
		if(status == WGPURequestAdapterStatus_Success)
		{
			userData.adapter = adapter;
		}
		else
		{
			std::cout << "Could not get webGPU adapter: " << message << std::endl;
		}
		userData.requestEnded = true;
	};

	wgpuInstanceRequestAdapter(
		instance,
		options,
		onAdapterRequestEnded,
		(void*)&userData
	);

	assert(userData.requestEnded);

	return userData.adapter;

}

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

	// safety check to see if the window is open
	if (!window)
	{
		std::cerr << "Could not open window!" << std::endl;
		glfwTerminate();
		return 1;
	}

	// createing of the wGPU descriptor
	WGPUInstanceDescriptor descriptor = {};
	descriptor.nextInChain = nullptr;

	// we create a instance of the wGPU desc
	WGPUInstance instance = wgpuCreateInstance(&descriptor);

	//safety check to see if the creation of the wGPU instance is sucessfull
	if(!instance)
	{
		std::cout << "Could not instanciate wGPU" << std::endl;
		return 1;
	}
	
	std::cout << "WGPU instance: " << instance << std::endl;

	//getting the webGPU adapter
	std::cout << "Requesting adapter ! "  << std::endl;

	WGPURequestAdapterOptions adapterOptions = {};
	WGPUAdapter adapter = requestAdapter(instance, &adapterOptions);

	std::cout << "Got adapter: " << adapter << std::endl;

	// main loop like in opengl
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}

	// clean up
	// wGPU
	wgpuAdapterRelease(adapter);
	wgpuInstanceRelease(instance);

	// GLFW
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
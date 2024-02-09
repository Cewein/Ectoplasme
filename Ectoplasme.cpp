#include <iostream>
#include <GLFW/glfw3.h>
#include <webgpu/webgpu.h>
#include "src/wgpuhelper.h"
#include <glfw3webgpu.h>
#include <cassert>
#include <vector>

using namespace ectoplasme;

int main()
{
	// safety check to see if glfw works
	if (!glfwInit())
	{
		std::cerr << "Could not initialize GLFW!" << std::endl;
		return 1;
	}

	//get the surface for glfw and webgpu 

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

	WGPUSurface surface = glfwGetWGPUSurface(instance, window);

	//getting the webGPU adapter
	std::cout << "Requesting adapter"  << std::endl;

	WGPURequestAdapterOptions adapterOptions = {};
	adapterOptions.nextInChain = nullptr;
	adapterOptions.compatibleSurface = surface;


	WGPUAdapter adapter = requestAdapter(instance, &adapterOptions);

	std::cout << "Got adapter: " << adapter << std::endl;
	std::cout << "Requesting Device" << std::endl;

	WGPUDeviceDescriptor deviceDesc = {};
	deviceDesc.nextInChain = nullptr;
	deviceDesc.label = "Main Graphical Device";
	deviceDesc.requiredFeaturesCount = 0;
	deviceDesc.requiredLimits = nullptr;
	deviceDesc.defaultQueue.nextInChain = nullptr;
	deviceDesc.defaultQueue.label = "Default Queue";

	WGPUDevice device = requestDevice(adapter, &deviceDesc);

	std::cout << "Got Device: " << device << std::endl;
	setDeviceErrorCallBack(&device);

	std::vector<WGPUFeatureName> features;

	size_t featuresCount = wgpuAdapterEnumerateFeatures(adapter, nullptr);

	features.resize(featuresCount);
	wgpuAdapterEnumerateFeatures(adapter,features.data());

	for(size_t i = 0; i < featuresCount; i++)
	{
		std::cout <<"\t[" << i << "] " << features[i] << std::endl;
	}

	// main loop like in opengl
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}

	// clean up
	// wGPU
	wgpuDeviceRelease(device);
	wgpuAdapterRelease(adapter);
	wgpuSurfaceRelease(surface);
	wgpuInstanceRelease(instance);

	// GLFW
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
#include <iostream>
#include <GLFW/glfw3.h>
#include <webgpu/webgpu.h>
#include "src/wgpucallback.h"
#include "src/wgpu.h"
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

	//we create a instance of the wGPU desc
	WGPUInstance instance = getWGPUInstance();

	//we get the surface from glfw and the webGPU instance
	WGPUSurface surface = glfwGetWGPUSurface(instance, window);

	//getting the webGPU adapter
	WGPUAdapter adapter = getAdapter(instance, surface);

	//getting the webGPU device
	WGPUDevice device = getDevice(adapter);

	//enumerate feature 
	enumerateFeature(adapter);

	//getting the queue
	//more info : https://eliemichel.github.io/LearnWebGPU/getting-started/the-command-queue.html
	WGPUQueue queue = wgpuDeviceGetQueue(device);
	auto onQueueWorkDone = [](WGPUQueueWorkDoneStatus status, void*)
	{
		//check enum form WGPUQueueWorkDoneStatus to see the enum value
		std::cout << "Queue work finished with status: " << status << std::endl;
	};

	wgpuQueueOnSubmittedWorkDone(queue, onQueueWorkDone, nullptr);

	//In webGPU we cannot create Command buffer, there for we need a command encoder
	WGPUCommandEncoderDescriptor encoderDesc = {};

	encoderDesc.nextInChain = nullptr;
	encoderDesc.label = "Main command encoder";

	WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(device, &encoderDesc);

	wgpuCommandEncoderInsertDebugMarker(encoder, "Test one");
	wgpuCommandEncoderInsertDebugMarker(encoder, "Test two");

	//we speciffy the option for the command buffer create by the command encoder
	WGPUCommandBufferDescriptor cmdBufferDescriptor = {};
	cmdBufferDescriptor.nextInChain = nullptr;
	cmdBufferDescriptor.label = "Command buffer";

	//now we encode the command
	WGPUCommandBuffer command = wgpuCommandEncoderFinish(encoder, &cmdBufferDescriptor);

	//Release the encode
	wgpuCommandEncoderRelease(encoder);

	//we now submit the command
	std::cout << "Submitting command" << std::endl;
	wgpuQueueSubmit(queue, 1, &command);
	wgpuCommandBufferRelease(command);

	// main loop like in opengl
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
	}

	// clean up
	// wGPU
	wgpuCommandBufferRelease(command);
	wgpuQueueRelease(queue);
	wgpuDeviceRelease(device);
	wgpuAdapterRelease(adapter);
	wgpuSurfaceRelease(surface);
	wgpuInstanceRelease(instance);

	// GLFW
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
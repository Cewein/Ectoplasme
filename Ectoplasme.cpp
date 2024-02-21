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
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
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
	WGPUInstanceDescriptor InstanceDesc = {};
	WGPUInstance instance = getWGPUInstance(InstanceDesc);

	//we get the surface from glfw and the webGPU instance
	WGPUSurface surface = glfwGetWGPUSurface(instance, window);

	//getting the webGPU adapter
	WGPUAdapter adapter = getAdapter(instance, surface);

	//getting the webGPU device
	WGPUDeviceDescriptor deviceDesc = {};
	WGPUDevice device = getDevice(adapter, deviceDesc);

	//enumerate feature 
	enumerateFeature(adapter);

	//getting the queue
	//more info : https://eliemichel.github.io/LearnWebGPU/getting-started/the-command-queue.html
	WGPUQueue queue = getQueueFromDevice(device);

	//creating the swapchain
	WGPUSwapChain swapChain = createSwapChain(device, adapter,surface, width, height);

	// main loop like in opengl
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		WGPUTextureView nextTexture = wgpuSwapChainGetCurrentTextureView(swapChain);
		std::cout << "Next texture: " << nextTexture << std::endl;

		if (!nextTexture)
		{
			std::cout << "Cannot acquire next swap chain texture" << std::endl;
			break;
		}

		//In webGPU we cannot create Command buffer, there for we need a command encoder
		WGPUCommandEncoderDescriptor encoderDesc = {};

		encoderDesc.nextInChain = nullptr;
		encoderDesc.label = "Main command encoder";

		WGPUCommandEncoder encoder = wgpuDeviceCreateCommandEncoder(device, &encoderDesc);

		//for rendering we need a RenderPassEncoder
		WGPURenderPassDescriptor renderPassDesc = {};
		WGPURenderPassColorAttachment renderPassColorAttachment = {};
		renderPassColorAttachment.view = nextTexture;
		renderPassColorAttachment.resolveTarget = nullptr;
		renderPassColorAttachment.loadOp = WGPULoadOp_Clear;
		renderPassColorAttachment.storeOp = WGPUStoreOp_Store;
		renderPassColorAttachment.clearValue = WGPUColor{ 0.6,0.7,0.9,1.0 };

		renderPassDesc.colorAttachmentCount = 1;
		renderPassDesc.colorAttachments = &renderPassColorAttachment;
		renderPassDesc.depthStencilAttachment = nullptr;
		renderPassDesc.timestampWriteCount = 0;
		renderPassDesc.timestampWrites = nullptr;
		renderPassDesc.nextInChain = nullptr;

		WGPURenderPassEncoder renderPass = wgpuCommandEncoderBeginRenderPass(encoder, &renderPassDesc);
		wgpuRenderPassEncoderEnd(renderPass);
		wgpuRenderPassEncoderRelease(renderPass);

		wgpuTextureViewRelease(nextTexture);

		WGPUCommandBufferDescriptor cmdBufferDescriptor = {};
		cmdBufferDescriptor.nextInChain = nullptr;
		cmdBufferDescriptor.label = "Command buffer";
		WGPUCommandBuffer command = wgpuCommandEncoderFinish(encoder, &cmdBufferDescriptor);
		wgpuCommandEncoderRelease(encoder);
		wgpuQueueSubmit(queue, 1, &command);
		wgpuCommandBufferRelease(command);

		wgpuSwapChainPresent(swapChain);
	}

	// clean up
	// wGPU
	wgpuSwapChainRelease(swapChain);
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
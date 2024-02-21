#pragma once
#include <webgpu/webgpu.h>

//todo rewrite each stuff in there own class and namespace
namespace ectoplasme
{
	//instance
	WGPUInstance getWGPUInstance(WGPUInstanceDescriptor &descriptor);

	//adapater
	WGPUAdapter getAdapter(WGPUInstance instance, WGPUSurface surface);

	//device
	WGPUDevice getDevice(WGPUAdapter adapter, WGPUDeviceDescriptor& deviceDesc);
	void enumerateFeature(WGPUAdapter adapter);
	
	//Queue
	WGPUQueue getQueueFromDevice(WGPUDevice device);

	//swapchain
	WGPUSwapChain createSwapChain(WGPUDevice device, WGPUAdapter adapter, WGPUSurface surface, int width, int height);
}


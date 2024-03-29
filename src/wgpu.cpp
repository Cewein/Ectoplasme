#include "wgpu.h"
#include "wgpucallback.h"
#include <iostream>
#include <webgpu/webgpu.h>
#include <cassert>
#include <vector>

//Function to get a WGPU instance based on the provided descriptor
WGPUInstance ectoplasme::getWGPUInstance(WGPUInstanceDescriptor &descriptor)
{
	std::cout << "Creating instance" << std::endl;
	//We create a instance of the wGPU desc
	WGPUInstance instance = wgpuCreateInstance(&descriptor);

	//Safety check to see if the creation of the wGPU instance is sucessfull
	if (!instance)
	{
		std::cout << "Could not instanciate wGPU" << std::endl;
		assert(!instance);
	}

	std::cout << "WGPU instance: " << instance << std::endl;
	return instance;
}


// Function to request and retrieve a WGPUAdapter based on the provided WGPUInstance and WGPUSurface
WGPUAdapter ectoplasme::getAdapter(WGPUInstance instance, WGPUSurface surface)
{
    std::cout << "Requesting adapter" << std::endl;

    // Configure adapter options, specifying the compatible surface
    WGPURequestAdapterOptions adapterOptions = {};
    adapterOptions.nextInChain = nullptr;
    adapterOptions.compatibleSurface = surface;

    // Request the WGPUAdapter using a callback define ine wgpucallback.h
    WGPUAdapter adapter = ectoplasme::requestAdapter(instance, &adapterOptions);

    std::cout << "Got adapter: " << adapter << std::endl;

    return adapter;
}

WGPUDevice ectoplasme::getDevice(WGPUAdapter adapter, WGPUDeviceDescriptor &deviceDesc)
{
	std::cout << "Requesting Device" << std::endl;

	// Configure device options
	if (deviceDesc.label == nullptr)
	{
		deviceDesc.nextInChain = nullptr;
		deviceDesc.label = "Main Graphical Device";
		deviceDesc.requiredFeaturesCount = 0;
		deviceDesc.requiredLimits = nullptr;
		deviceDesc.defaultQueue.nextInChain = nullptr;
		deviceDesc.defaultQueue.label = "Default Queue";
	}

	// Request the WGPUDevice using a callback define ine wgpucallback.h
	WGPUDevice device = ectoplasme::requestDevice(adapter, &deviceDesc);

	std::cout << "Got Device: " << device << std::endl;
	ectoplasme::setDeviceErrorCallBack(&device);

	return device;
}

void ectoplasme::enumerateFeature(WGPUAdapter adapter)
{
	std::vector<WGPUFeatureName> features;

	size_t featuresCount = wgpuAdapterEnumerateFeatures(adapter, nullptr);

	features.resize(featuresCount);
	wgpuAdapterEnumerateFeatures(adapter, features.data());

	for (size_t i = 0; i < featuresCount; i++)
	{
		std::cout << "\t[" << i << "] " << features[i] << std::endl;
	}
}

WGPUQueue ectoplasme::getQueueFromDevice(WGPUDevice device)
{
	WGPUQueue queue = wgpuDeviceGetQueue(device);

	auto onDeviceError = [](WGPUErrorType type, char const* message, void* /* pUserData */) {
		std::cout << "Uncaptured device error: type " << type;
		if (message) std::cout << " (" << message << ")";
		std::cout << std::endl;
	};

	wgpuDeviceSetUncapturedErrorCallback(device, onDeviceError, nullptr /* pUserData */);

	return queue;
}

WGPUSwapChain ectoplasme::createSwapChain(WGPUDevice device, WGPUAdapter adapter, WGPUSurface surface, int width, int height) {
	WGPUSwapChainDescriptor swapChainDesc = {};
	swapChainDesc.width = width;
	swapChainDesc.height = height;
	swapChainDesc.usage = WGPUTextureUsage_RenderAttachment;

#ifdef WEBGPU_BACKEND_WGPU
	WGPUTextureFormat swapChainFormat = wgpuSurfaceGetPreferredFormat(surface, adapter);
#else
	WGPUTextureFormat swapChainFormat = WGPUTextureFormat_BGRA8Unorm;
#endif

	swapChainDesc.format = swapChainFormat;

	swapChainDesc.presentMode = WGPUPresentMode_Fifo;

	WGPUSwapChain swapChain = wgpuDeviceCreateSwapChain(device, surface, &swapChainDesc);

	std::cout << "Swapchain: " << swapChain << std::endl;

	return swapChain;
}


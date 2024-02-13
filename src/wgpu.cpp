#include "wgpu.h"
#include "wgpucallback.h"
#include <iostream>
#include <webgpu/webgpu.h>
#include <cassert>

//Function to get a WGPU instance based on the provided descriptor
WGPUInstance ectoplasme::getWGPUInstance(WGPUInstanceDescriptor descriptor)
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

WGPUDevice ectoplasme::getDevice(WGPUAdapter adapter)
{
	std::cout << "Requesting Device" << std::endl;

	// Configure device options
	WGPUDeviceDescriptor deviceDesc = {};
	deviceDesc.nextInChain = nullptr;
	deviceDesc.label = "Main Graphical Device";
	deviceDesc.requiredFeaturesCount = 0;
	deviceDesc.requiredLimits = nullptr;
	deviceDesc.defaultQueue.nextInChain = nullptr;
	deviceDesc.defaultQueue.label = "Default Queue";

	// Request the WGPUDevice using a callback define ine wgpucallback.h
	WGPUDevice device = ectoplasme::requestDevice(adapter, &deviceDesc);

	std::cout << "Got Device: " << device << std::endl;
	ectoplasme::setDeviceErrorCallBack(&device);

	return device;
}


#pragma once
#include <webgpu/webgpu.h>


namespace ectoplasme
{
	WGPUInstance getWGPUInstance(WGPUInstanceDescriptor descriptor = {nullptr});
	WGPUAdapter getAdapter(WGPUInstance instance, WGPUSurface surface);
	WGPUDevice getDevice(WGPUAdapter adapter);
	void enumerateFeature(WGPUAdapter adapter);
}



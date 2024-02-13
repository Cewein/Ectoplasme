#pragma once
#include <webgpu/webgpu.h>

namespace ectoplasme
{
	WGPUAdapter requestAdapter(WGPUInstance instance, WGPURequestAdapterOptions const* options);
	WGPUDevice requestDevice(WGPUAdapter adapter, WGPUDeviceDescriptor const* descriptor);
	void setDeviceErrorCallBack(WGPUDevice* device);
}
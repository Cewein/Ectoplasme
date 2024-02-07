#pragma once
#include <webgpu/webgpu.h>

namespace ectoplasme
{
	WGPUAdapter requestAdapter(WGPUInstance instance, WGPURequestAdapterOptions const* options);
}
#pragma once
#include <webgpu/webgpu.h>
#include <iostream>
#include <cassert>

namespace ectoplasme
{
	WGPUAdapter requestAdapter(WGPUInstance instance, WGPURequestAdapterOptions const* options)
	{
		struct UserData
		{
			WGPUAdapter adapter = nullptr;
			bool requestEnded = false;
		};

		UserData userData;

		//the next function is a lambda function.
		//it can be replace a a normal C function outside of the scope of this function
		auto onAdapterRequestEnded = [](WGPURequestAdapterStatus status, WGPUAdapter adapter, char const* message, void* pUserData)
		{
			UserData& userData = *reinterpret_cast<UserData*>(pUserData);
			if (status == WGPURequestAdapterStatus_Success)
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
}
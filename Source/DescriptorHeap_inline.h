#pragma once
#include "DescriptorHeap.h"
#include "GraphicsEngine.h"

inline ID3D12DescriptorHeap* DescriptorHeap::GetDescriptorHeap(GraphicsEngine* graphicsEngine)const
{
	auto back_Buffer_Index= graphicsEngine->GetBackBufferIndex();
	return this->descriptor_Heap_[back_Buffer_Index].Get();
}

inline D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeap::GetConstantBufferGpuDescriptorStartHandle(GraphicsEngine* graphicsEngine) const
{
	auto back_Buffer_Index = graphicsEngine->GetBackBufferIndex();
	return this->const_Buffer_Gpu_Descriptor_Start_[back_Buffer_Index];
}

inline D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeap::GetShaderResourceGpuDescriptorStartHandle(GraphicsEngine* graphicsEngine) const
{
	auto back_Buffer_Index = graphicsEngine->GetBackBufferIndex();
	return this->shader_Resource_Gpu_Descriptor_Start_[back_Buffer_Index];
}

inline D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeap::GetUavResourceGpuDescriptorStartHandle(GraphicsEngine* graphicsEngine) const
{
	auto back_Buffer_Index = graphicsEngine->GetBackBufferIndex();
	return this->uav_Gpu_Descriptor_Start_[back_Buffer_Index];
}

inline D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeap::GetSamplerResourceGpuDescriptorStartHandle(GraphicsEngine* graphicsEngine) const
{
	auto back_Buffer_Index = graphicsEngine->GetBackBufferIndex();
	return this->sampler_Gpu_Descriptor_Start_[back_Buffer_Index];
}
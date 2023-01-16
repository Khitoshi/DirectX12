#pragma once
#include "DescriptorHeap.h"
#include "GraphicsEngine.h"

inline ID3D12DescriptorHeap* DescriptorHeap::GetDescriptorHeap(GraphicsEngine* graphicsEngine)const
{
	auto backBufferIndex = graphicsEngine->GetBackBufferIndex();
	return this->descriptor_Heap_[backBufferIndex].Get();
}

inline D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeap::GetConstantBufferGpuDescriptorStartHandle(GraphicsEngine* graphicsEngine) const
{
	auto backBufferIndex = g_graphicsEngine->GetBackBufferIndex();
	return m_cbGpuDescriptorStart[backBufferIndex];
}

inline D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeap::GetShaderResourceGpuDescriptorStartHandle(GraphicsEngine* graphicsEngine) const
{
	auto backBufferIndex = g_graphicsEngine->GetBackBufferIndex();
	return m_srGpuDescriptorStart[backBufferIndex];
}

inline D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeap::GetUavResourceGpuDescriptorStartHandle(GraphicsEngine* graphicsEngine) const
{
	auto backBufferIndex = g_graphicsEngine->GetBackBufferIndex();
	return m_uavGpuDescriptorStart[backBufferIndex];
}

inline D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeap::GetSamplerResourceGpuDescriptorStartHandle(GraphicsEngine* graphicsEngine) const
{
	auto backBufferIndex = g_graphicsEngine->GetBackBufferIndex();
	return m_samplerGpuDescriptorStart[backBufferIndex];
}
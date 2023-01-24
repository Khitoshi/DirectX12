#pragma once
#include "RenderContext.h"
#include "DescriptorHeap.h"
#include "GraphicsEngine.h"


/*
// �f�B�X�N���v�^�q�[�v �ݒ�
inline void RenderContext::SetComputeDescriptorHeap(GraphicsEngine* graphicsEngine, DescriptorHeap& descriptorHeap)
{
    this->descriptor_Heap_[0] = descriptorHeap.GetDescriptorHeap(graphicsEngine);
    this->command_List_->SetDescriptorHeaps(1, this->descriptor_Heap_->GetAddressOf());

    //�f�B�X�N���v�^�e�[�u���ɓo�^����B
    if (descriptorHeap.IsRegistConstantBuffer()) {
        SetComputeRootDescriptorTable(0, descriptorHeap.GetConstantBufferGpuDescriptorStartHandle(graphicsEngine));
    }
    if (descriptorHeap.IsRegistShaderResource()) {
        SetComputeRootDescriptorTable(1, descriptorHeap.GetShaderResourceGpuDescriptorStartHandle(graphicsEngine));
    }
    if (descriptorHeap.IsRegistUavResource()) {
        SetComputeRootDescriptorTable(2, descriptorHeap.GetUavResourceGpuDescriptorStartHandle(graphicsEngine));
    }
}
*/
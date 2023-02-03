#pragma once
#include "DescriptorHeap.h"
#include "GraphicsEngine.h"

inline ID3D12DescriptorHeap* DescriptorHeap::GetDescriptorHeap(GraphicsEngine*& graphicsEngine)const
{
	auto back_Buffer_Index= graphicsEngine->GetBackBufferIndex();
	return this->descriptor_Heap_[back_Buffer_Index];
}

inline D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeap::GetConstantBufferGpuDescriptorStartHandle(GraphicsEngine*& graphicsEngine) const
{
	auto back_Buffer_Index = graphicsEngine->GetBackBufferIndex();
	return this->const_Buffer_Gpu_Descriptor_Start_[back_Buffer_Index];
}

inline D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeap::GetShaderResourceGpuDescriptorStartHandle(GraphicsEngine*& graphicsEngine) const
{
	auto back_Buffer_Index = graphicsEngine->GetBackBufferIndex();
	return this->shader_Resource_Gpu_Descriptor_Start_[back_Buffer_Index];
}

inline D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeap::GetUavResourceGpuDescriptorStartHandle(GraphicsEngine*& graphicsEngine) const
{
	auto back_Buffer_Index = graphicsEngine->GetBackBufferIndex();
	return this->uav_Gpu_Descriptor_Start_[back_Buffer_Index];
}

inline D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHeap::GetSamplerResourceGpuDescriptorStartHandle(GraphicsEngine*& graphicsEngine) const
{
	auto back_Buffer_Index = graphicsEngine->GetBackBufferIndex();
	return this->sampler_Gpu_Descriptor_Start_[back_Buffer_Index];
}


//���\�[�X���f�B�X�N���v�^�q�[�v�ɓo�^
template<class T>
inline void DescriptorHeap::RegistResource(int registerIndex, T res, T resTable[], int& numRes, const int MAX_RESOURCE, const wchar_t* errorMessage)
{
    if (registerIndex == -1)
    {
        //-1���w�肳��Ă���̂ŁC
        //���ݓo�^����Ă��郊�\�[�X�̈�Ԍ��ɓo�^
        registerIndex = numRes;
    }

    //�o�^�ԍ������\�[�X�ő�l�ȉ��̏ꍇ
    if (registerIndex < MAX_RESOURCE)
    {
        //�o�^
        resTable[registerIndex] = res;

        if (numRes < registerIndex + 1)
        {
            //�o�^����Ă��郊�\�[�X���𑝂₷
            numRes = registerIndex + 1;
        }
    }
    else
    {
        //���\�[�X�̍ő�l��葽���ꍇ
        //�G���[���b�Z�[�W���o��
        MessageBox(nullptr, errorMessage, L"DescriptorHeap::RegistResource�Ŏ��s", MB_OK);
        std::abort();
        //�����I��
        std::abort();
    }
}

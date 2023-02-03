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


//リソースをディスクリプタヒープに登録
template<class T>
inline void DescriptorHeap::RegistResource(int registerIndex, T res, T resTable[], int& numRes, const int MAX_RESOURCE, const wchar_t* errorMessage)
{
    if (registerIndex == -1)
    {
        //-1が指定されているので，
        //現在登録されているリソースの一番後ろに登録
        registerIndex = numRes;
    }

    //登録番号がリソース最大値以下の場合
    if (registerIndex < MAX_RESOURCE)
    {
        //登録
        resTable[registerIndex] = res;

        if (numRes < registerIndex + 1)
        {
            //登録されているリソース数を増やす
            numRes = registerIndex + 1;
        }
    }
    else
    {
        //リソースの最大値より多い場合
        //エラーメッセージを出す
        MessageBox(nullptr, errorMessage, L"DescriptorHeap::RegistResourceで失敗", MB_OK);
        std::abort();
        //強制終了
        std::abort();
    }
}

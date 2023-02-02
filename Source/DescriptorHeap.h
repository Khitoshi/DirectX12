#pragma once

#include <d3dx12.h>
#include<wrl.h>

using namespace Microsoft::WRL;

class IShaderResource;
class IUnorderAccessResrouce;
class ConstantBuffer;
class GraphicsEngine;

class DescriptorHeap
{
public:
    /// <summary>
    /// �f�t�H���g �R���X�g���N�^
    /// </summary>
    DescriptorHeap();

    /// <summary>
    /// �f�t�H���g �f�X�g���N�^
    /// </summary>
    ~DescriptorHeap();

    /// <summary>
    /// �V�F�[�_�[���\�[�X���f�B�X�N���v�^�q�[�v�ɓo�^
    /// </summary>
    /// <param name="registerNo">���W�X�^�ԍ� -1���w�߂��ꂽ��C���ݓo�^����Ă��郊�\�[�X�̎��Ƀ��W�X�^��o�^����</param>
    /// <param name="shaderResource">�V�F�[�_�[���\�[�X</param>
    void RegistShaderResource(int registerNo, IShaderResource& shaderResource);

    /// <summary>
    /// �A���I�[�_�[�A�N�Z�X���\�[�X��o�^
    /// </summary>
    /// <param name="registerNo">���W�X�^�ԍ��C-1���o�^���ꂽ�猻�ݓo�^����Ă��郊�\�[�X�̈�Ԍ��ɓo�^</param>
    /// <param name="sr"></param>
    void RegistUnorderAccessResource(int registerIndex, IUnorderAccessResrouce& unorderAccessResrouce);
    
    /// <summary>
    /// �萔�o�b�t�@���f�B�X�N���v�^�q�[�v�ɓo�^
    /// </summary>
    /// <param name="registerNo">���W�X�^�ԍ� -1���w�߂��ꂽ��C���ݓo�^����Ă��郊�\�[�X�̎��Ƀ��W�X�^��o�^����</param>
    /// <param name="cb">�萔�o�b�t�@</param>
    void RegistConstantBuffer(int registerIndex, ConstantBuffer& constantBuffer);

    /// <summary>
    /// �T���v����`���f�B�X�N���v�^�q�[�v�ɒǉ�
    /// </summary>
    /// <param name="registerNo">���W�X�^�ԍ� -1���w�߂��ꂽ��C���ݓo�^����Ă��郊�\�[�X�̎��Ƀ��W�X�^��o�^����</param>
    /// <param name="desc">�T���v����`</param>
    void RegistSamplerDesc(int registerIndex, const D3D12_SAMPLER_DESC& samplerDesc);

    /// <summary>
    /// �f�B�X�N���v�^�q�[�v�ւ̓o�^���m��
    /// </summary>
    /// <param name="graphicsEngine">�f�o�C�X���i�[����Ă���</param>
    void Commit(GraphicsEngine*& graphicsEngine);

    /// <summary>
    /// �T���v���X�e�[�g�p�̃f�B�X�N���v�^�q�[�v�ւ̓o�^
    /// </summary>
    /// <param name="graphicsEngine">�f�o�C�X���i�[����Ă���</param>
    void CommitSamplerHeap(GraphicsEngine*& graphicsEngine);

private:
    /// <summary>
    /// ���\�[�X���f�B�X�N���v�^�q�[�v�ɓo�^
    /// </summary>
    /// <typeparam name="T"></typeparam>
    /// <param name="registerIndex">�o�^�ԍ�</param>
    /// <param name="res">�o�^���郊�\�[�X</param>
    /// <param name="resTable">���\�[�X�e�[�u���B���̃e�[�u���Ƀ��\�[�X���ǉ�����܂��B</param>
    /// <param name="numRes">�o�^����Ă��郊�\�[�X�̐��B�{�֐����Ăяo���ƁA���̐����P�C���N�������g����܂��B</param>
    /// <param name="MAX_RESOURCE">���\�[�X�̍ő�l</param>
    /// <param name="errorMessage">�G���[�����������ۂɕ\������TEXT</param>
    template<class T>
    void RegistResource(int registerIndex, T res, T resTable[], int& numRes, const int MAX_RESOURCE, const wchar_t* errorMessage);

public://get method

    /// <summary>
    /// �f�B�X�N���v�^�q�[�v �擾
    /// </summary>
    /// <param name="graphicsEngine">�f�o�C�X���i�[���Ă���</param>
    /// <returns>this Descriptor Heap</returns>
    ID3D12DescriptorHeap* GetDescriptorHeap(GraphicsEngine*& graphicsEngine)const;

    /// <summary>
    /// �萔�o�b�t�@�̊J�n�n���h�� �擾
    /// </summary>
    /// <param name="graphicsEngine">�f�o�C�X���i�[���Ă���</param>
    /// <returns>this constant Buffer GPU descriptor start handle</returns>
    D3D12_GPU_DESCRIPTOR_HANDLE GetConstantBufferGpuDescriptorStartHandle(GraphicsEngine*& graphicsEngine)const;

    /// <summary>
    /// �V�F�[�_�[���\�[�X�̃f�B�X�N���v�^�̊J�n�n���h�����擾
    /// </summary>
    /// <param name="graphicsEngine">�f�o�C�X���i�[���Ă���</param>
    /// <returns>this shader resource GPU descriptor start handle</returns>
    D3D12_GPU_DESCRIPTOR_HANDLE GetShaderResourceGpuDescriptorStartHandle(GraphicsEngine*& graphicsEngine)const;

    /// <summary>
    /// Unorder Access ���\�[�X�̃f�B�X�N���v�^�̊J�n�n���h�����擾
    /// </summary>
    /// <param name="graphicsEngine">�f�o�C�X���i�[���Ă���</param>
    /// <returns>this uav resource GPU descriptor start handle</returns>
    D3D12_GPU_DESCRIPTOR_HANDLE GetUavResourceGpuDescriptorStartHandle(GraphicsEngine*& graphicsEngine) const;

    /// <summary>
    /// Sampler�̃f�B�X�N���v�^�̊J�n�n���h�����擾
    /// </summary>
    /// <param name="graphicsEngine">�f�o�C�X���i�[���Ă���</param>
    /// <returns>this sampler resource GPU descriptor start handle</returns>
    D3D12_GPU_DESCRIPTOR_HANDLE GetSamplerResourceGpuDescriptorStartHandle(GraphicsEngine*& graphicsEngine) const;

    /// <summary>
    /// �V�F�[�_�[���\�[�X����ł��o�^����Ă��邩����
    /// </summary>
    /// <returns>true = not zero</returns>
    bool IsRegistShaderResource() const { return this->num_Shader_Resource_ != 0; }

    /// <summary>
    /// �萔�o�b�t�@����ł��o�^����Ă��邩����
    /// </summary>
    /// <returns>true = not zero</returns>
    bool IsRegistConstantResource() const { return this->num_Constant_Buffer_ != 0; }

    /// <summary>
    /// UAV���\�[�X����ł��o�^����Ă��邩����
    /// </summary>
    /// <returns>true = not zero</returns>
    bool IsRegistUavResource() const { return this->num_Uav_Resource_ != 0; }

    /// <summary>
    /// �萔�o�b�t�@����ł��o�^����Ă��邩����B
    /// </summary>
    /// <returns>tru = not zero</returns>
    bool IsRegistConstantBuffer() const { return this->num_Constant_Buffer_ != 0; }

    /// <summary>
    /// UAV�f�B�X�N���v�^���n�܂�z��ԍ����擾����B
    /// </summary>
    /// <remarks>
    /// UAV�f�B�X�N���v�^��SRV�f�B�X�N���v�^�̎�����o�^����Ă���B
    /// �Ȃ̂ŁASRV�f�B�X�N���v�^��10�o�^����Ă���΁A
    /// UAV�f�B�X�N���v�^�͔z���10�Ԗڂ���o�^����Ă��邱�ƂɂȂ�B
    /// ���̊֐��͌��݃��C�g���G���W���Ŏg�p����Ă���B
    /// </remarks>
    /// <returns></returns>
    int GetOffsetUAVDescriptorFromTableStart() const { return this->num_Shader_Resource_ + this->num_Constant_Buffer_; }

    /// <summary>
    /// SRV�f�B�X�N���v�^���n�܂�z��ԍ����擾����B
    /// </summary>
    /// <returns></returns>
    int GetOffsetSRVDescriptorFromTableStart() const { return this->num_Constant_Buffer_; }

    /// <summary>
    /// �萔�o�b�t�@�f�B�X�N���v�^���n�܂�z��ԍ����擾����B
    /// </summary>
    /// /// <remarks>
    /// �萔�o�b�t�@�f�B�X�N���v�^��SRV�f�B�X�N���v�^��UAV�f�B�X�N���v�^�̎�����o�^����Ă���B
    /// �Ȃ̂ŁASRV�f�B�X�N���v�^��10�AUVA�f�B�X�N���v�^��5�o�^����Ă���΁A
    /// �萔�o�b�t�@�f�B�X�N���v�^�͔z���15�Ԗڂ���o�^����Ă��邱�ƂɂȂ�B
    /// ���̊֐��͌��݃��C�g���G���W���Ŏg�p����Ă���B
    /// <returns></returns>
    int GetOffsetConstantBufferDescriptorFromTableStart() const{return this->num_Shader_Resource_ + this->num_Uav_Resource_;}

private:
    enum {
        MAX_SHADER_RESOURCE = 1024 * 10,	//�V�F�[�_�[���\�[�X�̍ő吔�B
        MAX_CONSTANT_BUFFER = 1024 * 10,	//�萔�o�b�t�@�̍ő吔�B
        MAX_SAMPLER_STATE = 16,	            //�T���v���X�e�[�g�̍ő吔�B
    };

private:
    //�f�B�X�N���v�^�q�[�v
    //ComPtr<ID3D12DescriptorHeap> descriptor_Heap_[2];
    ID3D12DescriptorHeap* descriptor_Heap_[2];
    //�V�F�[�_�[���\�[�X�B
    std::vector<IShaderResource*> shader_Resources_;
    //UAV���\�[�X�B
    std::vector <IUnorderAccessResrouce*> uav_Resources_;
    //�萔�o�b�t�@�B
    std::vector <ConstantBuffer*> constant_Buffers_;
    //�T���v���X�e�[�g�B
    D3D12_SAMPLER_DESC sampler_Descs_[MAX_SAMPLER_STATE];
    //�萔�o�b�t�@�̃f�B�X�N���v�^�q�[�v�̊J�n�n���h���B
    D3D12_GPU_DESCRIPTOR_HANDLE const_Buffer_Gpu_Descriptor_Start_[2];
    //�V�F�[�_�[���\�[�X�̃f�B�X�N���v�^�q�[�v�̊J�n�n���h���B
    D3D12_GPU_DESCRIPTOR_HANDLE shader_Resource_Gpu_Descriptor_Start_[2];
    //UAV���\�[�X�̃f�B�X�N���v�^�q�[�v�̊J�n�n���h���B
    D3D12_GPU_DESCRIPTOR_HANDLE uav_Gpu_Descriptor_Start_[2];
    //�T���v���[�X�e�[�g�̃f�B�X�N���v�^�̊J�n�n���h��
    D3D12_GPU_DESCRIPTOR_HANDLE sampler_Gpu_Descriptor_Start_[2];

    //�V�F�[�_�[���\�[�X�̐�
    int num_Shader_Resource_;
    //�萔�o�b�t�@�̐�
    int num_Constant_Buffer_;
    //�A���I�[�_�[�A�N�Z�X���\�[�X�̐�
    int num_Uav_Resource_;
    //�T���v���̐�
    int num_Sampler_Desc_;
    int num_Descriptor_Heap_;
};


#pragma once

#include "IShaderResource.h"
#include <wrl.h>
#include <d3dx12.h>

using namespace Microsoft::WRL;

class GraphicsEngine;

class StructuredBuffer :public IShaderResource
{
public:
    /// <summary>
    /// �f�t�H���g �R���X�g���N�^ 
    /// </summary>
    StructuredBuffer();

    /// <summary>
    /// �f�t�H���g �f�X�g���N�^ 
    /// </summary>
    ~StructuredBuffer()override;

    /// <summary>
    /// ������
    /// </summary>
    /// <param name="graphicsEngine">�f�o�C�X���i�[���Ă���</param>
    /// <param name="elementSize">�G�������g�̃T�C�Y</param>
    /// <param name="elementNumber">�G�������g�̐�</param>
    /// <param name="initData">�������f�[�^</param>
    void Init(GraphicsEngine*& graphicsEngine, int elementSize, int elementNumber, void* initData);

    /// <summary>
    /// SRV�ɓo�^�B
    /// </summary>
    /// <param name="graphicsEngine">�f�o�C�X���i�[���Ă���</param>
    /// <param name="descriptorHandle">CPU �f�B�X�N���v�^�@�n���h��</param>
    /// <param name="buffernumber">�o�b�t�@ �ԍ�</param>
    void RegistShaderResourceView(GraphicsEngine*& graphicsEngine, D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandle, int bufferNumber)override;

    /// <summary>
    /// �\�����o�b�t�@�̓��e�X�V
    /// </summary>
    /// <param name="graphicsEngine">�f�o�C�X���i�[���Ă���</param>
    /// <param name="data"></param>
    void Update(GraphicsEngine*& graphicsEngine,void* data);

    
private:

public:
#pragma region Get method

    /// <summary>
    /// ���������ʗp�t���O�擾
    /// </summary>
    /// <returns>true = �������ς�</returns>
    bool IsInited() const { return this->is_Inited_; }

    //GPU����A�N�Z�X�ł���o�b�t�@�[�̎擾
    ID3D12Resource* GetResouce(GraphicsEngine*& graphicsEngine);

#pragma endregion


private:
    //GPU����A�N�Z�X�ł���o�b�t�@�[
    ComPtr<ID3D12Resource> buffers_On_GPU_[2];
    //CPU����A�N�Z�X�ł���o�b�t�@�[
    void* buffers_On_CPU_[2];

    //�G�������g �v�f��
    int element_Number_;
    //�G�������g�T�C�Y
    int element_Size_;

    //�������ς� ���f �t���O
    bool is_Inited_;

};


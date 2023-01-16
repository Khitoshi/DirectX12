#pragma once
#include<d3dx12.h>
#include<wrl.h>

#include "GraphicsEngine.h"

using namespace Microsoft::WRL;

/// <summary>
/// �C���f�b�N�X �o�b�t�@
/// </summary>
class IndexBuffer
{
public:
    /// <summary>
    /// �f�t�H���g �R���X�g���N�^
    /// </summary>
    IndexBuffer();

    /// <summary>
    /// �f�t�H���g �f�X�g���N�^
    /// </summary>
    ~IndexBuffer();

    /// <summary>
    /// ������
    /// </summary>
    /// <param name="graphicsEngine">�g�p���Ă���f�o�C�X���i�[���Ă���N���X</param>
    /// <param name="size">�C���f�b�N�X�o�b�t�@�̃T�C�Y</param>
    /// <param name="stride">�X�g���C�h</param>
    void Init(GraphicsEngine* graphicsEngine,int size, int stride);

    /// <summary>
    /// �C���f�b�N�X�f�[�^���C���f�b�N�X�o�b�t�@�ɃR�s�[
    /// </summary>
    /// <param name="srcIndecies">�R�s�[���̃C���f�b�N�X�f�[�^</param>
    void Copy(uint16_t* srcIndecies);


    /// <summary>
    /// �C���f�b�N�X�f�[�^���C���f�b�N�X�o�b�t�@�ɃR�s�[
    /// </summary>
    /// <param name="srcIndecies">�R�s�[���̃C���f�b�N�X�f�[�^</param>
    void Copy(uint32_t* srcIndecies);


public:
    /// <summary>
    /// �C���f�b�N�X�o�b�t�@�r���[���擾
    /// </summary>
    /// <returns>this index buffer view</returns>
    const D3D12_INDEX_BUFFER_VIEW& GetIndexBufferView()const { return this->index_Buffer_View_; }

    /// <summary>
    /// �C���f�b�N�X�̐����擾
    /// </summary>
    /// <returns></returns>
    const int GetIndexCount()const { return this->index_Count_; }

    /// <summary>
    /// �C���f�b�N�X�o�b�t�@�̃X�g���C�h���擾
    /// </summary>
    /// <returns>this indexbuffer stride in bytes size</returns>
    const UINT GetStrideInBytes() const { return static_cast<UINT>(this->stride_In_Bytes_); }

    /// <summary>
    /// �C���f�b�N�X�o�b�t�@�̃T�C�Y(�P��:byte)���擾
    /// </summary>
    /// <returns>this indexbuffer size in bytes</returns>
    const UINT GetSizeInBytes()const { return static_cast<UINT>(this->size_In_Bytes_); }

    /// <summary>
    /// ID3D12Resouce�̃A�h���X����t��
    /// </summary>
    /// <returns>return this Resouce</returns>
    ID3D12Resource* GetResouceAddress()const { return this->index_Buffer_.Get(); }

private:
    //�C���f�b�N�X�o�b�t�@
    ComPtr<ID3D12Resource> index_Buffer_;
    //�C���f�b�N�X�o�b�t�@�r���[
    D3D12_INDEX_BUFFER_VIEW index_Buffer_View_;

    //�C���f�b�N�X�̐�
    int index_Count_;
    //�X�g���C�h
    int stride_In_Bytes_;
    //�T�C�Y
    int size_In_Bytes_;
};
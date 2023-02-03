#pragma once
#include <utility>
#include "IShaderResource.h"

#include "Skeleton.h"
#include "MeshParts.h"

//���f���̏����
enum EnModelUpAxis {
    enModelUpAxisY,		//���f���̏������Y���B
    enModelUpAxisZ,		//���f���̏������Z���B
};


struct ModelInitData
{
    //�t�@�C���p�X
    const char* model_File_Path_;

    //.fx�t�@�C���̃t�@�C���p�X�B
    const char* shader_File_Path = nullptr;

    //���_�V�F�[�_�[�̃G���g���[�|�C���g�B
    const char* vs_Entry_Point_Func_ = "VSMain";

    //�X�L������}�e���A���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g�B
    const char* vs_Skin_Entry_Point_Func_ = "VSMain";

    //�s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g�B
    const char* ps_entry_point_func_ = "PSMain";


    //���[�U�[�g���̒萔�o�b�t�@�B
    void* expand_Constant_Buffer = nullptr;
    //���[�U�[�g���̒萔�o�b�t�@�̃T�C�Y�B
    int expand_Constant_Buffer_Size = 0;


    //���[�U�[�g���̃V�F�[�_�[���\�[�X�B
    std::array<IShaderResource*, 6> expand_Shader_Resoruce_View_ = { nullptr };
    //std::array<IShaderResource*, MeshParts::GetMAXMODELEXPANDSRV()> expand_Shader_Resoruce_View_ = { nullptr };

    //�X�P���g���B
    Skeleton* skeleton_ = nullptr;

    //���f���̏�����B
    EnModelUpAxis en_model_Up_Axis_ = EnModelUpAxis::enModelUpAxisZ;

    //�e�N�X�`���T���v�����O�̃t�B���^�B
    D3D12_FILTER sampler_Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;

    //�����_�����O����J���[�o�b�t�@�̃t�H�[�}�b�g�B
    std::array<DXGI_FORMAT, static_cast<int>(D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT)> color_Buffer_Format_ = {
        DXGI_FORMAT_R8G8B8A8_UNORM,
        DXGI_FORMAT_UNKNOWN,
        DXGI_FORMAT_UNKNOWN,
        DXGI_FORMAT_UNKNOWN,
        DXGI_FORMAT_UNKNOWN,
        DXGI_FORMAT_UNKNOWN,
        DXGI_FORMAT_UNKNOWN,
        DXGI_FORMAT_UNKNOWN,
    };
}; 
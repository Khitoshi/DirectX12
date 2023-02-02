#pragma once

#include <array>
#include <vector>
#include <memory>
#include <functional>

#include "TkmFile.h"
#include "StructuredBuffer.h"
#include "ConstantBuffer.h"
#include "IShaderResource.h"
#include "Matrix.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "DescriptorHeap.h"
#include "MeshParts.h"
#include "tkEngine.h"
class Material;
class Skeleton;

struct SMesh
{
    //���_�o�b�t�@
    VertexBuffer vertexBuffer;
    //�C���f�b�N�X�o�b�t�@
    std::vector<IndexBuffer*> indexBufferArray;
    //�}�e���A��
    std::vector<Material*> materials;
    //�X�L���������Ă��邩�ǂ����̃t���O
    std::vector<int> skinFlags;
};

class MeshParts
{
private:
    //�g��SRV���ݒ肳��郌�W�X�^�̊J�n�ԍ�
    enum { EXPAND_SRV_REG__START_NO = 10 };
    //�g��SRV�̍ő吔
    enum { MAX_MODEL_EXPAND_SRV = 6 };
    //�P�̃}�e���A���Ŏg�p�����SRV�̐�
    enum { NUM_SRV_ONE_MATERIAL = EXPAND_SRV_REG__START_NO + MAX_MODEL_EXPAND_SRV };
    //�P�̃}�e���A���Ŏg�p�����CBV�̐�
    enum { NUM_CBV_ONE_MATERIAL = 6 };
public:

    /// <summary>
    /// �f�t�H���g �R���X�g���N�^
    /// </summary>
    MeshParts();

    /// <summary>
    /// �f�t�H���g �f�X�g���N�^
    /// </summary>
    ~MeshParts();

    /// <summary>
    /// .tkm file ������
    /// </summary>
    /// <param name="graphicsEngine">�f�o�C�X �擾</param>
    /// <param name="tkmFile">tkm file</param>
    /// <param name="fxFilePath">.fx file path</param>
    /// <param name="vsEntryPointFuncName">���_�V�F�[�_�[�̃G���g���[�|�C���g��</param>
    /// <param name="vsSkinEntryPointFuncName">�X�L������}�e���A���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g��</param>
    /// <param name="psEntryPointFuncName">�s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g��</param>
    /// <param name="expandData"></param>
    /// <param name="expandDataSize"></param>
    /// <param name="expandShaderResourceView"></param>
    /// <param name="colorBufferFormat">���̃��f���������_�����O����J���[�o�b�t�@�̃t�H�[�}�b�g</param>
    /// <param name="samplerFilter">�T���v���t�B���^</param>
    void InitFromTkmFile(
        tkEngine*& tk,
        GraphicsEngine*& graphicsEngine,
        const TkmFile& tkmFile,
        const char* fxFilePath,
        const char* vsEntryPointFuncName,
        const char* vsSkinEntryPointFuncName,
        const char* psEntryPointFuncName,
        void* expandData,
        int expandDataSize,
        const std::array<IShaderResource*, MAX_MODEL_EXPAND_SRV>& expandShaderResourceView,
        const std::array<DXGI_FORMAT, static_cast<int>(D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT)>& colorBufferFormat,
        D3D12_FILTER samplerFilter
    );

    /// <summary>
    /// �`��
    /// </summary>
    /// <param name="renderContext">�����_�����O�R���e�L�X�g</param>
    /// <param name="matrixWorld">���[���h �s��</param>
    /// <param name="matrixView">�r���[ �s��</param>
    /// <param name="matrixProjection">�v���W�F�N�V���� �s��</param>
    void Draw(
        GraphicsEngine*& graphicsEngine,
        RenderContext& renderContext,
        const Matrix& matrixWorld,
        const Matrix& matrixView,
        const Matrix& matrixProjection
    );

    /// <summary>
    /// �C���X�^���V���O�`��
    /// </summary>
    /// <param name="renderContext">�����_�����O�R���e�L�X�g</param>
    /// <param name="numInstance">�C���X�^���X��</param>
    /// <param name="mView">�r���[�s��</param>
    /// <param name="mProj">�v���W�F�N�V�����s��</param>
    void DrawInstancing(GraphicsEngine*& graphicsEngine,RenderContext& renderContext, int numInstance, const Matrix& matrixView, const Matrix& matrixProjection);

    /// <summary>
    /// �X�P���g�����֘A�t����
    /// </summary>
    /// <param name="skeleton">�X�P���g��</param>
    void BindSkeleton(GraphicsEngine*& graphicsEngine,Skeleton& skeleton);

    /// <summary>
    /// ���b�V���ɑ΂��Ė₢���킹���s���B
    /// </summary>
    /// <param name="queryFunc">�N�G���֐�</param>
    void QueryMeshs(std::function<void(const SMesh& mesh)> queryFunc)
    {
        for (const auto& mesh : this->meshs_) {
            queryFunc(*mesh);
        }
    }
    void QueryMeshAndDescriptorHeap(std::function<void(const SMesh& mesh, const DescriptorHeap& ds)> queryFunc)
    {
        for (int i = 0; i < this->meshs_.size(); i++) {
            queryFunc(*this->meshs_[i], this->descriptor_Heap_);
        }
    }


    /// <summary>
    /// �f�B�X�N���v�^�q�[�v���쐬�B
    /// </summary>
    /// <param name = "graphicsEngine">�f�o�C�X���i�[���Ă���</param>
    void CreateDescriptorHeaps(GraphicsEngine*& graphicsEngine);

private:
    /// <summary>
    /// tkm���b�V�����烁�b�V�����쐬�B
    /// </summary>
    /// <param name="mesh">���b�V��</param>
    /// <param name="meshNo">���b�V���ԍ�</param>
    /// <param name="fxFilePath">fx�t�@�C���̃t�@�C���p�X</param>
    /// <param name="vsEntryPointFuncName">���_�V�F�[�_�[�̃G���g���[�|�C���g�̖��O</param>
    /// <param name="vsSkinEntryPointFuncName">�X�L������}�e���A���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g�̖��O</param>
    /// <param name="psEntryPointFuncName">�s�N�Z���V�F�[�_�[�̃G���g���[�|�C���g�̖��O</param>
    /// <param name="colorBufferFormat">���̃��f���������_�����O����J���[�o�b�t�@�̃t�H�[�}�b�g</param>
    /// <param name="samplerFilter">�T���v���t�B���^</param>
    void CreateMeshFromTkmMesh(
        tkEngine*& tk,
        GraphicsEngine*& graphicsEngine,
        const TkmFile::SMesh& mesh,
        int meshNo,
        int& materialNum,
        const char* fxFilePath,
        const char* vsEntryPointFuncName,
        const char* vsSkinEntryPointFuncName,
        const char* psEntryPointFuncName,
        const std::array<DXGI_FORMAT, static_cast<int>D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT>& colorBufferFormat,
        D3D12_FILTER samplerFilter
    );

    /// <summary>
    /// �`�揈���̋��ʏ���
    /// </summary>
    /// <param name="rc">�����_�����O�R���e�L�X�g</param>
    /// <param name="mWorld">���[���h�s��</param>
    /// <param name="mView">�r���[�s��</param>
    /// <param name="mProj">�v���W�F�N�V�����s��</param>
    void DrawCommon(GraphicsEngine*& graphicsEngine,RenderContext& renderContext, const Matrix& matrixWorld, const Matrix& matrixView, const Matrix& matrixProjection);

public:
#pragma region Get method
    /// <summary>
    /// �g��SRV���ݒ肳��郌�W�X�^�̊J�n�ԍ� �擾
    /// </summary>
    /// <returns>static int</returns>
    const static int GetEXPANDSRVREGSTARTNO() { return EXPAND_SRV_REG__START_NO; }

    /// <summary>
    /// �g��SRV�̍ő吔 �擾
    /// </summary>
    /// <returns>static int</returns>
    const static int GetMAXMODELEXPANDSRV() { return MAX_MODEL_EXPAND_SRV; }

    /// <summary>
    /// �P�̃}�e���A���Ŏg�p�����SRV�̐� �擾
    /// </summary>
    /// <returns>static int</returns>
    const static int GetNUMSRVONEMATERIAL() { return NUM_SRV_ONE_MATERIAL; }

    /// <summary>
    /// �P�̃}�e���A���Ŏg�p�����CBV�̐� �擾
    /// </summary>
    /// <returns>static int</returns>
    const static int GetNUMCBVONEMATERIAL() { return NUM_CBV_ONE_MATERIAL; }
#pragma endregion

private:
    /// <summary>
    /// �萔�o�b�t�@�B
    /// </summary>
    /// <remarks>
    /// ���̍\���̂�ύX������ASimpleModel.fx��CB���ύX����悤�ɁB
    /// </remarks>
    struct SConstantBuffer {
        Matrix world;		//���[���h�s��B
        Matrix view;		//�r���[�s��B
        Matrix proj;		//�v���W�F�N�V�����s��B
    };

    //���b�V�����ʂ̒萔�o�b�t�@
    ConstantBuffer common_Constant_Buffer_;
    //���[�U�[�g���p�̒萔�o�b�t�@
    ConstantBuffer expand_Constant_Buffer_;

    std::array<IShaderResource*, static_cast<int>(MAX_MODEL_EXPAND_SRV)>expand_Shader_Resource_View_;
    
    //�{�[���̍s��̍\�����o�b�t�@
    StructuredBuffer bone_Matrices_Structure_Buffer_;

    //���b�V��
    std::vector<std::unique_ptr<SMesh>> meshs_;

    //�f�B�X�N���v�^�q�[�v
    DescriptorHeap descriptor_Heap_;

    //�X�P���g��
    Skeleton* skelton_;

    void* expand_Data_;

};


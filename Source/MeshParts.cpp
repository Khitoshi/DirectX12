#include "MeshParts.h"
#include "TkmFile.h"
#include "RenderContext.h"
#include "Material.h"

//�R���X�g���N�^
MeshParts::MeshParts():
    common_Constant_Buffer_(),
    expand_Constant_Buffer_(),
    expand_Shader_Resource_View_(),
    bone_Matrices_Structure_Buffer_(),
    meshs_(),
    descriptor_Heap_(),
    skelton_(),
    expand_Data_()
{
}

//�f�X�g���N�^
MeshParts::~MeshParts()
{
}

//.tkm file ������
void MeshParts::InitFromTkmFile(
    GraphicsEngine* graphicsEngine,
    const TkmFile& tkmFile, 
    const char* fxFilePath, 
    const char* vsEntryPointFuncName, 
    const char* vsSkinEntryPointFuncName, 
    const char* psEntryPointFuncName, 
    void* expandData, 
    int expandDataSize, 
    const std::array<IShaderResource*, MAX_MODEL_EXPAND_SRV>& expandShaderResourceView, 
    const std::array<DXGI_FORMAT, static_cast<int>(D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT)>& colorBufferFormat, 
    D3D12_FILTER samplerFilter)
{
    //�T�C�Y �ݒ�
    this->meshs_.resize(tkmFile.GetNumMesh());

    int mesh_no = 0;
    int materian_no = 0;
    tkmFile.QueryMeshParts([&](const TkmFile::SMesh& mesh) {
        //tkm�t�@�C���̃��b�V����񂩂烁�b�V�� �쐬
        CreateMeshFromTkmMesh(
            mesh,
            mesh_no,
            materian_no,
            fxFilePath,
            vsEntryPointFuncName,
            vsSkinEntryPointFuncName,
            psEntryPointFuncName,
            colorBufferFormat,
            samplerFilter
        );
        mesh_no++;
        });
    //���ʒ萔�o�b�t�@�̍쐬�B
    this->common_Constant_Buffer_.Init(graphicsEngine,sizeof(SConstantBuffer), nullptr);
    //���[�U�[�g���p�̒萔�o�b�t�@���쐬�B
    if (expandData) {
        this->expand_Constant_Buffer_.Init(graphicsEngine,expandDataSize, nullptr);
        this->expand_Data_ = expandData;
    }
    for (int i = 0; i < MAX_MODEL_EXPAND_SRV; i++) {
        this->expand_Shader_Resource_View_[i] = expandShaderResourceView[i];
    }
    //�f�B�X�N���v�^�q�[�v���쐬�B
    CreateDescriptorHeaps(graphicsEngine);
}

//�`��
void MeshParts::Draw(RenderContext& renderContext, const Matrix& matrixWorld, const Matrix& matrixView, const Matrix& matrixProjection)
{
    //�`�拤�ʏ������s��
    DrawCommon(renderContext, matrixWorld, matrixView, matrixProjection);

    int descriptor_heap_number = 0;
    for (auto& mesh : this->meshs_)
    {
        //���_�o�b�t�@��ݒ�
        renderContext.SetVertexBuffer(mesh->vertexBuffer);
        //�}�e���A�����ƂɃh���[
        for (int material_number = 0; material_number < mesh->materials.size(); material_number++)
        {
            //���̃}�e���A���������Ă��郁�b�V���̕`��J�n
            mesh->materials[material_number]->BeginRender(renderContext, mesh->skinFlags[material_number]);

            renderContext.SetDescriptorHeap(this->descriptor_Heap_);
        }
    }

}

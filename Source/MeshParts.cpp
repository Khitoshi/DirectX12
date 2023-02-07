#include "MeshParts.h"
#include "TkmFile.h"
//#include "RenderContext.h"
#include "RenderContext_inline.h"
#include "Material.h"
#include "Skeleton.h"
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
    if (skelton_)delete skelton_;

    for (auto& mesh : this->meshs_) {
        //�C���f�b�N�X�o�b�t�@���폜�B
        for (auto& ib : mesh->indexBufferArray) {
            if (ib != nullptr)delete ib;
        }
        //�}�e���A�����폜�B
        for (auto& mat : mesh->materials) {
            if(mat != nullptr)delete mat;
        }
        //���b�V�����폜�B
        if(mesh != nullptr)delete mesh;
    }

}

//.tkm file ������
void MeshParts::InitFromTkmFile(
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
    D3D12_FILTER samplerFilter)
{
    //�T�C�Y �ݒ�
    this->meshs_.resize(tkmFile.GetNumMesh());

    int mesh_no = 0;
    int materian_no = 0;
    tkmFile.QueryMeshParts([&](const TkmFile::SMesh& mesh) {
        //tkm�t�@�C���̃��b�V����񂩂烁�b�V�� �쐬
        CreateMeshFromTkmMesh(
            tk,
            graphicsEngine,
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
void MeshParts::Draw(
    GraphicsEngine*& graphicsEngine,
    RenderContext& renderContext, 
    const Matrix& matrixWorld, 
    const Matrix& matrixView, 
    const Matrix& matrixProjection
)
{
    //�`�拤�ʏ������s��
    DrawCommon(graphicsEngine,renderContext, matrixWorld, matrixView, matrixProjection);

    int descriptorHeapNo = 0;
    for (auto& mesh : this->meshs_) {
        // ���_�o�b�t�@��ݒ�B
        renderContext.SetVertexBuffer(mesh->vertexBuffer);
        //�}�e���A�����ƂɃh���[�B
        for (int matNo = 0; matNo < mesh->materials.size(); matNo++) {
            //���̃}�e���A�����\���Ă��郁�b�V���̕`��J�n�B
            mesh->materials[matNo]->BeginRender(renderContext, mesh->skinFlags[matNo]);
            // �f�B�X�N���v�^�q�[�v��ݒ�B
            renderContext.SetDescriptorHeap(graphicsEngine,this->descriptor_Heap_);
            // �C���f�b�N�X�o�b�t�@��ݒ�B
            auto& ib = mesh->indexBufferArray[matNo];
            renderContext.SetIndexBuffer(*ib);

            //�h���[�R�[�������s�B
            renderContext.DrawIndexed(ib->GetIndexCount());
            descriptorHeapNo++;
        }
    }
}

//�C���X�^���V���O�`��
void MeshParts::DrawInstancing(GraphicsEngine*& graphicsEngine,RenderContext& renderContext, int numInstance, const Matrix& matrixView, const Matrix& matrixProjection)
{
    //�萔�o�b�t�@�̐ݒ�A�X�V�ȂǕ`��̋��ʏ��������s����B
    DrawCommon(graphicsEngine,renderContext, Matrix::GetIdentity(), matrixView, matrixProjection);

    int descriptor_heap_no = 0;
    for (auto& mesh : this->meshs_)
    {
        //���_�o�b�t�@�ݒ�
        renderContext.SetVertexBuffer(mesh->vertexBuffer);

        //�}�e���A�����Ƃɕ`��
        for (int materialNo = 0; materialNo < mesh->materials.size(); materialNo++)
        {
            //�}�e���A���������Ă��郁�b�V���̕`��J�n
            mesh->materials[materialNo]->BeginRender(renderContext, mesh->skinFlags[materialNo]);

            //�f�B�X�N���v�^�q�[�v �ݒ�
            renderContext.SetDescriptorHeap(graphicsEngine, this->descriptor_Heap_);

            //�C���f�b�N�X�o�b�t�@�ݒ�
            auto& ib = mesh->indexBufferArray[materialNo];
            renderContext.SetIndexBuffer(*ib);
            
            //�h���[�R�[�������s
            renderContext.DrawIndexedInstanced(ib->GetIndexCount(), numInstance);
            descriptor_heap_no++;
        }
    }
}

//�X�P���g�����֘A�t����
void MeshParts::BindSkeleton(GraphicsEngine*& graphicsEngine, Skeleton& skeleton)
{
    this->skelton_ = &skeleton;

    this->bone_Matrices_Structure_Buffer_.Init(
        graphicsEngine,
        sizeof(Matrix),
        this->skelton_->GetNumBones(),
        this->skelton_->GetBoneMatricesTopAddress()
    );

}

//�f�B�X�N���v�^�q�[�v�쐬
void MeshParts::CreateDescriptorHeaps(GraphicsEngine*& graphicsEngine)
{
    //�f�B�X�N���v�^�q�[�v���\�z���Ă����B
    int srv_no = 0;
    int cb_no = 0;

    for (auto& mesh : this->meshs_)
    {
        for (int material_no = 0; material_no < mesh->materials.size(); material_no++) {

            //�f�B�X�N���v�^�q�[�v�Ƀf�B�X�N���v�^��o�^���Ă���
            
            //�A���x�h�}�b�v
            this->descriptor_Heap_.RegistShaderResource(srv_no, mesh->materials[material_no]->GetAlbedoMap());
            //�@���}�b�v
            this->descriptor_Heap_.RegistShaderResource(srv_no+1, mesh->materials[material_no]->GetNormalMap());
            //�X�y�L�����}�b�v
            this->descriptor_Heap_.RegistShaderResource(srv_no+2, mesh->materials[material_no]->GetSpecularMap());
            //�{�[���̃X�g���N�`���[�h�o�b�t�@
            this->descriptor_Heap_.RegistShaderResource(srv_no + 3, this->bone_Matrices_Structure_Buffer_);

            for (int i = 0; i < MAX_MODEL_EXPAND_SRV; i++) {
                if (this->expand_Shader_Resource_View_[i]) {
                    this->descriptor_Heap_.RegistShaderResource(srv_no + EXPAND_SRV_REG__START_NO + i, *this->expand_Shader_Resource_View_[i]);
                }
            }

            srv_no += NUM_SRV_ONE_MATERIAL;
            this->descriptor_Heap_.RegistConstantBuffer(cb_no, this->common_Constant_Buffer_);
            if (this->expand_Constant_Buffer_.IsValid()) {
                this->descriptor_Heap_.RegistConstantBuffer(cb_no + 1, this->expand_Constant_Buffer_);
            }
            cb_no += NUM_CBV_ONE_MATERIAL;
        }
    }
    this->descriptor_Heap_.Commit(graphicsEngine);
}

void MeshParts::CreateMeshFromTkmMesh(
    tkEngine*& tk,
    GraphicsEngine *& graphicsEngine,
    const TkmFile::SMesh& tkmMesh,
    int meshNo, 
    int& materialNum,
    const char* fxFilePath,
    const char* vsEntryPointFuncName,
    const char* vsSkinEntryPointFuncName,
    const char* psEntryPointFuncName,
    const std::array<DXGI_FORMAT,
    static_cast<int>D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT>& colorBufferFormat,
    D3D12_FILTER samplerFilter
)
{
    //���_�o�b�t�@ �쐬
    int num_vertex = (int)tkmMesh.vertexBuffer.size();
    int vertex_stride = sizeof(TkmFile::SVertex);
    auto mesh = new SMesh;
    mesh->skinFlags.reserve(tkmMesh.materials.size());
    mesh->vertexBuffer.Init(graphicsEngine, vertex_stride * num_vertex, vertex_stride);
    mesh->vertexBuffer.Copy((void*)&tkmMesh.vertexBuffer[0]);

    auto SetSkinFlag = [&](int index) {
        if (tkmMesh.vertexBuffer[index].skinWeights.x > 0.0f) {
            //�X�L��������B
            mesh->skinFlags.push_back(1);
        }
        else {
            //�X�L���Ȃ��B
            mesh->skinFlags.push_back(0);
        }
    };

    //�C���f�b�N�X�o�b�t�@�쐬
    if (!tkmMesh.indexBuffer16Array.empty())
    {
        //�C���f�b�N�X�̃T�C�Y��2byte
        mesh->indexBufferArray.reserve(tkmMesh.indexBuffer16Array.size());
        for (auto& tkIb : tkmMesh.indexBuffer16Array) {
            auto ib = new IndexBuffer;
            //std::unique_ptr<IndexBuffer>ib = std::make_unique<IndexBuffer>();
            ib->Init(graphicsEngine,static_cast<int>(tkIb.indices.size()) * 2, 2);
            ib->Copy((uint16_t*)&tkIb.indices.at(0));

            //�X�L�������邩�ǂ�����ݒ肷��B
            SetSkinFlag(tkIb.indices[0]);

            mesh->indexBufferArray.push_back(ib);
            //delete  ib;
        }
    }
    else
    {
        //�C���f�b�N�X�̃T�C�Y��4byte
        mesh->indexBufferArray.reserve(tkmMesh.indexBuffer32Array.size());
        for (auto& tkIb : tkmMesh.indexBuffer32Array) {
            auto ib = new IndexBuffer;
            ib->Init(graphicsEngine,static_cast<int>(tkIb.indices.size()) * 4, 4);
            ib->Copy((uint32_t*)&tkIb.indices.at(0));

            //�X�L�������邩�ǂ�����ݒ肷��B
            SetSkinFlag(tkIb.indices[0]);

            mesh->indexBufferArray.push_back(ib);
        }
    }

    //�}�e���A���쐬
    mesh->materials.reserve(tkmMesh.materials.size());
    for (auto& tkmMat : tkmMesh.materials) {
        auto* mat = new Material();
        mat->InitFromTkmMaterila(
            tk,
            graphicsEngine,
            tkmMat,
            fxFilePath,
            vsEntryPointFuncName,
            vsSkinEntryPointFuncName,
            psEntryPointFuncName,
            colorBufferFormat,
            NUM_SRV_ONE_MATERIAL,
            NUM_CBV_ONE_MATERIAL,
            NUM_CBV_ONE_MATERIAL * materialNum,
            NUM_SRV_ONE_MATERIAL * materialNum,
            samplerFilter
        );
        //�쐬�����}�e���A�������J�E���g����B
        materialNum++;
        mesh->materials.push_back(mat);
    }

    this->meshs_[meshNo] = mesh;

}


//�`�揈���̋��ʏ���
void MeshParts::DrawCommon(GraphicsEngine*& graphicsEngine,RenderContext& renderContext, const Matrix& matrixWorld, const Matrix& matrixView, const Matrix& matrixProjection)
{
    //���b�V�����ƂɃh���[
    //�v���~�e�B�u�̃g�|���W�[�̓g���C�A���O�����X�g�̂�
    renderContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //�萔�o�b�t�@���X�V����
    SConstantBuffer constant_buffer;
    constant_buffer.world = matrixWorld;
    constant_buffer.proj= matrixProjection;
    constant_buffer.view = matrixView;
    this->common_Constant_Buffer_.CopyToVRAM(graphicsEngine, constant_buffer);

    if (this->expand_Data_) {
        this->expand_Constant_Buffer_.CopyToVRAM(graphicsEngine, this->expand_Data_);
    }
    if (this->bone_Matrices_Structure_Buffer_.IsInited()) {
        //�{�[���s����X�V����B
        this->bone_Matrices_Structure_Buffer_.Update(graphicsEngine,this->skelton_->GetBoneMatricesTopAddress());
    }
}

#include "MeshParts.h"
#include "TkmFile.h"
//#include "RenderContext.h"
#include "RenderContext_inline.h"
#include "Material.h"
#include "Skeleton.h"
//コンストラクタ
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

//デストラクタ
MeshParts::~MeshParts()
{
    if (skelton_)delete skelton_;

    for (auto& mesh : this->meshs_) {
        //インデックスバッファを削除。
        for (auto& ib : mesh->indexBufferArray) {
            if (ib != nullptr)delete ib;
        }
        //マテリアルを削除。
        for (auto& mat : mesh->materials) {
            if(mat != nullptr)delete mat;
        }
        //メッシュを削除。
        if(mesh != nullptr)delete mesh;
    }

}

//.tkm file 初期化
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
    //サイズ 設定
    this->meshs_.resize(tkmFile.GetNumMesh());

    int mesh_no = 0;
    int materian_no = 0;
    tkmFile.QueryMeshParts([&](const TkmFile::SMesh& mesh) {
        //tkmファイルのメッシュ情報からメッシュ 作成
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
    //共通定数バッファの作成。
    this->common_Constant_Buffer_.Init(graphicsEngine,sizeof(SConstantBuffer), nullptr);
    //ユーザー拡張用の定数バッファを作成。
    if (expandData) {
        this->expand_Constant_Buffer_.Init(graphicsEngine,expandDataSize, nullptr);
        this->expand_Data_ = expandData;
    }
    for (int i = 0; i < MAX_MODEL_EXPAND_SRV; i++) {
        this->expand_Shader_Resource_View_[i] = expandShaderResourceView[i];
    }
    //ディスクリプタヒープを作成。
    CreateDescriptorHeaps(graphicsEngine);
}

//描画
void MeshParts::Draw(
    GraphicsEngine*& graphicsEngine,
    RenderContext& renderContext, 
    const Matrix& matrixWorld, 
    const Matrix& matrixView, 
    const Matrix& matrixProjection
)
{
    //描画共通処理を行う
    DrawCommon(graphicsEngine,renderContext, matrixWorld, matrixView, matrixProjection);

    int descriptorHeapNo = 0;
    for (auto& mesh : this->meshs_) {
        // 頂点バッファを設定。
        renderContext.SetVertexBuffer(mesh->vertexBuffer);
        //マテリアルごとにドロー。
        for (int matNo = 0; matNo < mesh->materials.size(); matNo++) {
            //このマテリアルが貼られているメッシュの描画開始。
            mesh->materials[matNo]->BeginRender(renderContext, mesh->skinFlags[matNo]);
            // ディスクリプタヒープを設定。
            renderContext.SetDescriptorHeap(graphicsEngine,this->descriptor_Heap_);
            // インデックスバッファを設定。
            auto& ib = mesh->indexBufferArray[matNo];
            renderContext.SetIndexBuffer(*ib);

            //ドローコールを実行。
            renderContext.DrawIndexed(ib->GetIndexCount());
            descriptorHeapNo++;
        }
    }
}

//インスタンシング描画
void MeshParts::DrawInstancing(GraphicsEngine*& graphicsEngine,RenderContext& renderContext, int numInstance, const Matrix& matrixView, const Matrix& matrixProjection)
{
    //定数バッファの設定、更新など描画の共通処理を実行する。
    DrawCommon(graphicsEngine,renderContext, Matrix::GetIdentity(), matrixView, matrixProjection);

    int descriptor_heap_no = 0;
    for (auto& mesh : this->meshs_)
    {
        //頂点バッファ設定
        renderContext.SetVertexBuffer(mesh->vertexBuffer);

        //マテリアルごとに描画
        for (int materialNo = 0; materialNo < mesh->materials.size(); materialNo++)
        {
            //マテリアルが張られているメッシュの描画開始
            mesh->materials[materialNo]->BeginRender(renderContext, mesh->skinFlags[materialNo]);

            //ディスクリプタヒープ 設定
            renderContext.SetDescriptorHeap(graphicsEngine, this->descriptor_Heap_);

            //インデックスバッファ設定
            auto& ib = mesh->indexBufferArray[materialNo];
            renderContext.SetIndexBuffer(*ib);
            
            //ドローコールを実行
            renderContext.DrawIndexedInstanced(ib->GetIndexCount(), numInstance);
            descriptor_heap_no++;
        }
    }
}

//スケルトンを関連付ける
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

//ディスクリプタヒープ作成
void MeshParts::CreateDescriptorHeaps(GraphicsEngine*& graphicsEngine)
{
    //ディスクリプタヒープを構築していく。
    int srv_no = 0;
    int cb_no = 0;

    for (auto& mesh : this->meshs_)
    {
        for (int material_no = 0; material_no < mesh->materials.size(); material_no++) {

            //ディスクリプタヒープにディスクリプタを登録していく
            
            //アルベドマップ
            this->descriptor_Heap_.RegistShaderResource(srv_no, mesh->materials[material_no]->GetAlbedoMap());
            //法線マップ
            this->descriptor_Heap_.RegistShaderResource(srv_no+1, mesh->materials[material_no]->GetNormalMap());
            //スペキュラマップ
            this->descriptor_Heap_.RegistShaderResource(srv_no+2, mesh->materials[material_no]->GetSpecularMap());
            //ボーンのストラクチャードバッファ
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
    //頂点バッファ 作成
    int num_vertex = (int)tkmMesh.vertexBuffer.size();
    int vertex_stride = sizeof(TkmFile::SVertex);
    auto mesh = new SMesh;
    mesh->skinFlags.reserve(tkmMesh.materials.size());
    mesh->vertexBuffer.Init(graphicsEngine, vertex_stride * num_vertex, vertex_stride);
    mesh->vertexBuffer.Copy((void*)&tkmMesh.vertexBuffer[0]);

    auto SetSkinFlag = [&](int index) {
        if (tkmMesh.vertexBuffer[index].skinWeights.x > 0.0f) {
            //スキンがある。
            mesh->skinFlags.push_back(1);
        }
        else {
            //スキンなし。
            mesh->skinFlags.push_back(0);
        }
    };

    //インデックスバッファ作成
    if (!tkmMesh.indexBuffer16Array.empty())
    {
        //インデックスのサイズが2byte
        mesh->indexBufferArray.reserve(tkmMesh.indexBuffer16Array.size());
        for (auto& tkIb : tkmMesh.indexBuffer16Array) {
            auto ib = new IndexBuffer;
            //std::unique_ptr<IndexBuffer>ib = std::make_unique<IndexBuffer>();
            ib->Init(graphicsEngine,static_cast<int>(tkIb.indices.size()) * 2, 2);
            ib->Copy((uint16_t*)&tkIb.indices.at(0));

            //スキンがあるかどうかを設定する。
            SetSkinFlag(tkIb.indices[0]);

            mesh->indexBufferArray.push_back(ib);
            //delete  ib;
        }
    }
    else
    {
        //インデックスのサイズが4byte
        mesh->indexBufferArray.reserve(tkmMesh.indexBuffer32Array.size());
        for (auto& tkIb : tkmMesh.indexBuffer32Array) {
            auto ib = new IndexBuffer;
            ib->Init(graphicsEngine,static_cast<int>(tkIb.indices.size()) * 4, 4);
            ib->Copy((uint32_t*)&tkIb.indices.at(0));

            //スキンがあるかどうかを設定する。
            SetSkinFlag(tkIb.indices[0]);

            mesh->indexBufferArray.push_back(ib);
        }
    }

    //マテリアル作成
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
        //作成したマテリアル数をカウントする。
        materialNum++;
        mesh->materials.push_back(mat);
    }

    this->meshs_[meshNo] = mesh;

}


//描画処理の共通処理
void MeshParts::DrawCommon(GraphicsEngine*& graphicsEngine,RenderContext& renderContext, const Matrix& matrixWorld, const Matrix& matrixView, const Matrix& matrixProjection)
{
    //メッシュごとにドロー
    //プリミティブのトポロジーはトライアングルリストのみ
    renderContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //定数バッファを更新する
    SConstantBuffer constant_buffer;
    constant_buffer.world = matrixWorld;
    constant_buffer.proj= matrixProjection;
    constant_buffer.view = matrixView;
    this->common_Constant_Buffer_.CopyToVRAM(graphicsEngine, constant_buffer);

    if (this->expand_Data_) {
        this->expand_Constant_Buffer_.CopyToVRAM(graphicsEngine, this->expand_Data_);
    }
    if (this->bone_Matrices_Structure_Buffer_.IsInited()) {
        //ボーン行列を更新する。
        this->bone_Matrices_Structure_Buffer_.Update(graphicsEngine,this->skelton_->GetBoneMatricesTopAddress());
    }
}

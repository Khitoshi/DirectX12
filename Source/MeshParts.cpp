#include "MeshParts.h"
#include "TkmFile.h"
#include "RenderContext.h"
#include "Material.h"

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
}

//.tkm file 初期化
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
    //サイズ 設定
    this->meshs_.resize(tkmFile.GetNumMesh());

    int mesh_no = 0;
    int materian_no = 0;
    tkmFile.QueryMeshParts([&](const TkmFile::SMesh& mesh) {
        //tkmファイルのメッシュ情報からメッシュ 作成
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
void MeshParts::Draw(RenderContext& renderContext, const Matrix& matrixWorld, const Matrix& matrixView, const Matrix& matrixProjection)
{
    //描画共通処理を行う
    DrawCommon(renderContext, matrixWorld, matrixView, matrixProjection);

    int descriptor_heap_number = 0;
    for (auto& mesh : this->meshs_)
    {
        //頂点バッファを設定
        renderContext.SetVertexBuffer(mesh->vertexBuffer);
        //マテリアルごとにドロー
        for (int material_number = 0; material_number < mesh->materials.size(); material_number++)
        {
            //このマテリアルが張られているメッシュの描画開始
            mesh->materials[material_number]->BeginRender(renderContext, mesh->skinFlags[material_number]);

            renderContext.SetDescriptorHeap(this->descriptor_Heap_);
        }
    }

}

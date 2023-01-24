#include "Material.h"
#include "RenderContext.h"
#include "PipelineState.h"
#include "GraphicsEngine.h"
#include "tkEngine.h"

//デフォルト コンストラクタ
Material::Material():
    albedo_Map_(),
    normal_Map_(),
    specular_Map_(),
    reflection_Map_(),
    refraction_Map_(),
    vs_Non_Skin_Model_(nullptr),
    vs_Skin_Model_(nullptr),
    ps_Model_(nullptr),
    non_Skin_Model_Pipeline_State_(),
    skin_Model_Pipeline_State_(),
    trans_Skin_Model_Pipeline_State_(),
    trans_Non_Skin_Model_Pipeline_State_(),
    constant_Buffer_(),
    root_Signature_()
{
}

//デフォルト デストラクタ
Material::~Material()
{
}

//tkm file のマテリアル情報から初期化する
void Material::InitFromTkmMaterila(
    tkEngine* tkEngine,
    GraphicsEngine* graphicsEngine,
    const TkmFile::SMaterial& tkmMat, 
    const char* fxFilePath,
    const char* vsEntryPointFuncName, 
    const char* vsSkinEntryPointFuncName, 
    const char* psEntryPointFuncName, 
    const std::array<DXGI_FORMAT, 
    static_cast<int>(D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT)>& colorBufferFormat, 
    int numSrv, 
    int numCbv, 
    UINT offsetInDescriptorsFromTableStartCB, 
    UINT offsetInDescriptorsFromTableStartSRV, 
    D3D12_FILTER samplerFilter
)
{
    //テクスチャをロード
    InitTexture(tkEngine, graphicsEngine, tkmMat);

    //定数バッファ 生成
    SMaterialParam material_param;
    material_param.hasNormalMap = this->normal_Map_.get()->IsValid() ? 1 : 0;
    material_param.hasSpecMap = this->specular_Map_.get()->IsValid() ? 1 : 0;
    this->constant_Buffer_.Init(graphicsEngine, sizeof(SMaterialParam), &material_param);

    //ルートシグネチャを初期化(デフォルトとシャドウの2つ)
    D3D12_STATIC_SAMPLER_DESC sampler_desc_array[2];
    
    //デフォルトサンプラ
    sampler_desc_array[0].Filter = samplerFilter;
    sampler_desc_array[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    sampler_desc_array[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    sampler_desc_array[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    sampler_desc_array[0].MipLODBias = 0;
    sampler_desc_array[0].MaxAnisotropy = 0;
    sampler_desc_array[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    sampler_desc_array[0].BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
    sampler_desc_array[0].MinLOD = 0.0f;
    sampler_desc_array[0].MaxLOD = D3D12_FLOAT32_MAX;
    sampler_desc_array[0].ShaderRegister = 0;
    sampler_desc_array[0].RegisterSpace = 0;
    sampler_desc_array[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

    //シャドウマップ用サンプラ
    sampler_desc_array[1] = sampler_desc_array[0];
    //比較対象の値が小さければ0 大きければ1を返す比較関数を設定する
    sampler_desc_array[1].Filter = D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
    sampler_desc_array[1].ComparisonFunc = D3D12_COMPARISON_FUNC_GREATER;
    sampler_desc_array[1].MaxAnisotropy = 1;
    sampler_desc_array[1].ShaderRegister = 1;

    //初期化
    this->root_Signature_.Init(
        graphicsEngine,
        sampler_desc_array,
        2,
        numCbv,
        numSrv,
        8,
        offsetInDescriptorsFromTableStartCB,
        offsetInDescriptorsFromTableStartSRV
    );

    //.fx file path が存在する場合
    if (fxFilePath != nullptr && strlen(fxFilePath) > 0) {
        //シェーダーを初期化。
        InitShaders(
            tkEngine,
            fxFilePath,
            vsEntryPointFuncName,
            vsSkinEntryPointFuncName,
            psEntryPointFuncName
        );
        //パイプラインステートを初期化。
        InitPipelineState(graphicsEngine,colorBufferFormat);
    }
}

//レンダリング開始する時に呼び出す関数
void Material::BeginRender(RenderContext& rc, int hasSkin)
{
    //ルートシグネチャ　セット
    rc.SetRootSignature(this->root_Signature_);
    if (hasSkin)
    {
        rc.SetPipelineState(this->trans_Skin_Model_Pipeline_State_);
    }
    else
    {
        rc.SetPipelineState(this->trans_Non_Skin_Model_Pipeline_State_);
    }
}

void Material::InitPipelineState(GraphicsEngine* graphicsEngine,const std::array<DXGI_FORMAT, static_cast<int>(D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT)>& colorBufferFormat)
{
    // 頂点レイアウトを定義する。
    D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 36, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 48, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_SINT, 0, 56, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 72, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    };

    //パイプラインステートを設定
    D3D12_GRAPHICS_PIPELINE_STATE_DESC pso_desc = { 0 };
    pso_desc.InputLayout = { inputElementDescs, _countof(inputElementDescs) };
    pso_desc.pRootSignature = this->root_Signature_.GetRootSignature();
    pso_desc.VS = CD3DX12_SHADER_BYTECODE(this->vs_Skin_Model_->GetCompiledBlob());
    pso_desc.PS = CD3DX12_SHADER_BYTECODE(this->ps_Model_->GetCompiledBlob());
    pso_desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    pso_desc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
    pso_desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

    pso_desc.DepthStencilState.DepthEnable = TRUE;
    pso_desc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    pso_desc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
    pso_desc.DepthStencilState.StencilEnable = FALSE;
    pso_desc.SampleMask = UINT_MAX;
    pso_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

    int num_render_target = 0;
    for (auto& format : colorBufferFormat) {
        if (format == DXGI_FORMAT_UNKNOWN) {
            //フォーマットが指定されていない場所が来たら終わり。
            break;
        }
        pso_desc.RTVFormats[num_render_target] = colorBufferFormat[num_render_target];
        num_render_target++;
    }
    pso_desc.NumRenderTargets = num_render_target;
    pso_desc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
    pso_desc.SampleDesc.Count = 1;


    this->skin_Model_Pipeline_State_.Init(graphicsEngine,pso_desc);

    //続いてスキンなしモデル用を作成。
    pso_desc.VS = CD3DX12_SHADER_BYTECODE(this->vs_Non_Skin_Model_->GetCompiledBlob());
    this->non_Skin_Model_Pipeline_State_.Init(graphicsEngine,pso_desc);

    //続いて半透明マテリアル用。
    pso_desc.VS = CD3DX12_SHADER_BYTECODE(this->vs_Skin_Model_->GetCompiledBlob());
    pso_desc.BlendState.IndependentBlendEnable = TRUE;
    pso_desc.BlendState.RenderTarget[0].BlendEnable = TRUE;
    pso_desc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
    pso_desc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
    pso_desc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;


    this->trans_Skin_Model_Pipeline_State_.Init(graphicsEngine,pso_desc);

    pso_desc.VS = CD3DX12_SHADER_BYTECODE(this->vs_Non_Skin_Model_->GetCompiledBlob());
    this->trans_Non_Skin_Model_Pipeline_State_.Init(graphicsEngine,pso_desc);


}

void Material::InitShaders(
    tkEngine* tk,
    const char* fxFilePath, 
    const char* vsEntryPointFuncName, 
    const char* vsSkinEntriyPointFuncName, 
    const char* psEntryPointFuncName)
{
    //スキンなしモデル用のシェーダーをロードする。
    this->vs_Non_Skin_Model_ = tk->GetShaderFromBank(fxFilePath, vsEntryPointFuncName);
    if (this->vs_Non_Skin_Model_ == nullptr) {
        this->vs_Non_Skin_Model_ = new Shader;
        this->vs_Non_Skin_Model_->LoadVS(fxFilePath, vsEntryPointFuncName);
        tk->RegistShaderToBank(fxFilePath, vsEntryPointFuncName, this->vs_Non_Skin_Model_);
    }
    //スキンありモデル用のシェーダーをロードする。
    this->vs_Skin_Model_ = tk->GetShaderFromBank(fxFilePath, vsSkinEntriyPointFuncName);
    if (this->vs_Skin_Model_ == nullptr) {
        this->vs_Skin_Model_ = new Shader;
        this->vs_Skin_Model_->LoadVS(fxFilePath, vsSkinEntriyPointFuncName);
        tk->RegistShaderToBank(fxFilePath, vsSkinEntriyPointFuncName, this->vs_Skin_Model_);
    }

    this->ps_Model_ = tk->GetShaderFromBank(fxFilePath, psEntryPointFuncName);
    if (this->ps_Model_ == nullptr) {
        this->ps_Model_ = new Shader;
        this->ps_Model_->LoadPS(fxFilePath, psEntryPointFuncName);
        tk->RegistShaderToBank(fxFilePath, psEntryPointFuncName, this->ps_Model_);
    }
}

void Material::InitTexture(
    tkEngine* tk,
    GraphicsEngine* graphicsEngine,
    const TkmFile::SMaterial& tkmMat)
{
    const auto& nullTextureMaps = graphicsEngine->GetNullTextureMaps();

    const char* filePath = nullptr;
    char* map = nullptr;
    unsigned int mapSize;
    
    //アルベドマップ。
    {
        if (tkmMat.albedoMap != nullptr)
        {
            filePath = tkmMat.albedoMapFilePath.c_str();
            map = tkmMat.albedoMap.get();
            mapSize = tkmMat.albedoMapSize;
        }
        else
        {
            filePath = nullTextureMaps.GetAlbedoMapFilePath();
            map = nullTextureMaps.GetAlbedoMap().get();
            mapSize = nullTextureMaps.GetAlbedoMapSize();
        }
        auto albedoMap = tk->GetTextureFromBank(filePath);

        if (albedoMap == nullptr)
        {
            albedoMap = new Texture();
            albedoMap->InitFromMemory(graphicsEngine,map, mapSize);
            tk->RegistTextureToBank(filePath, albedoMap);
        }
        this->albedo_Map_.reset(albedoMap);
    }


    //法線マップ。
    {
        if (tkmMat.normalMap != nullptr)
        {
            filePath = tkmMat.normalMapFilePath.c_str();
            map = tkmMat.normalMap.get();
            mapSize = tkmMat.normalMapSize;
        }
        else
        {
            filePath = nullTextureMaps.GetNormalMapFilePath();
            map = nullTextureMaps.GetNormalMap().get();
            mapSize = nullTextureMaps.GetNormalMapSize();
        }
        auto normalMap = tk->GetTextureFromBank(filePath);

        if (normalMap == nullptr)
        {
            normalMap = new Texture();
            normalMap->InitFromMemory(graphicsEngine,map, mapSize);
            tk->RegistTextureToBank(filePath, normalMap);
        }
        this->normal_Map_.reset(normalMap);
    }



    //スペキュラマップ。
    {
        if (tkmMat.specularMap != nullptr)
        {
            filePath = tkmMat.specularMapFilePath.c_str();
            map = tkmMat.specularMap.get();
            mapSize = tkmMat.specularMapSize;
        }
        else
        {
            filePath = nullTextureMaps.GetSpecularMapFilePath();
            map = nullTextureMaps.GetSpecularMap().get();
            mapSize = nullTextureMaps.GetSpecularMapSize();
        }
        auto specularMap = tk->GetTextureFromBank(filePath);

        if (specularMap == nullptr)
        {
            specularMap = new Texture();
            specularMap->InitFromMemory(graphicsEngine,map, mapSize);
            tk->RegistTextureToBank(filePath, specularMap);
        }
        this->specular_Map_.reset(specularMap);
    }

    //反射マップ。
    {
        if (tkmMat.reflectionMap != nullptr)
        {
            filePath = tkmMat.reflectionMapFilePath.c_str();
            map = tkmMat.reflectionMap.get();
            mapSize = tkmMat.reflectionMapSize;
        }
        else
        {
            filePath = nullTextureMaps.GetReflectionMapFilePath();
            map = nullTextureMaps.GetReflectionMap().get();
            mapSize = nullTextureMaps.GetReflectionMapSize();
        }
        auto reflectionMap = tk->GetTextureFromBank(filePath);

        if (reflectionMap == nullptr)
        {
            reflectionMap = new Texture();
            reflectionMap->InitFromMemory(graphicsEngine,map, mapSize);
            tk->RegistTextureToBank(filePath, reflectionMap);
        }
        this->reflection_Map_.reset(reflectionMap);
    }

    //屈折マップ。
    {
        if (tkmMat.refractionMap != nullptr)
        {
            filePath = tkmMat.refractionMapFilePath.c_str();
            map = tkmMat.refractionMap.get();
            mapSize = tkmMat.refractionMapSize;
        }
        else
        {
            filePath = nullTextureMaps.GetReflectionMapFilePath();
            map = nullTextureMaps.GetReflectionMap().get();
            mapSize = nullTextureMaps.GetReflectionMapSize();
        }
        auto refractionMap = tk->GetTextureFromBank(filePath);

        if (refractionMap == nullptr)
        {
            refractionMap = new Texture();
            refractionMap->InitFromMemory(graphicsEngine,map, mapSize);
            tk->RegistTextureToBank(filePath, refractionMap);
        }
        this->refraction_Map_.reset(refractionMap);
    }


}

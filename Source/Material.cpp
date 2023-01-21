#include "Material.h"
#include "RenderContext.h"
#include "PipelineState.h"
#include "GraphicsEngine.h"
#include "tkEngine.h"

//�f�t�H���g �R���X�g���N�^
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

//�f�t�H���g �f�X�g���N�^
Material::~Material()
{
}

//tkm file �̃}�e���A����񂩂珉��������
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
    //�e�N�X�`�������[�h
    InitTexture(tkmMat);

    //�萔�o�b�t�@ ����
    SMaterialParam material_param;
    material_param.hasNormalMap = this->normal_Map_.get()->IsValid() ? 1 : 0;
    material_param.hasSpecMap = this->specular_Map_.get()->IsValid() ? 1 : 0;
    this->constant_Buffer_.Init(graphicsEngine, sizeof(SMaterialParam), &material_param);

    //���[�g�V�O�l�`����������(�f�t�H���g�ƃV���h�E��2��)
    D3D12_STATIC_SAMPLER_DESC sampler_desc_array[2];
    
    //�f�t�H���g�T���v��
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

    //�V���h�E�}�b�v�p�T���v��
    sampler_desc_array[1] = sampler_desc_array[0];
    //��r�Ώۂ̒l�����������0 �傫�����1��Ԃ���r�֐���ݒ肷��
    sampler_desc_array[1].Filter = D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
    sampler_desc_array[1].ComparisonFunc = D3D12_COMPARISON_FUNC_GREATER;
    sampler_desc_array[1].MaxAnisotropy = 1;
    sampler_desc_array[1].ShaderRegister = 1;

    //������
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

    //.fx file path �����݂���ꍇ
    if (fxFilePath != nullptr && strlen(fxFilePath) > 0) {
        //�V�F�[�_�[���������B
        InitShaders(
            tkEngine,
            fxFilePath,
            vsEntryPointFuncName,
            vsSkinEntryPointFuncName,
            psEntryPointFuncName
        );
        //�p�C�v���C���X�e�[�g���������B
        InitPipelineState(colorBufferFormat);
    }
}

//�����_�����O�J�n���鎞�ɌĂяo���֐�
void Material::BeginRender(RenderContext& rc, int hasSkin)
{
    //���[�g�V�O�l�`���@�Z�b�g
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

void Material::InitShaders(
    tkEngine* tkEngine,
    const char* fxFilePath, 
    const char* vsEntryPointFuncName, 
    const char* vsSkinEntriyPointFuncName, 
    const char* psEntryPointFuncName)
{
    //TODO:������������
    /*
    this->vs_Non_Skin_Model_ = std::make_unique<Shader>(tkEngine->GetShaderFromBank(fxFilePath, vsEntryPointFuncName));
    if (this->vs_Non_Skin_Model_ == nullptr) {
        this->vs_Non_Skin_Model_ = std::make_unique <Shader>();
        this->vs_Non_Skin_Model_->LoadVS(fxFilePath, vsEntryPointFuncName);
        tkEngine->RegistShaderToBank(fxFilePath, vsEntryPointFuncName, this->vs_Non_Skin_Model_.get());
    }

    //�X�L�����胂�f���p�̃V�F�[�_�[�����[�h����B
    this->vs_Skin_Model_ = std::make_unique<Shader>(tkEngine->GetShaderFromBank(fxFilePath, vsSkinEntriyPointFuncName));
    if (this->vs_Skin_Model_ == nullptr) {
        this->vs_Skin_Model_ = std::make_unique<Shader>();
        this->vs_Skin_Model_->LoadVS(fxFilePath, vsSkinEntriyPointFuncName);
        tkEngine->RegistShaderToBank(fxFilePath, vsSkinEntriyPointFuncName, this->vs_Skin_Model_.get());
    }

    this->ps_Model_ = std::make_unique<Shader>(tkEngine->GetShaderFromBank(fxFilePath, psEntryPointFuncName));
    if (this->ps_Model_ == nullptr) {
        this->ps_Model_ = std::make_unique<Shader>();
        this->ps_Model_->LoadPS(fxFilePath, psEntryPointFuncName);
        tkEngine->RegistShaderToBank(fxFilePath, psEntryPointFuncName, this->ps_Model_.get());
    }
    */
}

void Material::InitTexture(const TkmFile::SMaterial& tkmMat)
{
}

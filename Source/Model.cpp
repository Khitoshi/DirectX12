#include "Model.h"
#include "Camera.h"

//�f�t�H���g �R���X�g���N�^
Model::Model():
    is_Inited_(false),
    world_(),
    tkm_file_(nullptr),
    skeleton_(),
    mesh_parts_(),
    model_up_axis_(EnModelUpAxis::enModelUpAxisY)
{
}

//�f�t�H���g �f�X�g���N�^
Model::~Model()
{
    if (tkm_file_ != nullptr)
    {
        delete tkm_file_;
    }
}

//������
void Model::Init(tkEngine*& tk, GraphicsEngine*& graphicsEngine, const ModelInitData& initData)
{
    //�t�@�C���p�X�̃`�F�b�N
    if (initData.model_File_Path_ == nullptr)
    {
        //���b�Z�[�W�{�b�N�X��\�����āC�ُ�I������
        MessageBox(nullptr, TEXT("Model::Init�Ń��f���̃t�@�C���p�X���w�肳��Ă��܂���"), TEXT("�G���["), MB_OK);
        std::abort();
    }

    //�X�P���g���̃`�F�b�N
    if (initData.skeleton_ != nullptr) {
        //�X�P���g�����w�肳��Ă���B
        this->mesh_parts_.BindSkeleton(graphicsEngine, *initData.skeleton_);
    }

    //���f���̏�����ݒ�
    this->model_up_axis_ = initData.en_model_Up_Axis_;

    //.tkm file �̏d�����`�F�b�N
    auto tkm_file = tk->GetTkmFileFromBank(initData.model_File_Path_);
    if (tkm_file == nullptr) {
        //���o�^�Ȃ̂œo�^����
        tkm_file = new TkmFile;
        tkm_file->Load(initData.model_File_Path_);
        tk->RegistTkmFileToBank(initData.model_File_Path_, tkm_file);
    }

    this->tkm_file_ = tkm_file;

    this->mesh_parts_.InitFromTkmFile(
        tk,
        graphicsEngine,
        *this->tkm_file_,
        initData.shader_File_Path,
        initData.vs_Entry_Point_Func_,
        initData.vs_Skin_Entry_Point_Func_,
        initData.ps_entry_point_func_,
        initData.expand_Constant_Buffer,
        initData.expand_Constant_Buffer_Size,
        initData.expand_Shader_Resoruce_View_,
        initData.color_Buffer_Format_,
        initData.sampler_Filter
    );

    //���[���h�s����v�Z���āC�����o�ϐ���world���X�V����
    UpdateWorldMatrix(Vector3::GetVec3Zero(), Quaternion::GetIdentity(), Vector3::GetVec3One());

    //�����������̂ŏ������t���O�����Ă�
    this->is_Inited_ = true;
}

//���[���h�s����v�Z���āA�����o�ϐ���world�s����X�V����
void Model::UpdateWorldMatrix(Vector3 pos, Quaternion rot, Vector3 scale)
{
    //�X�V
    this->world_ = CalcWorldMatrix(pos, rot, scale);
}

void Model::Draw(GraphicsEngine*& graphicsEngine, RenderContext& rc, Camera& camera)
{
    Draw(
        graphicsEngine,
        rc, 
        camera.GetViewMatrix(graphicsEngine), 
        camera.GetProjectionMatrix(graphicsEngine)
    );
}

//�`��(�J�����s��w���)
void Model::Draw(GraphicsEngine*& graphicsEngine, RenderContext& rc, const Matrix& viewMatrix, const Matrix& projMatrix)
{
    this->mesh_parts_.Draw(
        graphicsEngine,
        rc,
        this->world_,
        viewMatrix,
        projMatrix
    );
}

//�C���X�^���V���O�`��
void Model::DrawInstancing(GraphicsEngine*& graphicsEngine,RenderContext& rc, int numInstance, Camera* camera)
{
    this->mesh_parts_.DrawInstancing(
        graphicsEngine,
        rc,
        numInstance,
        camera->GetViewMatrix(graphicsEngine),
        camera->GetProjectionMatrix(graphicsEngine)
    );
}




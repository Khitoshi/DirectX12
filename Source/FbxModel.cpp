#include "FbxModel.h"

//�R���X�g���N�^
FbxModel::FbxModel():
    mesh_parts_(),
    model_up_axis_()
{
}

//�f�X�g���N�^
FbxModel::~FbxModel()
{
}

//������
void FbxModel::Init(tkEngine* tk, GraphicsEngine* graphicsEngine, const ModelInitData& initData)
{
    //���f���̃t�@�C���p�X�����݂��Ȃ��ꍇ�@�G���[
    if (!initData.model_File_Path_)
    {
        //���b�Z�[�W�{�b�N�X��\�����āC�ُ�I������
        MessageBox(nullptr, TEXT("Model::Init�Ń��f���̃t�@�C���p�X���w�肳��Ă��܂���"), TEXT("�G���["), MB_OK);
        std::abort();
    }

    //�X�P���g���̃`�F�b�N
    if (initData.skeleton_) {
        //�X�P���g�����w�肳��Ă���B
        this->mesh_parts_.BindSkeleton(graphicsEngine, *initData.skeleton_);
    }

    //���f���̏�����ݒ�
    model_up_axis_ = initData.en_model_Up_Axis_;

    //�d���t�@�C���̊m�F
    auto file = tk->GetTkmFileFromBank(initData.model_File_Path_);
    if (file == nullptr) {
        //���o�^�Ȃ̂œo�^����
        file = new TkmFile;
        file->Load(initData.model_File_Path_);
        tk->RegistTkmFileToBank(initData.model_File_Path_, file);
    }




}



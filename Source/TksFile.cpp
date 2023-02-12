#include <Windows.h>

#include "TksFile.h"

//�R���X�g���N�^
TksFile::TksFile():
    num_Bone_(0),
    bones_(0)
{
}

//�f�X�g���N�^
TksFile::~TksFile()
{
}

//.tks �t�@�C�� �ǂݍ���
void TksFile::Load(const char* filePath)
{
    //�t�@�C�� �ǂݍ���
    FILE* fp;
    fopen_s(&fp, filePath, "rb");
    
    //�ǂݍ��� �`�F�b�N
    if (fp)
    {
        //���s�����̂Ń��b�Z�[�W�{�b�N�X��\�����ċ����I��������
        MessageBox(nullptr, TEXT(".tks�t�@�C���̓ǂݍ��݂Ɏ��s���܂���"), TEXT("�G���["), MB_OK);
        std::abort();
    }

    //���̑��� �擾
    fread(&this->num_Bone_, sizeof(this->num_Bone_), 1, fp);

    //�����X�g�̃T�C�Y��ݒ�
    this->bones_.resize(this->num_Bone_);

    //�S���@���@�擾
    for (int i = 0; i < this->num_Bone_; i++)
    {
        //�� �擾
        auto& bone = this->bones_.at(i);


        //���̖��O �擾
        size_t name_count = 0;
        fread(&name_count, 1, 1, fp);
        bone.name = std::make_unique<char[]>(name_count + 1);
        //bone.name = make_unique<std::string>();
        
        //���̖��O �ݒ�
        fread(bone.name.get(), name_count + 1, 1, fp);

        //�e��ID �ݒ�
        fread(&bone.id, sizeof(bone.parent_No), 1, fp);
        
        //�o�C���h�|�[�Y �ݒ�
        fread(&bone.bind_Pose, sizeof(bone.bind_Pose), 1, fp);

        //�o�C���h�|�[�Y�̋t�� �ݒ�
        fread(&bone.inv_Bind_Pose, sizeof(bone.inv_Bind_Pose), 1, fp);
        
        //�{�[���̔ԍ� �ݒ�
        bone.id = i;
    }

    //�t�@�C�� �����
    fclose(fp);
}

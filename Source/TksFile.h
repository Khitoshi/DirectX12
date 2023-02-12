#pragma once

#include <memory>
#include <vector>
#include <string>
#include <functional>

class TksFile
{
public:
    /// <summary>
    /// �{�[��
    /// </summary>
    struct SBone {
        //�{�[���̖��O
        std::unique_ptr<char[]> name;
        //�e�ԍ�
        int parent_No;
        //�o�C���h�|�[�Y
        float bind_Pose[4][3];
        //�o�C���h�|�[�Y�̋t��
        float inv_Bind_Pose[4][3];
        //�{�[���̔ԍ�
        int id;
    };

public:

    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    TksFile();

    /// <summary>
    /// �f�X�g���N�^
    /// </summary>
    ~TksFile();


    /// <summary>
    /// .tks �t�@�C���@�ǂݍ���
    /// </summary>
    /// <param name="filePath"></param>
    void Load(const char* filePath);

    /// <summary>
    /// �{�[���ɑ΂��ăN�G�����s��
    /// </summary>
    /// <param name="query">�N�G���֐�</param>
    void QueryBone(std::function<void(SBone& bone)>query)
    {
        for (auto& bone : this->bones_)
        {
            query(bone);
        }
    }

private:
    //���̑���
    int num_Bone_;

    //���̃��X�g
    std::vector<SBone> bones_;

};

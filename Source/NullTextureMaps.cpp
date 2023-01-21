#include "NullTextureMaps.h"
#include <memory>
#include <Windows.h>


NullTextureMaps::NullTextureMaps():
	albedo_Map_(),
	albedo_Map_File_Path_(),
	albedo_Map_Size_(),
	normal_Map_(),
	normal_Map_File_Path_(),
	normal_Map_Size_(),
	zero_Value_Map_(),
	zero_Value_Map_File_Path_(),
	zero_Value_Map_Size_()
{
}
NullTextureMaps::~NullTextureMaps()
{
}
//������
void NullTextureMaps::Init()
{
	//�e�N�X�`�����[�h
	auto TexLoad = [&](
		const char* loadTexFilePath,
		std::unique_ptr<char[]>& outTexData,
		unsigned int& outTexSize
		) {
			//FILE* fp = fopen(loadTexFilePath, "rb");
			FILE* fp;
			errno_t er = fopen_s(&fp,loadTexFilePath, "rb");
			if (fp == nullptr) {
				//null�e�N�X�`���̃��[�h�Ɏ��s�B
				MessageBoxA(nullptr, "null�e�N�X�`���̃��[�h�Ɏ��s���܂����B", "�G���[", MB_OK);
				std::abort();
			}
			//�e�N�X�`���T�C�Y���v�Z�B
			fseek(fp, 0L, SEEK_END);
			outTexSize = ftell(fp);
			fseek(fp, 0L, SEEK_SET);

			//���������m��
			outTexData = std::make_unique<char[]>(outTexSize);
			fread(outTexData.get(), outTexSize, 1, fp);
			fclose(fp);
	};

	//�A���x�h�}�b�v�̃t�@�C���p�X �ݒ�
	this->albedo_Map_File_Path_ = "./Assets/Model/Preset/NullAlbedoMap.DDS";
	//�A���x�h�}�b�v�����[�h
	TexLoad(
		this->albedo_Map_File_Path_, 
		this->albedo_Map_, 
		this->albedo_Map_Size_
	);


	//�@���}�b�v�̃t�@�C���p�X �ݒ�
	this->normal_Map_File_Path_ = "./Assets/Model/Preset/NullNormalMap.DDS";
	//�@���}�b�v�����[�h�B
	TexLoad(
		this->normal_Map_File_Path_,
		this->normal_Map_,
		this->normal_Map_Size_
	);

	this->scecular_Map_File_Path_ = "./Assets/Model/Preset/specMap_None.DDS";
	//�X�y�L�����}�b�v�}�b�v�����[�h�B
	TexLoad(
		this->scecular_Map_File_Path_,
		this->scecular_Map_,
		this->scecular_Map_Size_);

	this->zero_Value_Map_File_Path_ = "./Assets/Model/Preset/ZeroValueMap.DDS";
	//�O�̒l���i�[���Ă���}�b�v�����[�h�B
	TexLoad(
		this->zero_Value_Map_File_Path_,
		this->zero_Value_Map_,
		this->zero_Value_Map_Size_);

}

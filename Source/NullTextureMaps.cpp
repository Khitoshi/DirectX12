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
//初期化
void NullTextureMaps::Init()
{
	//テクスチャロード
	auto TexLoad = [&](
		const char* loadTexFilePath,
		std::unique_ptr<char[]>& outTexData,
		unsigned int& outTexSize
		) {
			//FILE* fp = fopen(loadTexFilePath, "rb");
			FILE* fp;
			errno_t er = fopen_s(&fp,loadTexFilePath, "rb");
			if (fp == nullptr) {
				//nullテクスチャのロードに失敗。
				MessageBoxA(nullptr, "nullテクスチャのロードに失敗しました。", "エラー", MB_OK);
				std::abort();
			}
			//テクスチャサイズを計算。
			fseek(fp, 0L, SEEK_END);
			outTexSize = ftell(fp);
			fseek(fp, 0L, SEEK_SET);

			//メモリを確保
			outTexData = std::make_unique<char[]>(outTexSize);
			fread(outTexData.get(), outTexSize, 1, fp);
			fclose(fp);
	};

	//アルベドマップのファイルパス 設定
	this->albedo_Map_File_Path_ = "./Assets/Model/Preset/NullAlbedoMap.DDS";
	//アルベドマップをロード
	TexLoad(
		this->albedo_Map_File_Path_, 
		this->albedo_Map_, 
		this->albedo_Map_Size_
	);


	//法線マップのファイルパス 設定
	this->normal_Map_File_Path_ = "./Assets/Model/Preset/NullNormalMap.DDS";
	//法線マップをロード。
	TexLoad(
		this->normal_Map_File_Path_,
		this->normal_Map_,
		this->normal_Map_Size_
	);

	this->scecular_Map_File_Path_ = "./Assets/Model/Preset/specMap_None.DDS";
	//スペキュラマップマップをロード。
	TexLoad(
		this->scecular_Map_File_Path_,
		this->scecular_Map_,
		this->scecular_Map_Size_);

	this->zero_Value_Map_File_Path_ = "./Assets/Model/Preset/ZeroValueMap.DDS";
	//０の値を格納しているマップをロード。
	TexLoad(
		this->zero_Value_Map_File_Path_,
		this->zero_Value_Map_,
		this->zero_Value_Map_Size_);

}

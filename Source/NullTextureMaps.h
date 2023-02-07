#pragma once
#include <string>
#include <memory>
/// <summary>
/// Nullテクスチャマップを保持しているクラス。
/// </summary>
/// <remarks>
/// このクラスが保持しているのは、
/// スペキュラマップ、法線マップ、反射率マップなどの
/// マテリアルで使用するテクスチャマップです。
/// このクラスのマップはマテリアルに、上記のマップが
/// 貼り付けられていない場合です。
/// ですので、このクラスが保持しているマップは、スペキュラマップであれば
/// 鏡面反射率０、法線マップであれば凸凹なし、リフレクションマップであれば反射率０となります。
/// マテリアルにマップが貼り付けられていない場合に、このクラスのマップを利用することで、
/// シェーダーでの動的分岐を減らすことができ、シェーダ―プログラムをシンプルにすることができるようになっています。
/// このクラスでロードされているテクスチャは、Assets/modelData/presetの下に用意されています。
/// </remarks>
class NullTextureMaps
{
public:
	/// <summary>
	/// デフォルト コンストラクタ
	/// </summary>
	NullTextureMaps();

	/// <summary>
	/// デフォルト デストラクタ
	/// </summary>
	~NullTextureMaps();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

public://get method
#pragma region Get this AlbedoMap Infomation
	
	/// <summary> 
	/// アルベドマップを取得。
	/// </summary>
	/// <returns></returns>
	const std::unique_ptr<char[]>& GetAlbedoMap() const
	{
		return this->albedo_Map_;
	}
	/// <summary>
	/// アルベドマップのサイズを取得。
	/// </summary>
	/// <returns></returns>
	unsigned int GetAlbedoMapSize() const
	{
		return this->albedo_Map_Size_;
	}

	/// <summary>
	/// アルベドマップのファイルパス 取得
	/// </summary>
	/// <returns></returns>
	const char* GetAlbedoMapFilePath() const
	{
		return this->albedo_Map_File_Path_;
	}

#pragma endregion

#pragma region Get this NormalMap Infomation
	/// <summary>
	/// 法線マップ 取得
	/// </summary>
	/// <returns>this normal map</returns>
	const std::unique_ptr<char[]>& GetNormalMap() const
	{
		return this->normal_Map_;
	}

	/// <summary>
	/// 法線マップのサイズ 取得。
	/// </summary>
	/// <returns>this normal map size</returns>
	unsigned int GetNormalMapSize() const
	{
		return this->normal_Map_Size_;
	}

	/// <summary>
	/// 法線マップのファイルパス 取得
	/// </summary>
	/// <returns>this normal map file path</returns>
	const char* GetNormalMapFilePath() const
	{
		return this->normal_Map_File_Path_;
	}

#pragma endregion

#pragma region Get this SpecularMap Infomation
	/// <summary>
	/// スペキュラマップを取得。
	/// </summary>
	/// <returns>this scecular map</returns>
	const std::unique_ptr<char[]>& GetSpecularMap() const
	{
		return this->scecular_Map_;
	}

	/// <summary>
	/// スペキュラマップのサイズを取得。
	/// </summary>
	/// <returns>this scecular map size</returns>
	unsigned int GetSpecularMapSize() const
	{
		return this->scecular_Map_Size_;
	}

	/// <summary>
	/// スペキュラマップのファイルパス 取得
	/// </summary>
	/// <returns>this scecular map map file path </returns>
	const char* GetSpecularMapFilePath() const
	{
		return this->scecular_Map_File_Path_;
	}

#pragma endregion

#pragma region Get this ReflectionMap Infomation
	/// <summary>
	/// リフレクションマップを取得。
	/// </summary>
	/// <returns>this ReflectionMap </returns>
	const std::unique_ptr<char[]>& GetReflectionMap() const
	{
		return this->zero_Value_Map_;
	}

	/// <summary>
	/// リフレクションマップのサイズを取得。
	/// </summary>
	/// <returns>this ReflectionMap size</returns>
	unsigned int GetReflectionMapSize() const
	{
		return this->zero_Value_Map_Size_;
	}

	/// <summary>
	/// リフレクションマップのファイルパス取得
	/// </summary>
	/// <returns>this Get ReflectionMap file path</returns>
	const char* GetReflectionMapFilePath() const
	{
		return this->zero_Value_Map_File_Path_;
	}

#pragma endregion

private:
	//アルベドマップ
	std::unique_ptr<char[]> albedo_Map_;
	//アルベドマップのファイルパス
	const char* albedo_Map_File_Path_;
	//アルベドマップのサイズ
	unsigned int albedo_Map_Size_;

	//アルベドマップ
	std::unique_ptr<char[]> normal_Map_;
	//アルベドマップのファイルパス
	const char* normal_Map_File_Path_;
	//アルベドマップのサイズ
	unsigned int normal_Map_Size_;

	//スペキュラマップ
	std::unique_ptr<char[]> scecular_Map_;
	//スペキュラマップのファイルパス
	const char* scecular_Map_File_Path_;
	//スペキュラマップのサイズ
	unsigned int scecular_Map_Size_;

	//リフレクションマップ
	std::unique_ptr<char[]> zero_Value_Map_;
	//リフレクションマップのファイルパス
	const char* zero_Value_Map_File_Path_;
	//リフレクションマップのサイズ
	unsigned int zero_Value_Map_Size_;
};

#pragma once
#include <string>
#include <functional>

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
class TkmFile
{
public:
	/// <summary>
	/// マテリアル
	/// </summary>
	struct SMaterial {
		std::string albedoMapFileName;			//アルベドマップのファイル名
		std::string normalMapFileName;			//法線マップのファイル名
		std::string specularMapFileName;		//スペキュラマップのファイル名
		std::string reflectionMapFileName;		//リフレクションマップのファイル名
		std::string refractionMapFileName;		//屈折マップのファイル名
		std::unique_ptr<char[]>	albedoMap;		//ロードされたアルベドマップ(ddsファイル)
		unsigned int albedoMapSize;				//アルベドマップのサイズ(ddsファイル)
		std::unique_ptr<char[]>	normalMap;		//ロードされた法線マップ(ddsファイル)
		unsigned int normalMapSize;				//法線マップのサイズ
		std::unique_ptr<char[]>	specularMap;	//ロードされたスペキュラマップ(ddsファイル)
		unsigned int specularMapSize;			//スペキュラマップのサイズ(ddsファイル)
		std::unique_ptr<char[]>	reflectionMap;	//ロードされたリフレクションマップ(ddsファイル)
		unsigned int reflectionMapSize;			//リフレクションマップのサイズ(ddsファイル)
		std::unique_ptr<char[]>	refractionMap;	//ロードされた屈折マップ(ddsファイル)
		unsigned int refractionMapSize;			//屈折マップのサイズ(ddsファイル)
		std::string albedoMapFilePath;			//アルベドマップのファイルパス
		std::string normalMapFilePath;			//法線マップのファイルパス
		std::string specularMapFilePath;		//スペキュラマップのファイルパス
		std::string reflectionMapFilePath;		//リフレクションマップのファイルパス
		std::string refractionMapFilePath;		//屈折マップのファイルパス
	};

	/// <summary>
	/// 頂点。
	/// </summary>
	/// <remarks>
	/// ここを編集したら、レイトレのシェーダーで定義されている頂点構造体も変更する必要がある
	/// </remarks>
	struct SVertex {
		Vector3 pos;			//座標。
		Vector3 normal;			//法線。
		Vector3 tangent;		//接ベクトル。
		Vector3 binormal;		//従ベクトル。
		Vector2 uv;				//UV座標。
		int indices[4];			//スキンインデックス。
		Vector4 skinWeights;	//スキンウェイト。
	};

	/// <summary>
	/// 32ビットのインデックスバッファ
	/// </summary>
	struct SIndexBuffer32 {
		//インデックス
		std::vector<uint32_t> indices;
	};

	/// <summary>
	/// 16ビットのインデックスバッファ
	/// </summary>
	struct SIndexbuffer16 {
		//インデックス
		std::vector<uint16_t> indices;
	};

	/// <summary>
	/// メッシュパーツ
	/// </summary>
	struct SMesh {
		//フラットシェーディング？
		bool isFlatShading;									
		//マテリアルの配列。
		std::vector< SMaterial > materials;					
		//頂点バッファ。
		std::vector< SVertex >	vertexBuffer;				
		//インデックスバッファ(32ビット)の配列。マテリアルの数分だけインデックスバッファはあるよ。
		std::vector<SIndexBuffer32> indexBuffer32Array;		
		//インデックスバッファ(16ビット)の配列。
		std::vector< SIndexbuffer16> indexBuffer16Array;	
	};

public:

	TkmFile() :
		mesh_Parts_()
	{}
	~TkmFile() {};

	/// <summary>
	/// .tkm file をロード
	/// </summary>
	/// <param name="filePath">.tkm file path</param>
	void Load(const char* filePath);

	/// <summary>
	/// メッシュパーツに対してクエリを行う
	/// </summary>
	/// <param name="func">クエリ関数</param>
	void QueryMeshParts(std::function<void(const SMesh& mesh)> func) const
	{
		for (auto& mesh : this->mesh_Parts_) {
			func(mesh);
		}
	}

private:
	/// <summary>
	/// テクスチャ名をロード
	/// </summary>
	/// <param name="fp">ファイルポインタ</param>
	/// <returns>テクスチャ名</returns>
	std::string LoadTextureFileName(FILE* fp);

	/// <summary>
	/// インデックスバッファをロード
	/// </summary>
	template<class T>
	void LoadIndexBuffer(std::vector<T>& indices, int numIndex, FILE* fp);

	/// <summary>
	/// マテリアルを構築
	/// </summary>
	/// <param name="tkmMat">tkmマテリアルの構築先</param>
	/// <param name="fp">ファイルポインタ</param>
	/// <param name="filePath">ロード中のtkmファイルのファイルパス。</param>
	void BuildMaterial(SMaterial& tkmMaterial, FILE* fp, const char* filePath);

	/// <summary>
	/// 接ベクトルと従ベクトルを計算
	/// </summary>
	/// <remarks>
	/// 3dsMaxScriptでやるべきなんだろうけど、デバッグしたいので今はこちらでやる。
	/// </remarks>
	void BuildTangentAndBiNormal();

public://get method
#pragma region Get Method
	/// <summary>
	/// メッシュパーツを取得
	/// </summary>
	/// <returns>メッシュパーツの可変長配列</returns>
	const std::vector< SMesh>& GetMeshParts() const
	{
		return this->mesh_Parts_;
	}

	/// <summary>
	/// メッシュの数を取得
	/// </summary>
	/// <returns>メッシュの数</returns>
	int GetNumMesh() const
	{
		return (int)(this->mesh_Parts_.size());
	}

#pragma endregion

private:
	//メッシュパーツ
	std::vector<SMesh> mesh_Parts_;

};

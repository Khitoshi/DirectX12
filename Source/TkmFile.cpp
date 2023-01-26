#include "TkmFile.h"
#include <Windows.h>

//法線スムージング。
class NormalSmoothing {
private:
	struct SSmoothVertex {
		Vector3 newNormal = Vector3::GetVec3Zero();
		TkmFile::SVertex* vertex = nullptr;
	};
	struct SFace {
		Vector3 normal;
		std::vector<int> vertexNos;
	};
public:
	template <class IndexBuffer>
	void Execute(TkmFile::SMesh& mesh, const IndexBuffer& indexBuffer)
	{

		//面法線を計算していく。
		auto numPolygon = indexBuffer.indices.size() / 3;
		std::vector< SFace> faces;
		faces.reserve(numPolygon);

		for (auto polyNo = 0; polyNo < numPolygon; polyNo++) {
			auto no = polyNo * 3;
			auto vertNo_0 = indexBuffer.indices[no];
			auto vertNo_1 = indexBuffer.indices[no + 1];
			auto vertNo_2 = indexBuffer.indices[no + 2];

			auto& vert_0 = mesh.vertexBuffer[vertNo_0];
			auto& vert_1 = mesh.vertexBuffer[vertNo_1];
			auto& vert_2 = mesh.vertexBuffer[vertNo_2];

			//法線を計算する。
			Vector3 v0tov1 = vert_1.pos - vert_0.pos;
			Vector3 v0tov2 = vert_2.pos - vert_0.pos;
			Vector3 normal = Cross(v0tov1, v0tov2);
			normal.Normalize();
			SFace face;
			face.normal = normal;
			face.vertexNos.push_back(vertNo_0);
			face.vertexNos.push_back(vertNo_1);
			face.vertexNos.push_back(vertNo_2);
			faces.push_back(face);
		}

		//ステップ２　法線の平均化
		for (auto& face : faces) {
			for (auto vertNo : face.vertexNos) {
				auto& vert = mesh.vertexBuffer[vertNo];
				vert.normal += face.normal;
			}
		}
		for (auto& vert : mesh.vertexBuffer) {
			vert.normal.Normalize();
		}
		//ステップ２　座標と向きが同じ頂点の法線を平均化していく。
		if (mesh.isFlatShading == 0)
		{
			//重複している頂点の法線を平均化
			std::vector<SSmoothVertex> smoothVertex;
			smoothVertex.reserve(mesh.vertexBuffer.size());
			for (auto& v : mesh.vertexBuffer) {
				smoothVertex.push_back({ v.normal, &v });
			}
			for (auto& va : smoothVertex) {
				for (auto& vb : smoothVertex) {

					if (va.vertex != vb.vertex
						&& va.vertex->pos.x == vb.vertex->pos.x
						&& va.vertex->pos.y == vb.vertex->pos.y
						&& va.vertex->pos.z == vb.vertex->pos.z
						) {
						//同じ座標。
						if (va.vertex->normal.Dot(vb.vertex->normal) > 0.0f) {
							//同じ向き。
							va.newNormal += vb.vertex->normal;
						}
					}
				}
				va.newNormal.Normalize();
			}
			for (auto& va : smoothVertex) {
				va.vertex->normal = va.newNormal;
			}
		}
	}
};


/// <summary>
/// TKMファイルフォーマット。
/// </summary>
/// <remarks>
/// 処理系によっては1バイトが8bitでないことがあり、
/// int型やshort型が必ずしも、4バイト、2バイトであるとは限らない。
/// そのため、std::uint16_tやstd::uint32_tを利用している。
/// これらは定義されている処理系であれば、サイズは必ず同じである。
/// </remarks>
namespace tkmFileFormat {
	//現在のTKMファイルのバージョン。
	std::uint16_t VERSION = 100;

	/// <summary>
	/// ヘッダーファイル
	/// </summary>
	struct SHeader {
		std::uint8_t	version;		//バージョン。
		std::uint8_t	isFlatShading;	//フラットシェーディング？
		std::uint16_t	numMeshParts;	//メッシュパーツの数。
	};

	/// <summary>
	/// メッシュパーツヘッダー。
	/// </summary>
	struct SMeshePartsHeader {
		std::uint32_t numMaterial;		//マテリアルの数。
		std::uint32_t numVertex;		//頂点数。
		std::uint8_t indexSize;			//インデックスのサイズ。2か4。
		std::uint8_t pad[3];			//パディング。
	};

	/// <summary>
	/// 頂点
	/// </summary>
	struct SVertex {
		//頂点座標
		float pos[3];					
		//法線
		float normal[3];				
		//UV座標
		float uv[2];					
		//スキンウェイト
		float weights[4];				
		//スキンインデックス
		std::int16_t indices[4];		
	};
};

template< class IndexBuffer>
void BuildTangentAndBiNormalImp(TkmFile::SMesh& mesh, const IndexBuffer& indexBuffer)
{
	//頂点スムースは気にしない。
	auto numPolygon = indexBuffer.indices.size() / 3;
	for (auto polyNo = 0; polyNo < numPolygon; polyNo++) {
		auto no = polyNo * 3;
		auto vertNo_0 = indexBuffer.indices[no];
		auto vertNo_1 = indexBuffer.indices[no + 1];
		auto vertNo_2 = indexBuffer.indices[no + 2];

		auto& vert_0 = mesh.vertexBuffer[vertNo_0];
		auto& vert_1 = mesh.vertexBuffer[vertNo_1];
		auto& vert_2 = mesh.vertexBuffer[vertNo_2];

		Vector3 cp0[3];
		cp0[0] = { vert_0.pos.x, vert_0.uv.x, vert_0.uv.y };
		cp0[1] = { vert_0.pos.y, vert_0.uv.x, vert_0.uv.y };
		cp0[2] = { vert_0.pos.z, vert_0.uv.x, vert_0.uv.y };
		/*{
			{ vert_0.pos.x, vert_0.uv.x, vert_0.uv.y},
			{ vert_0.pos.y, vert_0.uv.x, vert_0.uv.y},
			{ vert_0.pos.z, vert_0.uv.x, vert_0.uv.y}
		};
		*/

		Vector3 cp1[3];
		cp1[0] = { vert_1.pos.x, vert_1.uv.x, vert_1.uv.y };
		cp1[1] = { vert_1.pos.y, vert_1.uv.x, vert_1.uv.y };
		cp1[2] = { vert_1.pos.z, vert_1.uv.x, vert_1.uv.y };
		/*
		{
			{ vert_1.pos.x, vert_1.uv.x, vert_1.uv.y},
			{ vert_1.pos.y, vert_1.uv.x, vert_1.uv.y},
			{ vert_1.pos.z, vert_1.uv.x, vert_1.uv.y}
		};
		*/

		Vector3 cp2[3];
		cp2[0] = { vert_2.pos.x, vert_2.uv.x, vert_2.uv.y };
		cp2[1] = { vert_2.pos.y, vert_2.uv.x, vert_2.uv.y };
		cp2[2] = { vert_2.pos.z, vert_2.uv.x, vert_2.uv.y };
		/*
		{
			{ vert_2.pos.x, vert_2.uv.x, vert_2.uv.y},
			{ vert_2.pos.y, vert_2.uv.x, vert_2.uv.y},
			{ vert_2.pos.z, vert_2.uv.x, vert_2.uv.y}
		};
		*/

		// 平面パラメータからUV軸座標算出する。
		Vector3 tangent, binormal;
		for (int i = 0; i < 3; ++i) {
			auto V1 = cp1[i] - cp0[i];
			auto V2 = cp2[i] - cp1[i];
			auto ABC = Cross(V1, V2);

			if (ABC.x == 0.0f) {
				tangent.v[i] = 0.0f;
				binormal.v[i] = 0.0f;
			}
			else {
				tangent.v[i] = -ABC.y / ABC.x;
				binormal.v[i] = -ABC.z / ABC.x;
			}
		}

		tangent.Normalize();
		binormal.Normalize();

		vert_0.tangent += tangent;
		vert_1.tangent += tangent;
		vert_2.tangent += tangent;

		vert_0.binormal += binormal;
		vert_1.binormal += binormal;
		vert_2.binormal += binormal;
	}
	//法線、接ベクトル、従ベクトルを平均化する。
	for (auto& vert : mesh.vertexBuffer) {
		vert.tangent.Normalize();
		vert.binormal.Normalize();
	}

}


//.tkm file ロード
void TkmFile::Load(const char* filePath)
{
    //ファイルを開く
    FILE* fp;
    fopen_s(&fp, filePath, "rb");

    //ファイルチェック
    if (fp == nullptr)
    {
        MessageBoxA(nullptr, ".tkmファイルが開けません。", "エラー", MB_OK);
		std::abort();
    }

    //tkmファイルのヘッダーを読み込み。
    tkmFileFormat::SHeader header;
    fread(&header, sizeof(header), 1, fp);
    if (header.version != tkmFileFormat::VERSION) {
        //tkmファイルのバージョンが違う。
        MessageBoxA(nullptr, ".tkmファイルのバージョンが異なっています。", "エラー", MB_OK);
		std::abort();
    }

	//メッシュ情報をロードしていく
	this->mesh_Parts_.resize(header.numMeshParts);
	for (int meshPartsNo = 0; meshPartsNo < header.numMeshParts; meshPartsNo++) {

		auto& mesh_parts = this->mesh_Parts_[meshPartsNo];
		mesh_parts.isFlatShading = header.isFlatShading;
		tkmFileFormat::SMeshePartsHeader meshPartsHeader;
		fread(&meshPartsHeader, sizeof(meshPartsHeader), 1, fp);
		//マテリアル情報を記録できる領域を確保。
		mesh_parts.materials.resize(meshPartsHeader.numMaterial);
		//マテリアル情報を構築していく。
		for (unsigned int materialNo = 0; materialNo < meshPartsHeader.numMaterial; materialNo++) {
			auto& material = mesh_parts.materials[materialNo];
			BuildMaterial(material, fp, filePath);
		}

		//続いて頂点バッファ
		mesh_parts.vertexBuffer.resize(meshPartsHeader.numVertex);
		for (unsigned int vertNo = 0; vertNo < meshPartsHeader.numVertex; vertNo++) {
			tkmFileFormat::SVertex vertexTmp;
			int err = fread(&vertexTmp, sizeof(vertexTmp), 1, fp);
			auto& vertex = mesh_parts.vertexBuffer[vertNo];
			vertex.pos.Set(vertexTmp.pos[0], vertexTmp.pos[1], vertexTmp.pos[2]);
			//	vertex.normal.Set(vertexTmp.normal[0], vertexTmp.normal[1], vertexTmp.normal[2]);
			vertex.normal = Vector3::GetVec3Zero();
			vertex.tangent = Vector3::GetVec3Zero();
			vertex.binormal = Vector3::GetVec3Zero();
			vertex.uv.Set(vertexTmp.uv[0], vertexTmp.uv[1]);
			vertex.skinWeights.Set(vertexTmp.weights[0], vertexTmp.weights[1], vertexTmp.weights[2], vertexTmp.weights[3]);
			vertex.indices[0] = vertexTmp.indices[0] != -1 ? vertexTmp.indices[0] : 0;
			vertex.indices[1] = vertexTmp.indices[1] != -1 ? vertexTmp.indices[1] : 0;
			vertex.indices[2] = vertexTmp.indices[2] != -1 ? vertexTmp.indices[2] : 0;
			vertex.indices[3] = vertexTmp.indices[3] != -1 ? vertexTmp.indices[3] : 0;
		}

		//続いてインデックスバッファ。
		//インデックスバッファはマテリアルの数分だけ存在する
		if (meshPartsHeader.indexSize == 2) {
			//16bitのインデックスバッファ。
			mesh_parts.indexBuffer16Array.resize(meshPartsHeader.numMaterial);
		}
		else {
			//32bitのインデックスバッファ。
			mesh_parts.indexBuffer32Array.resize(meshPartsHeader.numMaterial);
		}

		for (unsigned int materialNo = 0; materialNo < meshPartsHeader.numMaterial; materialNo++) {
			//ポリゴン数をロード。
			int numPolygon;
			fread(&numPolygon, sizeof(numPolygon), 1, fp);
			//トポロジーはトライアングルリストオンリーなので、3を乗算するとインデックスの数になる。
			int numIndex = numPolygon * 3;
			if (meshPartsHeader.indexSize == 2) {
				LoadIndexBuffer(
					mesh_parts.indexBuffer16Array[materialNo].indices,
					numIndex,
					fp
				);
			}
			else {
				LoadIndexBuffer(
					mesh_parts.indexBuffer32Array[materialNo].indices,
					numIndex,
					fp
				);
			}
		}
	}
	//接ベクトルと従ベクトルを構築する。
	BuildTangentAndBiNormal();

	//ファイルを閉じる
	fclose(fp);
}

//テクスチャ名をロード。
std::string TkmFile::LoadTextureFileName(FILE* fp)
{
	//テクスチャ名
	std::string texture_name;
	//ファイル名 の長さ
	std::uint32_t file_name_length;
	//ファイル名の長さの取得
	fread(&file_name_length, sizeof(file_name_length), 1, fp);

	//ファイルの確認
	//0 の場合 ファイルが無い
	if (file_name_length <= 0)
	{
		return texture_name;
		/*//テクスチャのロードに失敗*/
		//MessageBoxA(nullptr, "テクスチャのロードに失敗", "エラー", MB_OK);
		//std::abort();
	}

	//ファイルネーム生成
	char* local_file_name = reinterpret_cast<char*>(malloc(file_name_length + 1));
	//null分も読み込むので + 1
	fread(local_file_name, file_name_length + 1, 1, fp);
	texture_name = local_file_name;
	//解放
	free(local_file_name);
	
	return texture_name;
}

//インデックスバッファをロード
template<class T>
void TkmFile::LoadIndexBuffer(std::vector<T>& indices, int numIndex, FILE* fp)
{
	indices.resize(numIndex);
	for (int indexNo = 0; indexNo < numIndex; indexNo++)
	{
		T index;
		fread(&index, sizeof(index), 1, fp);
		indices[indexNo] = index - 1;
	}
}

//マテリアルを構築
void TkmFile::BuildMaterial(SMaterial& tkmMaterial, FILE* fp, const char* filePath)
{
	//各ファイル名をロード
	//アルベドマップ
	tkmMaterial.albedoMapFileName = LoadTextureFileName(fp);
	//法線マップ
	tkmMaterial.normalMapFileName = LoadTextureFileName(fp);
	//スペキュラマップ
	tkmMaterial.specularMapFileName = LoadTextureFileName(fp);
	//リフレクションマップのファイル名をロード。
	tkmMaterial.reflectionMapFileName = LoadTextureFileName(fp);
	//屈折マップ
	tkmMaterial.refractionMapFileName = LoadTextureFileName(fp);

	//テクスチャロード
	std::string tex_file_path = filePath;
	auto loadTexture = [&](
		std::string& texFileName,
		std::unique_ptr<char[]>& ddsFileMemory,
		unsigned int& fileSize,
		std::string& texFilePathDst
		)
	{
		//ファイル名の長さ
		int file_path_length = static_cast<int>(tex_file_path.length());

		//ファイルの存在確認
		if (texFileName.length() <= 0) { return; }

		//モデルのファイルパスからラストのフォルダ区切りを探す
		auto replaseStartPos = tex_file_path.find_last_of('/');
		if (replaseStartPos == std::string::npos) {
			replaseStartPos = tex_file_path.find_last_of('\\');
		}
		replaseStartPos += 1;
		auto replaceLen = file_path_length - replaseStartPos;
		tex_file_path.replace(replaseStartPos, replaceLen, texFileName);
		//拡張子をddsに変更する。
		replaseStartPos = tex_file_path.find_last_of('.') + 1;
		replaceLen = tex_file_path.length() - replaseStartPos;
		tex_file_path.replace(replaseStartPos, replaceLen, "dds");
		//テクスチャファイルパスを記憶しておく。
		texFilePathDst = tex_file_path;

		//テクスチャをロード
		FILE* tex_file_fp;
		fopen_s(&tex_file_fp, tex_file_path.c_str(), "rb");
		if (tex_file_fp == nullptr)
		{
			//テクスチャのロードに失敗
			MessageBoxA(nullptr, "テクスチャロードに失敗", "エラー", MB_OK);
			std::abort();
		}

		//ファイルサイズを取得。
		fseek(tex_file_fp, 0L, SEEK_END);
		fileSize = ftell(tex_file_fp);
		fseek(tex_file_fp, 0L, SEEK_SET);

		ddsFileMemory = std::make_unique<char[]>(fileSize);
		fread(ddsFileMemory.get(), fileSize, 1, tex_file_fp);
		fclose(tex_file_fp);
	};

	//各テクスチャをロード
	//アルベドマップ　ロード
	loadTexture(
		tkmMaterial.albedoMapFileName,
		tkmMaterial.albedoMap,
		tkmMaterial.albedoMapSize,
		tkmMaterial.albedoMapFilePath
	);

	//法線マップ　ロード
	loadTexture(
		tkmMaterial.normalMapFileName,
		tkmMaterial.normalMap,
		tkmMaterial.normalMapSize,
		tkmMaterial.normalMapFilePath
	);

	//スペキュラマップ ロード
	loadTexture(
		tkmMaterial.specularMapFileName,
		tkmMaterial.specularMap,
		tkmMaterial.specularMapSize,
		tkmMaterial.specularMapFilePath
	);

	//リフレクションマップ　ロード
	loadTexture(
		tkmMaterial.reflectionMapFileName,
		tkmMaterial.reflectionMap,
		tkmMaterial.reflectionMapSize,
		tkmMaterial.reflectionMapFilePath
	);

	//屈折マップ　ロード
	loadTexture(
		tkmMaterial.refractionMapFileName,
		tkmMaterial.refractionMap,
		tkmMaterial.refractionMapSize,
		tkmMaterial.refractionMapFilePath
	);
}

//接ベクトルと従ベクトルを計算
void TkmFile::BuildTangentAndBiNormal()
{
	NormalSmoothing normalSmoothing;
	for (auto& mesh : this->mesh_Parts_) {
		for (auto& indexBuffer : mesh.indexBuffer16Array) {
			normalSmoothing.Execute(mesh, indexBuffer);
			BuildTangentAndBiNormalImp(mesh, indexBuffer);
		}
		for (auto& indexBuffer : mesh.indexBuffer32Array) {
			normalSmoothing.Execute(mesh, indexBuffer);
			BuildTangentAndBiNormalImp(mesh, indexBuffer);
		}
	}
}

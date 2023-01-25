#pragma once
//#include <fbxsdk.h>
#include "TksFile.h"
#include "Skeleton.h"
#include "TkmFile.h"
#include "MeshParts.h"
#include "ModelData.h"

class GraphicsEngine;

class Model
{
public:
	Model();
	~Model();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="tk">DirectXtk関係を保有</param>
	/// <param name="graphicsEngine">デバイスを格納している</param>
	/// <param name="initData">初期化データ</param>
	void Init(tkEngine* tk,GraphicsEngine* graphicsEngine,const ModelInitData& initData);

	/// <summary>
	/// ワールド行列を計算して、メンバ変数のworld行列を更新する
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="rot">回転</param>
	/// <param name="scale">拡大率</param>
	void UpdateWorldMatrix(Vector3 pos, Quaternion rot, Vector3 scale);

	/// <summary>
	/// 描画(カメラ指定版)
	/// </summary>
	/// <param name="renderContext">レンダリングコンテキスト</param>
	/// <param name="camera">カメラ</param>
	void Draw(GraphicsEngine* graphicsEngine, RenderContext& rc, Camera& camera);

	/// <summary>
	/// 描画(カメラ行列指定版)
	/// </summary>
	/// <param name="renderContext">レンダリングコンテキスト</param>
	/// <param name="viewMatrix">ビュー行列</param>
	/// <param name="projMatrix">プロジェクション行列</param>
	void Draw(GraphicsEngine* graphicsEngine, RenderContext& rc, const Matrix& viewMatrix, const Matrix& projMatrix);

	/// <summary>
	/// インスタンシング描画
	/// </summary>
	/// <param name="renderContext">レンダリングコンテキスト</param>
	/// <param name="numInstance">インスタンスの数</param>
	void DrawInstancing(GraphicsEngine* graphicsEngine, RenderContext& renderContext, int numInstance, Camera* camera);

	/// <summary>
	/// メッシュに対して問い合わせを行う。
	/// </summary>
	/// <param name="queryFunc">問い合わせ関数</param>
	void QueryMeshs(std::function<void(const SMesh& mesh)> queryFunc)
	{
		this->mesh_parts_.QueryMeshs(queryFunc);
	}
	void QueryMeshAndDescriptorHeap(std::function<void(const SMesh& mesh, const DescriptorHeap& ds)> queryFunc)
	{
		this->mesh_parts_.QueryMeshAndDescriptorHeap(queryFunc);
	}


	/// <summary>
	/// ワールド行列を計算する
	/// </summary>
	/// <remark>
	/// この関数はUpdateWorldMatrix関数の中から使われています
	/// Modelクラスの使用に沿ったワールド行列の計算を行いたい場合
	/// 本関数を利用すると計算することができます
	/// </remark>
	/// <param name="pos">座標</param>
	/// <param name="rot">回転</param>
	/// <param name="scale">拡大率</param>
	/// <returns></returns>
	Matrix CalcWorldMatrix(Vector3 pos, Quaternion rot, Vector3 scale)
	{
		Matrix world;
		Matrix bias;
		if (this->model_up_axis_ == enModelUpAxisZ) {
			//Z-up
			bias.MakeRotationX(Math::PI() * -0.5f);
		}
		Matrix mTrans, mRot, mScale;
		mTrans.MakeTranslation(pos);
		mRot.MakeRotationFromQuaternion(rot);
		mScale.MakeScaling(scale);

		world = bias * mScale * mRot * mTrans;
		return world;
	}

public:
#pragma region Get Mehod

	/// <summary>
	/// ワールド行列を取得
	/// </summary>
	/// <returns></returns>
	const Matrix& GetWorldMatrix() const
	{
		return this->world_;
	}

	/// <summary>
	/// TKMファイルを取得
	/// </summary>
	/// <returns></returns>
	const TkmFile& GetTkmFile() const
	{
		return *this->tkm_file_;
	}

	/// <summary>
	/// 初期化確認フラグ取得 
	/// true = 初期化済み
	/// </summary>
	/// <returns></returns>
	bool IsInited()const 
	{ 
		return this->is_Inited_;
	}

#pragma endregion

#pragma region set & change
	/// <summary>
	/// アルベドマップを変更
	/// </summary>
	/// <remarks>
	/// この関数を呼び出すとディスクリプタヒープの再構築が行われるため、処理負荷がかかります
	/// 毎フレーム呼び出す必要がない場合は呼び出さないようにしてください
	/// </remarks>
	/// <param name="materialName">変更しいたマテリアルの名前</param>
	/// <param name="albedoMap">アルベドマップ</param>
	void ChangeAlbedoMap(const char* materialName, Texture& albedoMap);

#pragma endregion

private:
	//初期化されている
	bool is_Inited_;

	//ワールド行列
	Matrix world_;

	//.tkm file
	TkmFile* tkm_file_;

	//スケルトン
	Skeleton skeleton_;

	//メッシュパーツ
	MeshParts mesh_parts_;

	//モデルの上方向(blenderでは上方向が異なる)
	EnModelUpAxis model_up_axis_;
};

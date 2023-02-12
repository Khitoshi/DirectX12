#pragma once

#include <memory>
#include <vector>

#include "Skeleton.h"
#include "TksFile.h"
#include "Bone.h"
#include "Matrix.h"

class Skeleton
{
public:
	/// <summary>
	/// デフォルト コンストラクタ
	/// </summary>
	Skeleton();

	/// <summary>
	/// デフォルト デストラクタ
	/// </summary>
	~Skeleton();

	/// <summary>
	/// 
	/// </s初期化ummary>
	/// <param name="tksFilePath">.tks file path</param>
	void Init(const char* tksFilePath);

	/// <summary>
	/// ボーン行列の構築。
	/// </summary>
	void BuildBoneMatrices();

	/// <summary>
	/// ボーンの名前から番号を検索
	/// </summary>
	/// <param name="boneName">bone name</param>
	/// <returns>ボーンの番号，見つからなかった場合は-1</returns>
	int FindBoneID(const wchar_t* boneName)const;

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="matrix_World"></param>
	void Update(const Matrix& matrix_World);

	/// <summary>
	/// ボーンのワールド行列の更新関数
	/// </summary>
	/// <param name="bone">更新するボーン</param>
	/// <param name="parentMatrix">親ボーンのワールド行列</param>
	static 	void UpdateBoneWorldMatrix(Bone& bone, const Matrix& parentMatrix);

private:
	/// <summary>
	/// アニメーション更新処理
	/// </summary>
	/// <param name="matrix_world">マトリックスワールド行列</param>
	void AnimationUpdate(const Matrix& matrix_world);

public:
#pragma region Set method
	/// <summary>
	/// ボーンをローカル行列に設定
	/// </summary>
	/// <param name="boneNo">ボーン番号</param>
	/// <param name="matrix">行列</param>
	//void SetBoneLocalMatrix(int boneNumber, const Matrix& matrix);
	
	/// <summary>
	/// アニメーションが再生されているフラグを付ける
	/// </summary>
	void SetMarkPlayAnimation() { this->is_Play_Animation_ = true; }

#pragma endregion

#pragma region Get method
	/// <summary>
	/// ボーンの数を取得
	/// </summary>
	/// <returns>this bone number </returns>
	int GetNumBones()const { return static_cast<int>(this->bones_.size()); }

	/// <summary>
	/// 初期化済み 判定 取得
	/// </summary>
	/// <returns></returns>
	bool IsInited()const { this->is_Inited_; };

	/// <summary>
	/// ボーンを取得
	/// </summary>
	/// <param name="boneNumber">ボーンの番号</param>
	/// <returns></returns>
	Bone* GetBone(const int boneNumber)const { return this->bones_[boneNumber].get(); }

	/// <summary>
	/// ボーン行列の先頭アドレス取得
	/// </summary>
	/// <returns></returns>
	Matrix* GetBoneMatricesTopAddress()const { return this->bone_Matrixs_.get(); };

#pragma endregion

private:
	//ボーンの最大数
	enum { BONE_MAX = 512 };
private:
	//.tksファイル
	TksFile tks_file_;
	
	//ボーンの配列
	std::vector<std::unique_ptr<Bone>> bones_;

	//ボーン行列
	std::unique_ptr<Matrix[]> bone_Matrixs_;

	//初期化判断用フラグ
	//false = 初期化していない
	bool is_Inited_;

	//アニメーション流し込み判断用
	//false = アニメーションが流し込まれていない
	bool is_Play_Animation_;
};

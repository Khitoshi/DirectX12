#pragma once

#include "Quaternion.h"
#include "Matrix.h"
#include <string>
#include <list>
/// <summary>
/// ボーン。
/// </summary>
class Bone {
public:
	/// <summary>
	/// コンストラクタ。
	/// </summary>
	/// <param name="boneName">ボーンの名前</param>
	/// <param name="bindPose">バインドポーズの行列</param>
	/// <param name="invBindPose">バインドポーズの逆行列</param>
	/// <param name="parentBoneNo">親のボーン番号</param>
	/// <param name="boneId">ボーン番号</param>
	Bone(
		const wchar_t* boneName,
		const Matrix& bindPose,
		const Matrix& invBindPose,
		int parentBoneNo,
		int boneId
	);

	/// <summary>
	/// デストラクタ。
	/// </summary>
	~Bone();

	/// <summary>
	/// 子供 追加
	/// </summary>
	/// <param name="bone"></param>
	void AddChild(Bone* bone) { this->bone_children_.push_back(bone); }
	
	/// <summary>
	/// このボーンのワールド空間での位置と回転とスケールを計算する
	/// </summary>
	/// <param name="trans">平行移動量の格納先</param>
	/// <param name="rot">回転量の格納先</param>
	/// <param name="scale">拡大率の格納先</param>
	void CalcWorldTRS(Vector3& trans, Quaternion& rot, Vector3& scale);

public:
#pragma region get method
	/// <summary>
	/// ローカル行列(親の座標系での行列)を取得。
	/// </summary>
	const Matrix& GetLocalMatrix() const{return this->local_Matrix_;}

	/// <summary>
	/// ワールド行列を取得。
	/// </summary>
	const Matrix& GetWorldMatrix() const{return this->world_Matrix_;}

	/// <summary>
	/// バインドポーズの行列を取得。
	/// </summary>
	const Matrix& GetBindPoseMatrix() const{return this->bind_Pose_;}

	/// <summary>
	/// バインドポーズの逆行列を取得。
	/// </summary>
	const Matrix& GetInvBindPoseMatrix() const{return this->inv_Bind_Pose_;}

	/// <summary>
	/// 親のボーン番号を取得。
	/// </summary>
	/// <returns></returns>
	int GetParentBoneNumber() const{return this->parent_Bone_Number_;}

	/// <summary>
	/// ボーン番号を取得。
	/// </summary>
	/// <returns></returns>
	int GetId() const{return this->bone_Id_;}

	/// <summary>
	/// 子供取得
	/// </summary>
	/// <returns>this bone children</returns>
	std::list<Bone*>& GetChildren(){return this->bone_children_;}
	const Matrix& GetOffsetLocalMatrix() const{return this->offset_Local_Matrix_;}

	//ボーンの名前 取得
	const wchar_t* GetName() const { return this->bone_Name_.c_str(); }

#pragma endregion

public:
#pragma region set method
	/// <summary>
	/// ローカル行列(親の座標系での行列)を設定。
	/// </summary>
	void SetLocalMatrix(const Matrix& m){this->local_Matrix_ = m;}

	/// <summary>
	/// ワールド行列を設定。
	/// </summary>
	void SetWorldMatrix(const Matrix& m){this->world_Matrix_ = m;}

#pragma endregion

private:
	//ボーンの名前
	std::wstring	bone_Name_;
	//親のボーン番号
	int				parent_Bone_Number_;
	//ボーン番号
	int				bone_Id_;

	//バインドポーズ
	Matrix			bind_Pose_;
	//バインドポーズの逆行列
	Matrix			inv_Bind_Pose_;
	//ローカル行列
	Matrix			local_Matrix_;
	//ワールド行列
	Matrix			world_Matrix_;
	//このボーンのワールド空間での位置。最後にCalcWorldTRSを実行したときの結果が格納されている
	Matrix			offset_Local_Matrix_;

	//位置
	Vector3			positoin_;
	//このボーンの拡大率。最後にCalcWorldTRSを実行したときの結果が格納されている
	Vector3			scale_;
	//このボーンの回転。最後にCalcWorldTRSを実行したときの結果が格納されている
	Quaternion		rotation_;

	//子供のリスト。
	std::list<Bone*>	bone_children_;
};
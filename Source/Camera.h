#pragma once
#include <DirectXMath.h>
#include "Vector3.h"

using namespace DirectX;

class Quaternion;

class Camera
{
public:
	/// <summary>
	/// デフォルト コンストラクタ
	/// </summary>
	Camera();

	/// <summary>
	/// デフォルト デストラクタ
	/// </summary>
	~Camera();

	/// <summary>
	/// ビュー行列、プロジェクション行列を更新する。
	/// </summary>
	void Update();

	/// <summary>
	/// 注視点を原点としてカメラを回転させる。
	/// </summary>
	/// <param name="qRot">回転させるクォータニオン</param>
	void RotateOriginTarget(const Quaternion& qRot);

public:

public:
	/// <summary>
	/// 位置を取得
	/// </summary>
	/// <returns>Vector3 position</returns>
	Vector3& GetPosition() { return position_; }

	/// <summary>
	/// ターゲットを取得	
	/// </summary>
	/// <returns>Vector target</returns>
	Vector3& GetTarget() { return target_; }

private:
	//カメラ位置
	Vector3 position_;
	//カメラ中止点
	Vector3 target_;
};

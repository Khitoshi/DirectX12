#pragma once
#include <DirectXMath.h>
#include "Vector3.h"
using namespace DirectX;

class Camera
{
public:
	Camera();
	~Camera();
public:
	/// <summary>
	/// 位置を設定
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="z"></param>
	void SetPosition(const float x, const float y, const float z) { position_.Set(x, y, z); }

	/// <summary>
	/// ターゲットを設定	
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="z"></param>
	void SetTarget(const float x, const float y, const float z) { this->target_.Set(x, y, z); }

public:
	/// <summary>
	/// 位置を取得
	/// </summary>
	/// <returns>Vector3 position</returns>
	Vector3 GetPosition()const { return position_; }

	/// <summary>
	/// ターゲットを取得	
	/// </summary>
	/// <returns>Vector target</returns>
	Vector3 GetTarget()const { return target_; }

private:
	//カメラ位置
	Vector3 position_;
	//カメラ中止点
	Vector3 target_;
};

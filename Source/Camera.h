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

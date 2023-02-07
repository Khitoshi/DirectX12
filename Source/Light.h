#pragma once

#include <DirectXMath.h>
#include "Vector3.h"
using namespace DirectX;

class Light
{
private:
	struct DirectionLight
	{
		//方向
		Vector3 direction;
		float pad0;
		//色
		Vector3 color;
		float pad1;
	};

public:
	Light();
	~Light();

public:
public:
	/// <summary>
	/// 視点の位置を取得
	/// </summary>
	/// <returns>Vector3 eye_position</returns>
	Vector3& GetEyePosition() { return this->eye_Position_; }

	/// <summary>
	/// 構造体DirectionLightを取得
	/// </summary>
	/// <returns>DirectionLight direction</returns>
	DirectionLight& GetDirectionLight() { return this->direction_Light_; }

//private:
	//視点の位置
	Vector3 eye_Position_;
	//ディレクションライト
	DirectionLight direction_Light_;
};
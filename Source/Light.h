#pragma once

#include <DirectXMath.h>
#include "Vector3.h"
using namespace DirectX;

/// <summary>
/// ディレクションライト構造体
/// </summary>
struct DirectionLight
{
	Vector3 direction;  //ライトの方向
	float pad0;
	Vector3 color;      //ライトのカラー
	float pad1;
};

/// <summary>
/// ライト構造体
/// </summary>
struct Light
{
	DirectionLight directionLight;  // ディレクションライト
	Vector3 eyePos;                 // 視点の位置
};

/*
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
*/
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
		//色
		Vector3 color;
	};

public:
	Light();
	~Light();

public:
	/// <summary>
	/// 視点の位置を設定	
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="z"></param>
	void SetEyePosition(const float x, const float y, const float z) { this->eye_Position_.Set(x, y, z); }

	/// <summary>
	/// 構造体DirectionLightを設定
	/// </summary>
	/// <param name="direction"></param>
	void SetDirectionLight(const DirectionLight direction) { this->direction_Light_ = direction; }

	/// <summary>
	/// 構造体DirectionLightの方向を設定
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="z"></param>
	void SetDirectionLightDirection(const float x, const float y, const float z) { this->direction_Light_.direction.Set(x, y, z); }
	
	/// <summary>
	/// 構造体DirectionLightの色を設定
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="z"></param>
	void SetDirectionLightColor(const float x, const float y, const float z) { this->direction_Light_.color.Set(x, y, z); }

public:
	/// <summary>
	/// 視点の位置を取得
	/// </summary>
	/// <returns>Vector3 eye_position</returns>
	const Vector3 GetEyePosition()const { return this->eye_Position_; }

	/// <summary>
	/// 構造体DirectionLightを取得
	/// </summary>
	/// <returns>DirectionLight direction</returns>
	const DirectionLight GetDirectionLight()const { return this->direction_Light_; }

	/// <summary>
	/// 構造体DirectionLightの方向を取得
	/// </summary>
	/// <returns>Direction Direction.direction </returns>
	const Vector3 GetDirectionLightDirection()const { return this->direction_Light_.direction; }

	/// <summary>
	/// 構造体DirectionLightの色を取得
	/// </summary>
	/// <returns>Vector3 Direction.color</returns>
	const Vector3 GetDirectionLightColor()const { return this->direction_Light_.direction; }

//private:
	//視点の位置
	Vector3 eye_Position_;
	//ディレクションライト
	DirectionLight direction_Light_;
};
#pragma once
#include <DirectXMath.h>
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix.h"
#include "GraphicsEngine.h"
using namespace DirectX;

class Quaternion;
class Camera
{
private:
	/// <summary>
	/// 射影行列の更新方法。
	/// </summary>
	enum class EnUpdateProjMatrixFunc {
		enUpdateProjMatrixFunc_Perspective,		//透視射影行列。遠近法が効いた絵を作りたいならこっち。
		enUpdateProjMatrixFunc_Ortho,			//平行投影。２Ｄ的な表現がしたいならこっち。
	};

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
	/// ビュー行列、プロジェクション行列を更新する
	/// </summary>
	void Update(const GraphicsEngine* graphicsEngine);

	/// <summary>
	/// 注視点を原点としてカメラを回転させる
	/// </summary>
	/// <param name="qRot">回転させるクォータニオン</param>
	void RotateOriginTarget(const Quaternion& qRot);

#pragma region Move Method
	/// <summary>
	/// カメラを動かす
	/// </summary>
	/// <param name="move">動かす量</param>
	void Move(const Vector3& move)
	{
		this->position_ += move;
		this->target_ += move;
		this->is_dirty_ = true;
	}

	/// <summary>
	/// 注視点を動かす
	/// </summary>
	/// <param name="move">移動量</param>
	void MoveTarget(const Vector3& move)
	{
		this->target_ += move;
		this->is_dirty_ = true;
	}

	/// <summary>
	/// 視点を動かす
	/// </summary>
	/// <param name="move"></param>
	void MovePosition(const Vector3& move)
	{
		this->position_ += move;
		this->is_dirty_ = true;
	}

	/// <summary>
	/// カメラの前方方向に移動
	/// </summary>
	/// <param name="moveForward"></param>
	void MoveForward(float moveForward)
	{
		Move(this->forward_ * moveForward);
	}

	/// <summary>
	/// カメラの右方向に移動
	/// </summary>
	/// <param name="moveRight"></param>
	void MoveRight(float moveRight)
	{
		Move(this->right_ * moveRight);
	}

	/// <summary>
	/// カメラの上方向に移動
	/// </summary>
	/// <param name="moveUp"></param>
	void MoveUp(float moveUp)
	{
		Move(this->up_ * moveUp);
	}

#pragma endregion


	/// <summary>
	/// カメラのコピーを作成
	/// </summary>
	/// <param name="dst"></param>
	void CopyTo(Camera& dst)
	{
		memcpy(&dst, this, sizeof(dst));
		this->is_dirty_ = true;
	}

	/// <summary>
	/// ワールド座標からスクリーン座標を計算する
	/// </summary>
	/// <remarks>
	/// 計算されるスクリーン座標は画面の中心を{0,0}、左上を{画面の幅*-0.5,画面の高さ*-0.5}
	/// 右下を{ 画面の幅 * 0.5,画面の高さ * 0.5 }とする座標系です
	/// </remarks>
	/// <param name="screenPos">スクリーン座標の格納先</param>
	/// <param name="worldPos">ワールド座標</param>
	void CalcScreenPositionFromWorldPosition(GraphicsEngine*& graphicsEngine, Vector2& screenPos, const Vector3& worldPos) const;

public:
#pragma region Get Method
	/// <summary>
	/// 位置 取得
	/// </summary>
	/// <returns>Vector3 position</returns>
	Vector3& GetPosition() { return position_; }

	/// <summary>
	/// ターゲット 取得	
	/// </summary>
	/// <returns>Vector target</returns>
	Vector3& GetTarget() { return target_; }

	/// <summary>
	/// 注視点 取得
	/// </summary>
	const Vector3& GetTarget() const
	{
		return this->target_;
	}

	/// <summary>
	/// カメラの上方向 取得
	/// </summary>
	const Vector3& GetUp() const
	{
		return this->up_;
	}

	/// <summary>
	/// ビュー行列の逆行列 取得
	/// </summary>
	const Matrix& GetViewMatrixInv(GraphicsEngine*& graphicEngine)
	{
		if (this->is_dirty_) {
			//更新する必要がある。
			Update(graphicEngine);
		}
		return this->view_matrix_inv;
	}

	/// <summary>
	/// ビュー行列 取得
	/// </summary>
	const Matrix& GetViewMatrix(GraphicsEngine*& graphicEngine)
	{
		if (this->is_dirty_) {
			//更新する必要がある。
			Update(graphicEngine);
		}
		return this->view_matrix_;
	}

	/// <summary>
	/// プロジェクション行列 取得
	/// </summary>
	const Matrix& GetProjectionMatrix(GraphicsEngine*& graphicEngine)
	{
		if (this->is_dirty_) {
			//更新する必要がある。
			Update(graphicEngine);
		}
		return this->projection_matrix_;
	}

	/// <summary>
	/// ビュー×プロジェクション行列 取得
	/// </summary>
	const Matrix& GetViewProjectionMatrix(GraphicsEngine*& graphicEngine)
	{
		if (this->is_dirty_) {
			//更新する必要がある。
			Update(graphicEngine);
		}
		return this->view_projection_matrix;
	}

	/// <summary>
	/// カメラの回転行列 取得
	/// </summary>
	const Matrix& GetCameraRotation(GraphicsEngine*& graphicEngine)
	{
		if (this->is_dirty_) {
			//更新する必要がある
			Update(graphicEngine);
		}
		return this->camera_rotation_;
	}

	/// <summary>
	/// 遠平面までの距離 取得
	/// </summary>
	float GetFar() const
	{
		return this->far_;
	}

	/// <summary>
	/// 近平面までの距離 取得
	/// </summary>
	float GetNear() const
	{
		return this->near_;
	}

	/// <summary>
	/// 平行投影の幅 取得
	/// </summary>
	float GetWidth() const
	{
		return this->width_;
	}

	/// <summary>
	/// 平行投影の高さ 取得
	/// </summary>
	float GetHeight() const
	{
		return this->height_;
	}

	/// <summary>
	/// 画角を取得
	/// </summary>
	/// <returns>画角 (単位:ラジアン)</returns>
	float GetViewAngle() const
	{
		return this->view_angle_;
	}

	/// <summary>
	/// 注視点と視点の距離 取得
	/// </summary>
	/// <returns></returns>
	float GetTargetToPositionLength() const
	{
		return this->target_to_position_len_;
	}

	/// <summary>
	/// カメラの前方向 取得
	/// </summary>
	const Vector3& GetForward() const
	{
		return this->forward_;
	}

	/// <summary>
	/// カメラの右方向 取得
	/// </summary>
	const Vector3& GetRight() const
	{
		return this->right_;
	}

	/// <summary>
	/// アスペクト比 取得
	/// </summary>
	float GetAspect() const
	{
		return this->aspect_;
	}

#pragma endregion

#pragma region Set Method
	/// <summary>
	/// カメラの座標 設定
	/// </summary>
	void SetPosition(const Vector3& pos)
	{
		this->position_ = pos;
		this->is_dirty_ = true;
	}
	void SetPosition(float x, float y, float z)
	{
		SetPosition(Vector3(x, y, z));
	}

	/// <summary>
	/// 注視点 設定
	/// </summary>
	void SetTarget(float x, float y, float z)
	{
		SetTarget(Vector3(x, y, z));
	}
	void SetTarget(const Vector3& target)
	{
		this->target_ = target;
		this->is_dirty_ = true;
	}

	/// <summary>
	/// カメラの上方向 設定
	/// </summary>
	void SetUp(const Vector3& up)
	{
		this->up_ = up;
		this->up_.Normalize();
	}
	void SetUp(float x, float y, float z)
	{
		SetUp(Vector3(x, y, z));
	}

	/// <summary>
	/// 遠平面までの距離 設定
	/// </summary>
	void SetFar(float fFar)
	{
		this->far_ = fFar;
		this->is_dirty_ = true;
	}

	/// <summary>
	/// 近平面までの距離 設定
	/// </summary>
	void SetNear(float fNear)
	{
		this->near_ = fNear;
		this->is_dirty_ = true;
	}

	/// <summary>
	/// 平行投影の幅 設定
	/// </summary>
	/// <remarks>
	/// SetUpdateProjMatrixFuncでenUpdateProjMatrixFunc_Orthoが設定されているときに使用される
	/// </remarks>
	void SetWidth(float w)
	{
		this->width_ = w;
		this->is_dirty_ = true;
	}

	/// <summary>
	/// 平行投影の高さ 設定
	/// </summary>
	/// <remarks>
	/// SetUpdateProjMatrixFuncでenUpdateProjMatrixFunc_Orthoが設定されているときに使用される
	/// </remarks>
	void SetHeight(float h)
	{
		this->height_ = h;
		this->is_dirty_ = true;
	}

	/// <summary>
	/// 平行投影の高さを設定
	/// </summary>
	/// <remarks>
	/// SetUpdateProjMatrixFuncでenUpdateProjMatrixFunc_Orthoが設定されているときに使用される
	/// </remarks>
	void SetUpdateProjMatrixFunc(EnUpdateProjMatrixFunc func)
	{
		this->update_Projection_matrix_func = func;
		this->is_dirty_ = true;
	}

	/// <summary>
	/// 画角を設定
	/// </summary>
	/// <remarks>
	/// SetUpdateProjMatrixFuncでenUpdateProjMatrixFunc_Perspectiveが設定されているときに使用される
	/// </remarks>
	/// <param name="viewAngle">画角。単位ラジアン</param>
	void SetViewAngle(float viewAngle)
	{
		this->view_angle_ = viewAngle;
		this->is_dirty_ = true;
	}

#pragma endregion

private:
	//カメラ位置。
	Vector3		position_;
	//カメラの上方向。
	Vector3		up_;
	//カメラの中止点。
	Vector3		target_;
	//カメラの前方。
	Vector3		forward_;
	//カメラの右。
	Vector3		right_ ;

	//ビュー行列。
	Matrix		view_matrix_;
	//プロジェクション行列。
	Matrix		projection_matrix_;
	//ビュープロジェクション行列。
	Matrix		view_projection_matrix;
	//ビュー行列の逆行列。
	Matrix		view_matrix_inv;
	//カメラの回転行列。
	Matrix		camera_rotation_;

	//注視点と視点まで距離。
	float		target_to_position_len_;
	//近平面。
	float		near_;
	//遠平面。
	float		far_;
	//画角(ラジアン)。
	float		view_angle_;
	//アスペクト比。
	float		aspect_;
	//平行投影行列を作成するときに使用される幅。
	float		width_ ;
	//平行投影行列を作成するときに使用される高さ。
	float		height_;

	//プロジェクション行列の更新の仕方。
	EnUpdateProjMatrixFunc update_Projection_matrix_func;

	bool		is_Need_Update_Projection_matrix;

	//ダーティフラグ。
	bool		is_dirty_;
};

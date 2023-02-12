#include "Camera.h"

Camera::Camera():
    position_(Vector3(0.0f, 0.0f, 1.0f)),
    up_(Vector3::GetVec3Up()),
    target_(),
    forward_(Vector3::GetVec3Front()),
    right_(Vector3::GetVec3Right()),

    view_matrix_(),
    projection_matrix_(),
    view_projection_matrix(),
    view_matrix_inv(),
    camera_rotation_(),
    target_to_position_len_(1.0f),
    near_(1.0f),
    far_(5000.0f),
    view_angle_(Math::DegToRad(60.0f)),
    aspect_(1.0f),
    width_(1280.0f),
    height_(720.0f),
    update_Projection_matrix_func(EnUpdateProjMatrixFunc::enUpdateProjMatrixFunc_Perspective),
    
    is_Need_Update_Projection_matrix(true),
    is_dirty_(false)

{
}

Camera::~Camera()
{
}

//ビュー行列、プロジェクション行列を更新する
void Camera::Update(const GraphicsEngine* graphicsEngine)
{
    //アスペクト比を計算
    this->aspect_ = (float)graphicsEngine->GetFrameBufferWidth() / (float) graphicsEngine->GetFrameBufferHeight();
    if (this->is_Need_Update_Projection_matrix)
    {
        switch (this->update_Projection_matrix_func)
        {
        case EnUpdateProjMatrixFunc::enUpdateProjMatrixFunc_Perspective:
            //透視変換行列を計算。
            this->projection_matrix_.MakeProjectionMatrix(
                this->view_angle_,
                this->aspect_,
                this->near_,
                this->far_
            );
            break;

        //case EnUpdateProjMatrixFunc::enUpdateProjMatrixFunc_Ortho:
        //    break;

        default:
            //平行投影行列を計算
            this->projection_matrix_.MakeOrthoProjectionMatrix(
                this->width_, 
                this->height_, 
                this->near_, 
                this->far_
            );
        }
    }

    //ビュー行列の算出
    this->view_matrix_.MakeLookAt(this->position_, this->target_, this->up_);
    //ビュープロジェクション行列の作成。
    this->view_projection_matrix = this->view_matrix_ * this->projection_matrix_;
    //ビュー行列の逆行列を計算。
    this->view_matrix_inv.Inverse(this->view_matrix_);

    this->forward_.Set(this->view_matrix_inv.m[2][0], this->view_matrix_inv.m[2][1], this->view_matrix_inv.m[2][2]);
    this->right_.Set(this->view_matrix_inv.m[0][0], this->view_matrix_inv.m[0][1], this->view_matrix_inv.m[0][2]);
    //カメラの回転行列を取得。
    this->camera_rotation_ = this->view_matrix_inv;
    this->camera_rotation_.m[3][0] = 0.0f;
    this->camera_rotation_.m[3][1] = 0.0f;
    this->camera_rotation_.m[3][2] = 0.0f;
    this->camera_rotation_.m[3][3] = 1.0f;

    Vector3 toPos;
    toPos.Subtract(this->position_, this->target_);
    this->target_to_position_len_ = toPos.Length();

    this->is_dirty_ = false;
}

//注視点を原点としてカメラを回転させる
void Camera::RotateOriginTarget(const Quaternion& qRot)
{
    Vector3 camera_pos = this->position_;
    Vector3 camera_target = this->target_;
    Vector3 toPos = camera_pos - camera_target;
    qRot.Apply(toPos);
    this->position_ = this->target_ + toPos;
    this->is_dirty_ = true;
}

//ワールド座標からスクリーン座標を計算する
void Camera::CalcScreenPositionFromWorldPosition(GraphicsEngine*& graphicsEngine, Vector2& screenPos, const Vector3& worldPos) const
{
    float half_w = (float)graphicsEngine->GetFrameBufferWidth() * 0.5f;
    float half_h = (float)graphicsEngine->GetFrameBufferHeight() * 0.5f;
    Vector4 _screenPos;
    _screenPos.Set(worldPos.x, worldPos.y, worldPos.z, 1.0f);
    this->view_projection_matrix.Apply(_screenPos);
    screenPos.x = (_screenPos.x / _screenPos.w) * half_w;
    screenPos.y = (_screenPos.y / _screenPos.w) * half_h;
}

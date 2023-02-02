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
	/// �ˉe�s��̍X�V���@�B
	/// </summary>
	enum class EnUpdateProjMatrixFunc {
		enUpdateProjMatrixFunc_Perspective,		//�����ˉe�s��B���ߖ@���������G����肽���Ȃ炱�����B
		enUpdateProjMatrixFunc_Ortho,			//���s���e�B�Q�c�I�ȕ\�����������Ȃ炱�����B
	};

public:
	/// <summary>
	/// �f�t�H���g �R���X�g���N�^
	/// </summary>
	Camera();

	/// <summary>
	/// �f�t�H���g �f�X�g���N�^
	/// </summary>
	~Camera();

	/// <summary>
	/// �r���[�s��A�v���W�F�N�V�����s����X�V����
	/// </summary>
	void Update(const GraphicsEngine* graphicsEngine);

	/// <summary>
	/// �����_�����_�Ƃ��ăJ��������]������
	/// </summary>
	/// <param name="qRot">��]������N�H�[�^�j�I��</param>
	void RotateOriginTarget(const Quaternion& qRot);

#pragma region Move Method
	/// <summary>
	/// �J�����𓮂���
	/// </summary>
	/// <param name="move">��������</param>
	void Move(const Vector3& move)
	{
		this->position_ += move;
		this->target_ += move;
		this->is_dirty_ = true;
	}

	/// <summary>
	/// �����_�𓮂���
	/// </summary>
	/// <param name="move">�ړ���</param>
	void MoveTarget(const Vector3& move)
	{
		this->target_ += move;
		this->is_dirty_ = true;
	}

	/// <summary>
	/// ���_�𓮂���
	/// </summary>
	/// <param name="move"></param>
	void MovePosition(const Vector3& move)
	{
		this->position_ += move;
		this->is_dirty_ = true;
	}

	/// <summary>
	/// �J�����̑O�������Ɉړ�
	/// </summary>
	/// <param name="moveForward"></param>
	void MoveForward(float moveForward)
	{
		Move(this->forward_ * moveForward);
	}

	/// <summary>
	/// �J�����̉E�����Ɉړ�
	/// </summary>
	/// <param name="moveRight"></param>
	void MoveRight(float moveRight)
	{
		Move(this->right_ * moveRight);
	}

	/// <summary>
	/// �J�����̏�����Ɉړ�
	/// </summary>
	/// <param name="moveUp"></param>
	void MoveUp(float moveUp)
	{
		Move(this->up_ * moveUp);
	}

#pragma endregion


	/// <summary>
	/// �J�����̃R�s�[���쐬
	/// </summary>
	/// <param name="dst"></param>
	void CopyTo(Camera& dst)
	{
		memcpy(&dst, this, sizeof(dst));
		this->is_dirty_ = true;
	}

	/// <summary>
	/// ���[���h���W����X�N���[�����W���v�Z����
	/// </summary>
	/// <remarks>
	/// �v�Z�����X�N���[�����W�͉�ʂ̒��S��{0,0}�A�����{��ʂ̕�*-0.5,��ʂ̍���*-0.5}
	/// �E����{ ��ʂ̕� * 0.5,��ʂ̍��� * 0.5 }�Ƃ�����W�n�ł�
	/// </remarks>
	/// <param name="screenPos">�X�N���[�����W�̊i�[��</param>
	/// <param name="worldPos">���[���h���W</param>
	void CalcScreenPositionFromWorldPosition(GraphicsEngine*& graphicsEngine, Vector2& screenPos, const Vector3& worldPos) const;

public:
#pragma region Get Method
	/// <summary>
	/// �ʒu �擾
	/// </summary>
	/// <returns>Vector3 position</returns>
	Vector3& GetPosition() { return position_; }

	/// <summary>
	/// �^�[�Q�b�g �擾	
	/// </summary>
	/// <returns>Vector target</returns>
	Vector3& GetTarget() { return target_; }

	/// <summary>
	/// �����_ �擾
	/// </summary>
	const Vector3& GetTarget() const
	{
		return this->target_;
	}

	/// <summary>
	/// �J�����̏���� �擾
	/// </summary>
	const Vector3& GetUp() const
	{
		return this->up_;
	}

	/// <summary>
	/// �r���[�s��̋t�s�� �擾
	/// </summary>
	const Matrix& GetViewMatrixInv(GraphicsEngine*& graphicEngine)
	{
		if (this->is_dirty_) {
			//�X�V����K�v������B
			Update(graphicEngine);
		}
		return this->view_matrix_inv;
	}

	/// <summary>
	/// �r���[�s�� �擾
	/// </summary>
	const Matrix& GetViewMatrix(GraphicsEngine*& graphicEngine)
	{
		if (this->is_dirty_) {
			//�X�V����K�v������B
			Update(graphicEngine);
		}
		return this->view_matrix_;
	}

	/// <summary>
	/// �v���W�F�N�V�����s�� �擾
	/// </summary>
	const Matrix& GetProjectionMatrix(GraphicsEngine*& graphicEngine)
	{
		if (this->is_dirty_) {
			//�X�V����K�v������B
			Update(graphicEngine);
		}
		return this->projection_matrix_;
	}

	/// <summary>
	/// �r���[�~�v���W�F�N�V�����s�� �擾
	/// </summary>
	const Matrix& GetViewProjectionMatrix(GraphicsEngine*& graphicEngine)
	{
		if (this->is_dirty_) {
			//�X�V����K�v������B
			Update(graphicEngine);
		}
		return this->view_projection_matrix;
	}

	/// <summary>
	/// �J�����̉�]�s�� �擾
	/// </summary>
	const Matrix& GetCameraRotation(GraphicsEngine*& graphicEngine)
	{
		if (this->is_dirty_) {
			//�X�V����K�v������
			Update(graphicEngine);
		}
		return this->camera_rotation_;
	}

	/// <summary>
	/// �����ʂ܂ł̋��� �擾
	/// </summary>
	float GetFar() const
	{
		return this->far_;
	}

	/// <summary>
	/// �ߕ��ʂ܂ł̋��� �擾
	/// </summary>
	float GetNear() const
	{
		return this->near_;
	}

	/// <summary>
	/// ���s���e�̕� �擾
	/// </summary>
	float GetWidth() const
	{
		return this->width_;
	}

	/// <summary>
	/// ���s���e�̍��� �擾
	/// </summary>
	float GetHeight() const
	{
		return this->height_;
	}

	/// <summary>
	/// ��p���擾
	/// </summary>
	/// <returns>��p (�P��:���W�A��)</returns>
	float GetViewAngle() const
	{
		return this->view_angle_;
	}

	/// <summary>
	/// �����_�Ǝ��_�̋��� �擾
	/// </summary>
	/// <returns></returns>
	float GetTargetToPositionLength() const
	{
		return this->target_to_position_len_;
	}

	/// <summary>
	/// �J�����̑O���� �擾
	/// </summary>
	const Vector3& GetForward() const
	{
		return this->forward_;
	}

	/// <summary>
	/// �J�����̉E���� �擾
	/// </summary>
	const Vector3& GetRight() const
	{
		return this->right_;
	}

	/// <summary>
	/// �A�X�y�N�g�� �擾
	/// </summary>
	float GetAspect() const
	{
		return this->aspect_;
	}

#pragma endregion

#pragma region Set Method
	/// <summary>
	/// �J�����̍��W �ݒ�
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
	/// �����_ �ݒ�
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
	/// �J�����̏���� �ݒ�
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
	/// �����ʂ܂ł̋��� �ݒ�
	/// </summary>
	void SetFar(float fFar)
	{
		this->far_ = fFar;
		this->is_dirty_ = true;
	}

	/// <summary>
	/// �ߕ��ʂ܂ł̋��� �ݒ�
	/// </summary>
	void SetNear(float fNear)
	{
		this->near_ = fNear;
		this->is_dirty_ = true;
	}

	/// <summary>
	/// ���s���e�̕� �ݒ�
	/// </summary>
	/// <remarks>
	/// SetUpdateProjMatrixFunc��enUpdateProjMatrixFunc_Ortho���ݒ肳��Ă���Ƃ��Ɏg�p�����
	/// </remarks>
	void SetWidth(float w)
	{
		this->width_ = w;
		this->is_dirty_ = true;
	}

	/// <summary>
	/// ���s���e�̍��� �ݒ�
	/// </summary>
	/// <remarks>
	/// SetUpdateProjMatrixFunc��enUpdateProjMatrixFunc_Ortho���ݒ肳��Ă���Ƃ��Ɏg�p�����
	/// </remarks>
	void SetHeight(float h)
	{
		this->height_ = h;
		this->is_dirty_ = true;
	}

	/// <summary>
	/// ���s���e�̍�����ݒ�
	/// </summary>
	/// <remarks>
	/// SetUpdateProjMatrixFunc��enUpdateProjMatrixFunc_Ortho���ݒ肳��Ă���Ƃ��Ɏg�p�����
	/// </remarks>
	void SetUpdateProjMatrixFunc(EnUpdateProjMatrixFunc func)
	{
		this->update_Projection_matrix_func = func;
		this->is_dirty_ = true;
	}

	/// <summary>
	/// ��p��ݒ�
	/// </summary>
	/// <remarks>
	/// SetUpdateProjMatrixFunc��enUpdateProjMatrixFunc_Perspective���ݒ肳��Ă���Ƃ��Ɏg�p�����
	/// </remarks>
	/// <param name="viewAngle">��p�B�P�ʃ��W�A��</param>
	void SetViewAngle(float viewAngle)
	{
		this->view_angle_ = viewAngle;
		this->is_dirty_ = true;
	}

#pragma endregion

private:
	//�J�����ʒu�B
	Vector3		position_;
	//�J�����̏�����B
	Vector3		up_;
	//�J�����̒��~�_�B
	Vector3		target_;
	//�J�����̑O���B
	Vector3		forward_;
	//�J�����̉E�B
	Vector3		right_ ;

	//�r���[�s��B
	Matrix		view_matrix_;
	//�v���W�F�N�V�����s��B
	Matrix		projection_matrix_;
	//�r���[�v���W�F�N�V�����s��B
	Matrix		view_projection_matrix;
	//�r���[�s��̋t�s��B
	Matrix		view_matrix_inv;
	//�J�����̉�]�s��B
	Matrix		camera_rotation_;

	//�����_�Ǝ��_�܂ŋ����B
	float		target_to_position_len_;
	//�ߕ��ʁB
	float		near_;
	//�����ʁB
	float		far_;
	//��p(���W�A��)�B
	float		view_angle_;
	//�A�X�y�N�g��B
	float		aspect_;
	//���s���e�s����쐬����Ƃ��Ɏg�p����镝�B
	float		width_ ;
	//���s���e�s����쐬����Ƃ��Ɏg�p����鍂���B
	float		height_;

	//�v���W�F�N�V�����s��̍X�V�̎d���B
	EnUpdateProjMatrixFunc update_Projection_matrix_func;

	bool		is_Need_Update_Projection_matrix;

	//�_�[�e�B�t���O�B
	bool		is_dirty_;
};

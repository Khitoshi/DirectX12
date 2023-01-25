#pragma once
//#include <fbxsdk.h>
#include "TksFile.h"
#include "Skeleton.h"
#include "TkmFile.h"
#include "MeshParts.h"
#include "ModelData.h"

class GraphicsEngine;

class Model
{
public:
	Model();
	~Model();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="tk">DirectXtk�֌W��ۗL</param>
	/// <param name="graphicsEngine">�f�o�C�X���i�[���Ă���</param>
	/// <param name="initData">�������f�[�^</param>
	void Init(tkEngine* tk,GraphicsEngine* graphicsEngine,const ModelInitData& initData);

	/// <summary>
	/// ���[���h�s����v�Z���āA�����o�ϐ���world�s����X�V����
	/// </summary>
	/// <param name="pos">���W</param>
	/// <param name="rot">��]</param>
	/// <param name="scale">�g�嗦</param>
	void UpdateWorldMatrix(Vector3 pos, Quaternion rot, Vector3 scale);

	/// <summary>
	/// �`��(�J�����w���)
	/// </summary>
	/// <param name="renderContext">�����_�����O�R���e�L�X�g</param>
	/// <param name="camera">�J����</param>
	void Draw(GraphicsEngine* graphicsEngine, RenderContext& rc, Camera& camera);

	/// <summary>
	/// �`��(�J�����s��w���)
	/// </summary>
	/// <param name="renderContext">�����_�����O�R���e�L�X�g</param>
	/// <param name="viewMatrix">�r���[�s��</param>
	/// <param name="projMatrix">�v���W�F�N�V�����s��</param>
	void Draw(GraphicsEngine* graphicsEngine, RenderContext& rc, const Matrix& viewMatrix, const Matrix& projMatrix);

	/// <summary>
	/// �C���X�^���V���O�`��
	/// </summary>
	/// <param name="renderContext">�����_�����O�R���e�L�X�g</param>
	/// <param name="numInstance">�C���X�^���X�̐�</param>
	void DrawInstancing(GraphicsEngine* graphicsEngine, RenderContext& renderContext, int numInstance, Camera* camera);

	/// <summary>
	/// ���b�V���ɑ΂��Ė₢���킹���s���B
	/// </summary>
	/// <param name="queryFunc">�₢���킹�֐�</param>
	void QueryMeshs(std::function<void(const SMesh& mesh)> queryFunc)
	{
		this->mesh_parts_.QueryMeshs(queryFunc);
	}
	void QueryMeshAndDescriptorHeap(std::function<void(const SMesh& mesh, const DescriptorHeap& ds)> queryFunc)
	{
		this->mesh_parts_.QueryMeshAndDescriptorHeap(queryFunc);
	}


	/// <summary>
	/// ���[���h�s����v�Z����
	/// </summary>
	/// <remark>
	/// ���̊֐���UpdateWorldMatrix�֐��̒�����g���Ă��܂�
	/// Model�N���X�̎g�p�ɉ��������[���h�s��̌v�Z���s�������ꍇ
	/// �{�֐��𗘗p����ƌv�Z���邱�Ƃ��ł��܂�
	/// </remark>
	/// <param name="pos">���W</param>
	/// <param name="rot">��]</param>
	/// <param name="scale">�g�嗦</param>
	/// <returns></returns>
	Matrix CalcWorldMatrix(Vector3 pos, Quaternion rot, Vector3 scale)
	{
		Matrix world;
		Matrix bias;
		if (this->model_up_axis_ == enModelUpAxisZ) {
			//Z-up
			bias.MakeRotationX(Math::PI() * -0.5f);
		}
		Matrix mTrans, mRot, mScale;
		mTrans.MakeTranslation(pos);
		mRot.MakeRotationFromQuaternion(rot);
		mScale.MakeScaling(scale);

		world = bias * mScale * mRot * mTrans;
		return world;
	}

public:
#pragma region Get Mehod

	/// <summary>
	/// ���[���h�s����擾
	/// </summary>
	/// <returns></returns>
	const Matrix& GetWorldMatrix() const
	{
		return this->world_;
	}

	/// <summary>
	/// TKM�t�@�C�����擾
	/// </summary>
	/// <returns></returns>
	const TkmFile& GetTkmFile() const
	{
		return *this->tkm_file_;
	}

	/// <summary>
	/// �������m�F�t���O�擾 
	/// true = �������ς�
	/// </summary>
	/// <returns></returns>
	bool IsInited()const 
	{ 
		return this->is_Inited_;
	}

#pragma endregion

#pragma region set & change
	/// <summary>
	/// �A���x�h�}�b�v��ύX
	/// </summary>
	/// <remarks>
	/// ���̊֐����Ăяo���ƃf�B�X�N���v�^�q�[�v�̍č\�z���s���邽�߁A�������ׂ�������܂�
	/// ���t���[���Ăяo���K�v���Ȃ��ꍇ�͌Ăяo���Ȃ��悤�ɂ��Ă�������
	/// </remarks>
	/// <param name="materialName">�ύX�������}�e���A���̖��O</param>
	/// <param name="albedoMap">�A���x�h�}�b�v</param>
	void ChangeAlbedoMap(const char* materialName, Texture& albedoMap);

#pragma endregion

private:
	//����������Ă���
	bool is_Inited_;

	//���[���h�s��
	Matrix world_;

	//.tkm file
	TkmFile* tkm_file_;

	//�X�P���g��
	Skeleton skeleton_;

	//���b�V���p�[�c
	MeshParts mesh_parts_;

	//���f���̏����(blender�ł͏�������قȂ�)
	EnModelUpAxis model_up_axis_;
};

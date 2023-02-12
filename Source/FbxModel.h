#pragma once

#include "ModelData.h"

class tkEngine;
class GraphicsEngine;

class FbxModel
{
public:

    FbxModel();
    ~FbxModel();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="tk">DirectXtk�֌W��ۗL</param>
	/// <param name="graphicsEngine">�f�o�C�X���i�[���Ă���</param>
	/// <param name="initData">�������f�[�^</param>
	void Init(tkEngine* tk, GraphicsEngine* graphicsEngine, const ModelInitData& initData);

private:

	//���b�V���p�[�c
	MeshParts mesh_parts_;

	//���f���̏����(blender�ł͏�������قȂ�)
	EnModelUpAxis model_up_axis_;
};


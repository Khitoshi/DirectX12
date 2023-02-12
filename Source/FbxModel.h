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
	/// 初期化
	/// </summary>
	/// <param name="tk">DirectXtk関係を保有</param>
	/// <param name="graphicsEngine">デバイスを格納している</param>
	/// <param name="initData">初期化データ</param>
	void Init(tkEngine* tk, GraphicsEngine* graphicsEngine, const ModelInitData& initData);

private:

	//メッシュパーツ
	MeshParts mesh_parts_;

	//モデルの上方向(blenderでは上方向が異なる)
	EnModelUpAxis model_up_axis_;
};


#pragma once
#include <utility>
#include "IShaderResource.h"

#include "Skeleton.h"
#include "MeshParts.h"

//モデルの上方向
enum EnModelUpAxis {
    enModelUpAxisY,		//モデルの上方向がY軸。
    enModelUpAxisZ,		//モデルの上方向がZ軸。
};


struct ModelInitData
{
    //ファイルパス
    const char* model_File_Path_;

    //.fxファイルのファイルパス。
    const char* shader_File_Path = nullptr;

    //頂点シェーダーのエントリーポイント。
    const char* vs_Entry_Point_Func_ = "VSMain";

    //スキンありマテリアル用の頂点シェーダーのエントリーポイント。
    const char* vs_Skin_Entry_Point_Func_ = "VSMain";

    //ピクセルシェーダーのエントリーポイント。
    const char* ps_entry_point_func_ = "PSMain";


    //ユーザー拡張の定数バッファ。
    void* expand_Constant_Buffer = nullptr;
    //ユーザー拡張の定数バッファのサイズ。
    int expand_Constant_Buffer_Size = 0;


    //ユーザー拡張のシェーダーリソース。
    std::array<IShaderResource*, 6> expand_Shader_Resoruce_View_ = { nullptr };
    //std::array<IShaderResource*, MeshParts::GetMAXMODELEXPANDSRV()> expand_Shader_Resoruce_View_ = { nullptr };

    //スケルトン。
    Skeleton* skeleton_ = nullptr;

    //モデルの上方向。
    EnModelUpAxis en_model_Up_Axis_ = EnModelUpAxis::enModelUpAxisZ;

    //テクスチャサンプリングのフィルタ。
    D3D12_FILTER sampler_Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;

    //レンダリングするカラーバッファのフォーマット。
    std::array<DXGI_FORMAT, static_cast<int>(D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT)> color_Buffer_Format_ = {
        DXGI_FORMAT_R8G8B8A8_UNORM,
        DXGI_FORMAT_UNKNOWN,
        DXGI_FORMAT_UNKNOWN,
        DXGI_FORMAT_UNKNOWN,
        DXGI_FORMAT_UNKNOWN,
        DXGI_FORMAT_UNKNOWN,
        DXGI_FORMAT_UNKNOWN,
        DXGI_FORMAT_UNKNOWN,
    };
}; 

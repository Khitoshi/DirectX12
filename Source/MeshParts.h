#pragma once

#include <array>
#include <vector>
#include <memory>
#include <functional>

#include "TkmFile.h"
#include "StructuredBuffer.h"
#include "ConstantBuffer.h"
#include "IShaderResource.h"
#include "Matrix.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "DescriptorHeap.h"
#include "MeshParts.h"
#include "tkEngine.h"
class Material;
class Skeleton;

struct SMesh
{
    //頂点バッファ
    VertexBuffer vertexBuffer;
    //インデックスバッファ
    std::vector<IndexBuffer*> indexBufferArray;
    //マテリアル
    std::vector<Material*> materials;
    //スキンを持っているかどうかのフラグ
    std::vector<int> skinFlags;
};

class MeshParts
{
private:
    //拡張SRVが設定されるレジスタの開始番号
    enum { EXPAND_SRV_REG__START_NO = 10 };
    //拡張SRVの最大数
    enum { MAX_MODEL_EXPAND_SRV = 6 };
    //１つのマテリアルで使用されるSRVの数
    enum { NUM_SRV_ONE_MATERIAL = EXPAND_SRV_REG__START_NO + MAX_MODEL_EXPAND_SRV };
    //１つのマテリアルで使用されるCBVの数
    enum { NUM_CBV_ONE_MATERIAL = 6 };
public:

    /// <summary>
    /// デフォルト コンストラクタ
    /// </summary>
    MeshParts();

    /// <summary>
    /// デフォルト デストラクタ
    /// </summary>
    ~MeshParts();

    /// <summary>
    /// .tkm file 初期化
    /// </summary>
    /// <param name="graphicsEngine">デバイス 取得</param>
    /// <param name="tkmFile">tkm file</param>
    /// <param name="fxFilePath">.fx file path</param>
    /// <param name="vsEntryPointFuncName">頂点シェーダーのエントリーポイント名</param>
    /// <param name="vsSkinEntryPointFuncName">スキンありマテリアル用の頂点シェーダーのエントリーポイント名</param>
    /// <param name="psEntryPointFuncName">ピクセルシェーダーのエントリーポイント名</param>
    /// <param name="expandData"></param>
    /// <param name="expandDataSize"></param>
    /// <param name="expandShaderResourceView"></param>
    /// <param name="colorBufferFormat">このモデルをレンダリングするカラーバッファのフォーマット</param>
    /// <param name="samplerFilter">サンプラフィルタ</param>
    void InitFromTkmFile(
        tkEngine*& tk,
        GraphicsEngine*& graphicsEngine,
        const TkmFile& tkmFile,
        const char* fxFilePath,
        const char* vsEntryPointFuncName,
        const char* vsSkinEntryPointFuncName,
        const char* psEntryPointFuncName,
        void* expandData,
        int expandDataSize,
        const std::array<IShaderResource*, MAX_MODEL_EXPAND_SRV>& expandShaderResourceView,
        const std::array<DXGI_FORMAT, static_cast<int>(D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT)>& colorBufferFormat,
        D3D12_FILTER samplerFilter
    );

    /// <summary>
    /// 描画
    /// </summary>
    /// <param name="renderContext">レンダリングコンテキスト</param>
    /// <param name="matrixWorld">ワールド 行列</param>
    /// <param name="matrixView">ビュー 行列</param>
    /// <param name="matrixProjection">プロジェクション 行列</param>
    void Draw(
        GraphicsEngine*& graphicsEngine,
        RenderContext& renderContext,
        const Matrix& matrixWorld,
        const Matrix& matrixView,
        const Matrix& matrixProjection
    );

    /// <summary>
    /// インスタンシング描画
    /// </summary>
    /// <param name="renderContext">レンダリングコンテキスト</param>
    /// <param name="numInstance">インスタンス数</param>
    /// <param name="mView">ビュー行列</param>
    /// <param name="mProj">プロジェクション行列</param>
    void DrawInstancing(GraphicsEngine*& graphicsEngine,RenderContext& renderContext, int numInstance, const Matrix& matrixView, const Matrix& matrixProjection);

    /// <summary>
    /// スケルトンを関連付ける
    /// </summary>
    /// <param name="skeleton">スケルトン</param>
    void BindSkeleton(GraphicsEngine*& graphicsEngine,Skeleton& skeleton);

    /// <summary>
    /// メッシュに対して問い合わせを行う。
    /// </summary>
    /// <param name="queryFunc">クエリ関数</param>
    void QueryMeshs(std::function<void(const SMesh& mesh)> queryFunc)
    {
        for (const auto& mesh : this->meshs_) {
            queryFunc(*mesh);
        }
    }
    void QueryMeshAndDescriptorHeap(std::function<void(const SMesh& mesh, const DescriptorHeap& ds)> queryFunc)
    {
        for (int i = 0; i < this->meshs_.size(); i++) {
            queryFunc(*this->meshs_[i], this->descriptor_Heap_);
        }
    }


    /// <summary>
    /// ディスクリプタヒープを作成。
    /// </summary>
    /// <param name = "graphicsEngine">デバイスを格納している</param>
    void CreateDescriptorHeaps(GraphicsEngine*& graphicsEngine);

private:
    /// <summary>
    /// tkmメッシュからメッシュを作成。
    /// </summary>
    /// <param name="mesh">メッシュ</param>
    /// <param name="meshNo">メッシュ番号</param>
    /// <param name="fxFilePath">fxファイルのファイルパス</param>
    /// <param name="vsEntryPointFuncName">頂点シェーダーのエントリーポイントの名前</param>
    /// <param name="vsSkinEntryPointFuncName">スキンありマテリアル用の頂点シェーダーのエントリーポイントの名前</param>
    /// <param name="psEntryPointFuncName">ピクセルシェーダーのエントリーポイントの名前</param>
    /// <param name="colorBufferFormat">このモデルをレンダリングするカラーバッファのフォーマット</param>
    /// <param name="samplerFilter">サンプラフィルタ</param>
    void CreateMeshFromTkmMesh(
        tkEngine*& tk,
        GraphicsEngine*& graphicsEngine,
        const TkmFile::SMesh& mesh,
        int meshNo,
        int& materialNum,
        const char* fxFilePath,
        const char* vsEntryPointFuncName,
        const char* vsSkinEntryPointFuncName,
        const char* psEntryPointFuncName,
        const std::array<DXGI_FORMAT, static_cast<int>D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT>& colorBufferFormat,
        D3D12_FILTER samplerFilter
    );

    /// <summary>
    /// 描画処理の共通処理
    /// </summary>
    /// <param name="rc">レンダリングコンテキスト</param>
    /// <param name="mWorld">ワールド行列</param>
    /// <param name="mView">ビュー行列</param>
    /// <param name="mProj">プロジェクション行列</param>
    void DrawCommon(GraphicsEngine*& graphicsEngine,RenderContext& renderContext, const Matrix& matrixWorld, const Matrix& matrixView, const Matrix& matrixProjection);

public:
#pragma region Get method
    /// <summary>
    /// 拡張SRVが設定されるレジスタの開始番号 取得
    /// </summary>
    /// <returns>static int</returns>
    const static int GetEXPANDSRVREGSTARTNO() { return EXPAND_SRV_REG__START_NO; }

    /// <summary>
    /// 拡張SRVの最大数 取得
    /// </summary>
    /// <returns>static int</returns>
    const static int GetMAXMODELEXPANDSRV() { return MAX_MODEL_EXPAND_SRV; }

    /// <summary>
    /// １つのマテリアルで使用されるSRVの数 取得
    /// </summary>
    /// <returns>static int</returns>
    const static int GetNUMSRVONEMATERIAL() { return NUM_SRV_ONE_MATERIAL; }

    /// <summary>
    /// １つのマテリアルで使用されるCBVの数 取得
    /// </summary>
    /// <returns>static int</returns>
    const static int GetNUMCBVONEMATERIAL() { return NUM_CBV_ONE_MATERIAL; }
#pragma endregion

private:
    /// <summary>
    /// 定数バッファ。
    /// </summary>
    /// <remarks>
    /// この構造体を変更したら、SimpleModel.fxのCBも変更するように。
    /// </remarks>
    struct SConstantBuffer {
        Matrix world;		//ワールド行列。
        Matrix view;		//ビュー行列。
        Matrix proj;		//プロジェクション行列。
    };

    //メッシュ共通の定数バッファ
    ConstantBuffer common_Constant_Buffer_;
    //ユーザー拡張用の定数バッファ
    ConstantBuffer expand_Constant_Buffer_;

    std::array<IShaderResource*, static_cast<int>(MAX_MODEL_EXPAND_SRV)>expand_Shader_Resource_View_;
    
    //ボーンの行列の構造化バッファ
    StructuredBuffer bone_Matrices_Structure_Buffer_;

    //メッシュ
    std::vector<std::unique_ptr<SMesh>> meshs_;

    //ディスクリプタヒープ
    DescriptorHeap descriptor_Heap_;

    //スケルトン
    Skeleton* skelton_;

    void* expand_Data_;

};


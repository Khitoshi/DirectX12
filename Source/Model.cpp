#include "Model.h"
#include "Camera.h"

//デフォルト コンストラクタ
Model::Model():
    is_Inited_(false),
    world_(),
    tkm_file_(nullptr),
    skeleton_(),
    mesh_parts_(),
    model_up_axis_(EnModelUpAxis::enModelUpAxisY)
{
}

//デフォルト デストラクタ
Model::~Model()
{
    if (tkm_file_ != nullptr)
    {
        delete tkm_file_;
    }
}

//初期化
void Model::Init(tkEngine*& tk, GraphicsEngine*& graphicsEngine, const ModelInitData& initData)
{
    //ファイルパスのチェック
    if (initData.model_File_Path_ == nullptr)
    {
        //メッセージボックスを表示して，異常終了する
        MessageBox(nullptr, TEXT("Model::Initでモデルのファイルパスが指定されていません"), TEXT("エラー"), MB_OK);
        std::abort();
    }

    //スケルトンのチェック
    if (initData.skeleton_ != nullptr) {
        //スケルトンが指定されている。
        this->mesh_parts_.BindSkeleton(graphicsEngine, *initData.skeleton_);
    }

    //モデルの上方向設定
    this->model_up_axis_ = initData.en_model_Up_Axis_;

    //.tkm file の重複をチェック
    auto tkm_file = tk->GetTkmFileFromBank(initData.model_File_Path_);
    if (tkm_file == nullptr) {
        //未登録なので登録する
        tkm_file = new TkmFile;
        tkm_file->Load(initData.model_File_Path_);
        tk->RegistTkmFileToBank(initData.model_File_Path_, tkm_file);
    }

    this->tkm_file_ = tkm_file;

    this->mesh_parts_.InitFromTkmFile(
        tk,
        graphicsEngine,
        *this->tkm_file_,
        initData.shader_File_Path,
        initData.vs_Entry_Point_Func_,
        initData.vs_Skin_Entry_Point_Func_,
        initData.ps_entry_point_func_,
        initData.expand_Constant_Buffer,
        initData.expand_Constant_Buffer_Size,
        initData.expand_Shader_Resoruce_View_,
        initData.color_Buffer_Format_,
        initData.sampler_Filter
    );

    //ワールド行列を計算して，メンバ変数のworldを更新する
    UpdateWorldMatrix(Vector3::GetVec3Zero(), Quaternion::GetIdentity(), Vector3::GetVec3One());

    //初期化したので初期化フラグを建てる
    this->is_Inited_ = true;
}

//ワールド行列を計算して、メンバ変数のworld行列を更新する
void Model::UpdateWorldMatrix(Vector3 pos, Quaternion rot, Vector3 scale)
{
    //更新
    this->world_ = CalcWorldMatrix(pos, rot, scale);
}

void Model::Draw(GraphicsEngine*& graphicsEngine, RenderContext& rc, Camera& camera)
{
    Draw(
        graphicsEngine,
        rc, 
        camera.GetViewMatrix(graphicsEngine), 
        camera.GetProjectionMatrix(graphicsEngine)
    );
}

//描画(カメラ行列指定版)
void Model::Draw(GraphicsEngine*& graphicsEngine, RenderContext& rc, const Matrix& viewMatrix, const Matrix& projMatrix)
{
    this->mesh_parts_.Draw(
        graphicsEngine,
        rc,
        this->world_,
        viewMatrix,
        projMatrix
    );
}

//インスタンシング描画
void Model::DrawInstancing(GraphicsEngine*& graphicsEngine,RenderContext& rc, int numInstance, Camera* camera)
{
    this->mesh_parts_.DrawInstancing(
        graphicsEngine,
        rc,
        numInstance,
        camera->GetViewMatrix(graphicsEngine),
        camera->GetProjectionMatrix(graphicsEngine)
    );
}




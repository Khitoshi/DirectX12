#include "FbxModel.h"

//コンストラクタ
FbxModel::FbxModel():
    mesh_parts_(),
    model_up_axis_()
{
}

//デストラクタ
FbxModel::~FbxModel()
{
}

//初期化
void FbxModel::Init(tkEngine* tk, GraphicsEngine* graphicsEngine, const ModelInitData& initData)
{
    //モデルのファイルパスが存在しない場合　エラー
    if (!initData.model_File_Path_)
    {
        //メッセージボックスを表示して，異常終了する
        MessageBox(nullptr, TEXT("Model::Initでモデルのファイルパスが指定されていません"), TEXT("エラー"), MB_OK);
        std::abort();
    }

    //スケルトンのチェック
    if (initData.skeleton_) {
        //スケルトンが指定されている。
        this->mesh_parts_.BindSkeleton(graphicsEngine, *initData.skeleton_);
    }

    //モデルの上方向設定
    model_up_axis_ = initData.en_model_Up_Axis_;

    //重複ファイルの確認
    auto file = tk->GetTkmFileFromBank(initData.model_File_Path_);
    if (file == nullptr) {
        //未登録なので登録する
        file = new TkmFile;
        file->Load(initData.model_File_Path_);
        tk->RegistTkmFileToBank(initData.model_File_Path_, file);
    }




}



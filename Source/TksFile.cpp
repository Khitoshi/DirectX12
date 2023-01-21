#include <Windows.h>

#include "TksFile.h"

//コンストラクタ
TksFile::TksFile():
    num_Bone_(0),
    bones_(0)
{
}

//デストラクタ
TksFile::~TksFile()
{
}

//.tks ファイル 読み込み
void TksFile::Load(const char* filePath)
{
    //ファイル 読み込み
    FILE* fp;
    fopen_s(&fp, filePath, "rb");
    
    //読み込み チェック
    if (fp)
    {
        //失敗したのでメッセージボックスを表示して強制終了させる
        MessageBox(nullptr, TEXT(".tksファイルの読み込みに失敗しました"), TEXT("エラー"), MB_OK);
        std::abort();
    }

    //骨の総数 取得
    fread(&this->num_Bone_, sizeof(this->num_Bone_), 1, fp);

    //骨リストのサイズを設定
    this->bones_.resize(this->num_Bone_);

    //全骨　情報　取得
    for (int i = 0; i < this->num_Bone_; i++)
    {
        //骨 取得
        auto& bone = this->bones_.at(i);


        //骨の名前 取得
        size_t name_count = 0;
        fread(&name_count, 1, 1, fp);
        bone.name = std::make_unique<char[]>(name_count + 1);
        //bone.name = make_unique<std::string>();
        
        //骨の名前 設定
        fread(bone.name.get(), name_count + 1, 1, fp);

        //親のID 設定
        fread(&bone.id, sizeof(bone.parent_No), 1, fp);
        
        //バインドポーズ 設定
        fread(&bone.bind_Pose, sizeof(bone.bind_Pose), 1, fp);

        //バインドポーズの逆数 設定
        fread(&bone.inv_Bind_Pose, sizeof(bone.inv_Bind_Pose), 1, fp);
        
        //ボーンの番号 設定
        bone.id = i;
    }

    //ファイル を閉じる
    fclose(fp);
}

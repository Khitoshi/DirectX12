#pragma once

#include <memory>
#include <vector>
#include <string>
#include <functional>

class TksFile
{
public:
    /// <summary>
    /// ボーン
    /// </summary>
    struct SBone {
        //ボーンの名前
        std::unique_ptr<char[]> name;
        //親番号
        int parent_No;
        //バインドポーズ
        float bind_Pose[4][3];
        //バインドポーズの逆数
        float inv_Bind_Pose[4][3];
        //ボーンの番号
        int id;
    };

public:

    /// <summary>
    /// コンストラクタ
    /// </summary>
    TksFile();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~TksFile();


    /// <summary>
    /// .tks ファイル　読み込み
    /// </summary>
    /// <param name="filePath"></param>
    void Load(const char* filePath);

    /// <summary>
    /// ボーンに対してクエリを行う
    /// </summary>
    /// <param name="query">クエリ関数</param>
    void QueryBone(std::function<void(SBone& bone)>query)
    {
        for (auto& bone : this->bones_)
        {
            query(bone);
        }
    }

private:
    //骨の総数
    int num_Bone_;

    //骨のリスト
    std::vector<SBone> bones_;

};


#include <memory>
#include <iostream>
#include <Windows.h>
#include "Model.h"

#include "System.h"
#include "Camera.h"
#include "Light.h"

#pragma warning(disable : 4996)

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    //カメラ
    std::unique_ptr<Camera> camera_3d;
    camera_3d = std::make_unique<Camera>();

    //graphicエンジン
    //std::unique_ptr<GraphicsEngine> graphicsEngine;
    //graphicsEngine = std::make_unique<GraphicsEngine>();
    // 
    //DirectXTK エンジン
    //std::unique_ptr<tkEngine> engine;
    //engine = std::make_unique<tkEngine>();
    tkEngine* engine;
    engine = new tkEngine();
    std::unique_ptr<System>system;
    system = std::make_unique<System>();

    system->InitWindow(
        hInstance,
        hPrevInstance,
        lpCmdLine,
        nCmdShow,
        TEXT("DirectX12 自作ライブラリ")
        );

    //graphicsEngine = std::make_unique<GraphicsEngine>(system->GetHWnd(), system->GetFrameBufferWidth(), system->GetFrameBufferHeight());

    engine->Init(
        system->GetHWnd(),
        system->GetFrameBufferWidth(),
        system->GetFrameBufferHeight(),
        //*graphicsEngine,
        *camera_3d
    );


    //3D用　カメラ
    //カメラの位置を設定
    camera_3d->GetPosition().Set(0.0f, 0.0f, 100.0f);
    //カメラのターゲットを設定
    camera_3d->GetTarget().Set(0.0f, 0.0f, 0.0f);



    //ライト
    std::unique_ptr<Light> light;
    light = std::make_unique<Light>();
    //ライトは右から当たっている
    light->GetDirectionLight().direction.Set(1.0f, -1.0f, -1.0f);
    light->GetDirectionLight().direction.Normalize();
    //ライトの色は白
    light->GetDirectionLight().color.Set(0.5f, 0.5f, 0.5f);
    //視点の位置を設定する
    light->GetEyePosition().Set(camera_3d->GetPosition());

    // モデルを初期化する
    // モデルを初期化するための情報を構築する
    ModelInitData modelInitData;
    modelInitData.model_File_Path_ = "./Assets/Model/teapot.tkm";

    // 使用するシェーダーファイルパスを設定する
    modelInitData.shader_File_Path = "./Assets/Shader/sample.fx";

    // ディレクションライトの情報を定数バッファとしてディスクリプタヒープに登録するために
    // モデルの初期化情報として渡す
    modelInitData.expand_Constant_Buffer = &light;
    modelInitData.expand_Constant_Buffer_Size = sizeof(light);

    Model* model;
    model = new Model();
    model->Init(engine, engine->GetGraphicsEngine(), modelInitData);



    //以下更新コード
    auto& renderContext = engine->GetGraphicsEngine()->GetRenderContext();

    // ここからゲームループ
    while (system->DispatchWindowMessage())
    {
        //レンダリング開始
        engine->BeginFrame(engine->GetGraphicsEngine(),*camera_3d.get());

        //ここから絵を書くコードを記述
        //Quaternion qRot;
        //qRot.SetRotationDegY(g_pad[0]->GetLStickXF());
        //qRot.Apply(light.directionLight.direction);
        //qRot.SetRotationDegX(g_pad[0]->GetLStickYF());
        //qRot.Apply(light.directionLight.direction);

        model->Draw(engine->GetGraphicsEngine(), renderContext, *camera_3d);

        engine->EndFrame();
    }
    //delete model;
    return 0;
}

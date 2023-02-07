
#include <memory>
#include <iostream>
#include <Windows.h>
#include "Model.h"

#include "System.h"
#include "Camera.h"
#include "Light.h"
#include "imgui\imgui.h"
#include "imgui\imgui_impl_dx12.h"
#include "imgui\imgui_impl_win32.h"

#pragma warning(disable : 4996)

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
#ifdef _DEBUG
    //メモリリーク検出用
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    //_CrtSetBreakAlloc(393639);
#endif // _DEBUG

    //カメラ
    std::unique_ptr<Camera> camera_3d;
    camera_3d = std::make_unique<Camera>();

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

    system->Init(engine->GetGraphicsEngine());

    //3D用　カメラ
    //カメラの位置を設定
    camera_3d->SetPosition(0.0f, 0.0f, 100.0f);
    //カメラのターゲットを設定
    //camera_3d->GetTarget().Set(0.0f, 0.0f, 0.0f);
    camera_3d->SetTarget(0.0f, 0.0f, 0.0f);

    //ライト
    //std::unique_ptr<Light> light;
    //light = std::make_unique<Light>();
    Light light;

    /*
    //ライトは右から当たっている
    light->GetDirectionLight().direction.Set(1.0f, -1.0f, -1.0f);
    light->GetDirectionLight().direction.Normalize();
    //ライトの色は白
    light->GetDirectionLight().color.Set(0.5f, 0.5f, 0.5f);
    //視点の位置を設定する
    light->GetEyePosition().Set(camera_3d->GetPosition());
    */

    //ライトは右から当たっている
    //light.GetDirectionLight().direction.Set(1.0f, -1.0f, -1.0f);
    light.direction_Light_.direction.x = 1.0f;
    light.direction_Light_.direction.y = -1.0f;
    light.direction_Light_.direction.z = -1.0f;
    light.GetDirectionLight().direction.Normalize();

    //ライトの色は白
    //light.GetDirectionLight().color.Set(0.5f, 0.5f, 0.5f);
    light.direction_Light_.color.x = 1.0f;
    light.direction_Light_.color.y = 1.0f;
    light.direction_Light_.color.z = 1.0f;

    //視点の位置を設定する
    //light.GetEyePosition().Set(camera_3d->GetPosition());
    light.eye_Position_ = camera_3d->GetPosition();

    // モデルを初期化する
    // モデルを初期化するための情報を構築する
    ModelInitData modelInitData;
    //モデルファイルパス
    modelInitData.model_File_Path_ = "./Assets/Model/teapot.tkm";
    // 使用するシェーダーファイルパスを設定する
    modelInitData.shader_File_Path = "./Assets/Shader/sample.fx";

    // ディレクションライトの情報を定数バッファとしてディスクリプタヒープに登録するために
    // モデルの初期化情報として渡す
    modelInitData.expand_Constant_Buffer = &light;
    modelInitData.expand_Constant_Buffer_Size = sizeof(light);

    Model model = {};
    model.Init(engine, engine->GetGraphicsEngine(), modelInitData);
    //model = new Model();

    //以下更新コード
    auto graphicsEngine = engine->GetGraphicsEngine();
    auto& renderContext = engine->GetGraphicsEngine()->GetRenderContext();

    // ここからゲームループ
    while (system->DispatchWindowMessage())
    {
        //レンダリング開始
        engine->BeginFrame(graphicsEngine,*camera_3d);
        system->Update(engine);


        //ここから絵を書くコードを記述
        //Quaternion qRot;
        //qRot.SetRotationDegY(g_pad[0]->GetLStickXF());
        //qRot.Apply(light.directionLight.direction);
        //qRot.SetRotationDegX(g_pad[0]->GetLStickYF());
        //qRot.Apply(light.directionLight.direction);
        /*
        
        */

        ImGui::Begin("Hello, world!");

        ImGui::DragFloat3("camera position", (float*)&camera_3d->GetPosition());

        ImGui::DragFloat3("light direction", (float*)&light.direction_Light_.direction);
        ImGui::ColorEdit3("light color", (float*)&light.direction_Light_.color); // Edit 3 floats representing a color

        //camera_3d->SetPosition(position[0], position[1], position[2]);

        ImGui::End();


        model.Draw(graphicsEngine, renderContext, *camera_3d);

        system->Render(engine);
        engine->EndFrame();
    }
    delete engine;
    //delete model;
    return 0;
}

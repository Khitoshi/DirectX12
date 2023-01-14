
#include <memory>
#include <iostream>
#include <Windows.h>

#include "System.h"
#include "Camera.h"
#include "Light.h"

using namespace std;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    std::unique_ptr<System>system;
    system = make_unique<System>();
    system->InitGraphicSystem(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("DirectX12 自作ライブラリ"));
    
    //3D用　カメラ
    std::unique_ptr<Camera> camera3d;
    camera3d = make_unique<Camera>();
    //カメラの位置を設定
    camera3d->SetPosition(0.0f, 0.0f, 100.0f);
    //カメラのターゲットを設定
    camera3d->SetTarget(0.0f, 0.0f, 0.0f);

    //ライト
    std::unique_ptr<Light> light;
    light = make_unique<Light>();
    //ライトは右から当たっている
    light->SetDirectionLightDirection(1.0f, -1.0f, -1.0f);
    light->direction_Light_.direction.Normalize();
    //ライトの色は白
    light->SetDirectionLightColor(0.5f, 0.5f, 0.5f);
    //視点の位置を設定する
    light->eye_Position_.Set(camera3d->GetPosition());



    return 0;
}

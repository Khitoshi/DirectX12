#pragma once
#include <Windows.h>
#include <d3dx12.h>
#include <wrl.h>
#include "imgui\imguiSystem.h"

class tkEngine;
class GraphicsEngine;
class Camera;

class System
{
public:
    System();
    ~System();

    /// <summary>
    /// ウィンドウメッセージをディスパッチ
    /// </summary>
    /// <returns>false = ゲーム終了</returns>
    bool DispatchWindowMessage();

    /// <summary>
    /// ウィンドウ 初期化
    /// </summary>
    /// <param name="hInstance"></param>
    /// <param name="hPrevInstance"></param>
    /// <param name="lpCmdLine"></param>
    /// <param name="nCmdShow"></param>
    /// <param name="appName"></param>
    void InitWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow, const TCHAR* appName);

    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="GraphicsEngine"></param>
    void Init(const GraphicsEngine* GraphicsEngine);

    /// <summary>
    /// 更新
    /// </summary>
    /// <param name="engine"></param>
    void Update(const tkEngine* engine);

    /// <summary>
    /// 描画
    /// </summary>
    /// <param name="engine"></param>
    void Render(const tkEngine* engine);

public:
#pragma region Get Method
    /// <summary>
    /// 横 フレームバッファ
    /// </summary>
    /// <returns></returns>
    const UINT GetFrameBufferWidth() { return this->frame_Buffer_width_; }

    /// <summary>
    /// 高さ　フレームバッファ
    /// </summary>
    /// <returns></returns>
    const UINT GetFrameBufferHeight() { return this->frame_Buffer_height_; }

    const HWND GetHWnd() { return this->hWnd_; }
#pragma endregion

private:

    //フレームバッファの幅
    const UINT frame_Buffer_width_;				
    //フレームバッファの高さ
    const UINT frame_Buffer_height_;

    //ウィンドウハンドル。
    HWND hWnd_;

    std::unique_ptr<imguiSystem> imgui_System_;

};

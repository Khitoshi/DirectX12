#pragma once
#include <Windows.h>
class tkEngine;
class GraphicsEngine;
class Camera;
class System
{
public:
    System();
    ~System();

    /*
    void InitGraphicSystem(
        HINSTANCE hInstance, 
        HINSTANCE hPrevInstance, 
        LPWSTR lpCmdLine, 
        int nCmdShow, 
        const TCHAR* appName
        //tkEngine& tk,
        //GraphicsEngine& graphicsEngine,
        //Camera& camera
    );
    */
    /// <summary>
    /// ウィンドウメッセージをディスパッチ
    /// </summary>
    /// <returns>false = ゲーム終了</returns>
    bool DispatchWindowMessage();

    void InitWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow, const TCHAR* appName);
private:

public:
#pragma region Get Method
    const UINT GetFrameBufferWidth() { return this->frame_Buffer_width_; }
    const UINT GetFrameBufferHeight() { return this->frame_Buffer_height_; }
    const HWND GetHWnd() { return this->hWnd_; }
#pragma endregion


private:
    //フレームバッファの幅
    const UINT frame_Buffer_width_ = 1280;				
    //フレームバッファの高さ
    const UINT frame_Buffer_height_ = 720;

    //ウィンドウハンドル。
    HWND hWnd_;
};

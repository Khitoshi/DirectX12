#pragma once
#include <Windows.h>
class Camera;

class System
{
public:
    System();
    ~System();

    void InitGraphicSystem(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow, const TCHAR* appName, Camera& camera);
private:

    void InitWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow, const TCHAR* appName);
public:
private:
    //フレームバッファの幅
    const UINT frame_Buffer_width_ = 1280;				
    //フレームバッファの高さ
    const UINT frame_Buffer_height_ = 720;

    //ウィンドウハンドル。
    HWND hWnd_;
};

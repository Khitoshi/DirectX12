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
    //�t���[���o�b�t�@�̕�
    const UINT frame_Buffer_width_ = 1280;				
    //�t���[���o�b�t�@�̍���
    const UINT frame_Buffer_height_ = 720;

    //�E�B���h�E�n���h���B
    HWND hWnd_;
};

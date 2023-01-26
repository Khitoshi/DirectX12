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
    /// �E�B���h�E���b�Z�[�W���f�B�X�p�b�`
    /// </summary>
    /// <returns>false = �Q�[���I��</returns>
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
    //�t���[���o�b�t�@�̕�
    const UINT frame_Buffer_width_ = 1280;				
    //�t���[���o�b�t�@�̍���
    const UINT frame_Buffer_height_ = 720;

    //�E�B���h�E�n���h���B
    HWND hWnd_;
};

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
    /// �E�B���h�E���b�Z�[�W���f�B�X�p�b�`
    /// </summary>
    /// <returns>false = �Q�[���I��</returns>
    bool DispatchWindowMessage();

    /// <summary>
    /// �E�B���h�E ������
    /// </summary>
    /// <param name="hInstance"></param>
    /// <param name="hPrevInstance"></param>
    /// <param name="lpCmdLine"></param>
    /// <param name="nCmdShow"></param>
    /// <param name="appName"></param>
    void InitWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow, const TCHAR* appName);

    /// <summary>
    /// ������
    /// </summary>
    /// <param name="GraphicsEngine"></param>
    void Init(const GraphicsEngine* GraphicsEngine);

    /// <summary>
    /// �X�V
    /// </summary>
    /// <param name="engine"></param>
    void Update(const tkEngine* engine);

    /// <summary>
    /// �`��
    /// </summary>
    /// <param name="engine"></param>
    void Render(const tkEngine* engine);

public:
#pragma region Get Method
    /// <summary>
    /// �� �t���[���o�b�t�@
    /// </summary>
    /// <returns></returns>
    const UINT GetFrameBufferWidth() { return this->frame_Buffer_width_; }

    /// <summary>
    /// �����@�t���[���o�b�t�@
    /// </summary>
    /// <returns></returns>
    const UINT GetFrameBufferHeight() { return this->frame_Buffer_height_; }

    const HWND GetHWnd() { return this->hWnd_; }
#pragma endregion

private:

    //�t���[���o�b�t�@�̕�
    const UINT frame_Buffer_width_;				
    //�t���[���o�b�t�@�̍���
    const UINT frame_Buffer_height_;

    //�E�B���h�E�n���h���B
    HWND hWnd_;

    std::unique_ptr<imguiSystem> imgui_System_;

};

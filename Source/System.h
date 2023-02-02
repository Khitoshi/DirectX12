#pragma once
#include <Windows.h>
class GraphicsEngine;

extern HWND g_hWnd;

class System
{
public:
    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    System();

    /// <summary>
    /// �f�X�g���N�^
    /// </summary>
    ~System();


    /// <summary>
    /// �Q�[���̏�����
    /// </summary>
    /// <param name="hInstance"></param>
    /// <param name="hPrevInstance"></param>
    /// <param name="lpCmdLine"></param>
    /// <param name="nCmdShow"></param>
    /// <param name="appName"></param>
    void InitGame(
        HINSTANCE hInstance,
        HINSTANCE hPrevInstance, 
        LPWSTR lpCmdLine, 
        int nCmdShow, 
        const TCHAR* appName
    );


private:

    /// <summary>
    /// �E�B���h�E�̏�����
    /// </summary>
    void InitWindow(
        HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPWSTR lpCmdLine,
        int nCmdShow,
        const TCHAR* appName
    );

};


/// <summary>
/// �E�B���h�E���b�Z�[�W���f�B�X�p�b�`�Bfalse���Ԃ��Ă�����A�Q�[���I��
/// </summary>
/// <returns></returns>
bool DispatchWindowMessage();
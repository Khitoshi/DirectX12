#pragma once
#include <Windows.h>
class GraphicsEngine;

extern HWND g_hWnd;

class System
{
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    System();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~System();


    /// <summary>
    /// ゲームの初期化
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
    /// ウィンドウの初期化
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
/// ウィンドウメッセージをディスパッチ。falseが返ってきたら、ゲーム終了
/// </summary>
/// <returns></returns>
bool DispatchWindowMessage();
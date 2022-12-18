#include <Windows.h>
#include <string.h>
#include <vector>
#include <tchar.h>
#include <d3d12.h>
#include <dxgi1_6.h>

#include <iostream>

using namespace std;

void DebugOutputFormatString(const string format, ...)
{
#ifdef _DEBUG
    va_list valist;
    va_start(valist, format);
    vprintf(format.c_str(),valist);
    va_end(valist);
#endif // _DEBUG
}


LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    if (msg == WM_DESTROY)
    {
        //OSに終了を伝える
        PostQuitMessage(0);
        return 0;
    }

    //基底の処理を行う
    return DefWindowProc(hwnd, msg, wparam, lparam);
}

//ウィンドウサイズ
const unsigned int window_width = 1280;
const unsigned int window_height = 729;

IDXGIFactory6* dxgiFactory_ = nullptr;
ID3D12Device* dev_ = nullptr;
ID3D12CommandAllocator* cmdAlloctor_ = nullptr;
ID3D12GraphicsCommandList* cmdList_ = nullptr;
ID3D12CommandQueue* cmdQueue_ = nullptr;
IDXGISwapChain4* swapchain_ = nullptr;


#ifdef _DEBUG
int main() {
#else
#include<Windows.h>
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
#endif
    DebugOutputFormatString("show window.");

    //window class 生成
    WNDCLASSEX w = {};
    {
        
        w.cbSize = sizeof(WNDCLASSEX);
        //コールバック関数指定
        w.lpfnWndProc = (WNDPROC)WindowProcedure;
        //アプリケーションクラス指定
        w.lpszClassName = _T("DirectX12 自作ライブラリ");
        //ハンドル取得
        w.hInstance = GetModuleHandle(0);
    }

    //OS に　アプリケーションの生成を通達する
    RegisterClassEx(&w);



    //ウィンドウサイズを設定する
    RECT wrc{ 0,0,window_width,window_height };
    //ウィンドウサイズを関数を使用して補正する
    AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW,false);

    //window object 生成
    HWND hwnd = CreateWindow(
        w.lpszClassName,
        _T("DX12 自作ライブラリ"),   //クラス名指定  
        WS_OVERLAPPEDWINDOW,        //タイトルバーの文字
        CW_USEDEFAULT,              //タイトルバーと境界線があるウィンドウという設定にする
        CW_USEDEFAULT,              //表示X軸をOSに任せる
        wrc.right - wrc.left,       //表示Y軸をOSに任せる
        wrc.bottom - wrc.top,       //ウィンドウ幅
        nullptr,                    //ウィンドウ高
        nullptr,                    //親ウィンドウハンドル
        w.hInstance,                //呼び出しアプリケーションハンドル
        nullptr                     //追加パラメータ
    );

    HRESULT result = S_OK;
    if (FAILED(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&dxgiFactory_))))
    {
        if (FAILED(CreateDXGIFactory2(0, IID_PPV_ARGS(&dxgiFactory_))))
        {
            return -1;
        }
    }

    vector<IDXGIAdapter*> adapters;
    IDXGIAdapter* tmpAdapter = nullptr;
    for (int i = 0; dxgiFactory_->EnumAdapters(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; i++)
    {
        adapters.push_back(tmpAdapter);
    }

    //アダプターを検索
    for (auto& adapter : adapters)
    {
        DXGI_ADAPTER_DESC adapterdesc = {};
        adapter->GetDesc(&adapterdesc);
        wstring strdesc = adapterdesc.Description;
        //NVIDIAと名前の付いたアダプターの確認
        if (strdesc.find(L"NVIDIA") != string::npos) 
        {
            tmpAdapter = adapter;
            break;
        }
    }


    //DX12周りの初期化
    D3D_FEATURE_LEVEL levels[] = {
        D3D_FEATURE_LEVEL_12_1,
        D3D_FEATURE_LEVEL_12_0,
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0
    };

    D3D_FEATURE_LEVEL featureLevel;
    for (auto& level : levels) 
    {
        //feature Level の検索をする
        if (D3D12CreateDevice(nullptr, level, IID_PPV_ARGS(&dev_)) == S_OK)
        {
            featureLevel = level;
            break;
        }
    }


    //window output
    ShowWindow(hwnd,SW_SHOW);

    MSG msg = {};
    while (true)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        //アプリケーションが終わるときにmessageがWM_QUITになる
        if (msg.message == WM_QUIT)
        {
            break;
        }
    }
    //クラスの登録を解除する
    UnregisterClass(w.lpszClassName,w.hInstance);
}
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
    cout << endl;
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

    //規定の処理を行う
    return DefWindowProc(hwnd, msg, wparam, lparam);
}

//例外を判定し，処理するメソッド
void ExceptionHandlingFormatHresult(const HRESULT result)
{
#ifdef _DEBUG
    try
    {
        if (FAILED(result))
        {
            throw std::runtime_error("HRESULT is ExceptionHandling");
        }
    }
    catch (const runtime_error& e)
    {
        cout << e.what() << endl;
    }
#endif // _DEBUG
}


void EnableDebugLayer()
{
    ID3D12Debug* debugLayer = nullptr;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugLayer))))
    {
        debugLayer->EnableDebugLayer();
        debugLayer->Release();
    }
}

//ウィンドウサイズ
const unsigned int window_width = 1280;
const unsigned int window_height = 729;

IDXGIFactory6*              dxgiFactory_ = nullptr;
ID3D12Device*               dev_ = nullptr;
ID3D12CommandAllocator*     cmdAllocator_ = nullptr;//Command List 本体
ID3D12GraphicsCommandList*  cmdList_ = nullptr;//GPUに対する命令のインターフェース
ID3D12CommandQueue*         cmdQueue_ = nullptr;//command List を用いて貯めた　command を　順次実行していくための変数
IDXGISwapChain4*            swapchain_ = nullptr;
ID3D12DescriptorHeap*       rtvHeaps = nullptr;

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
        //構造体のsizeを設定
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
        //クラス名指定
        w.lpszClassName,            
        //タイトルバーの文字
        _T("DX12 自作ライブラリ"),   
        //タイトルバーと境界線があるウィンドウという設定にする
        WS_OVERLAPPEDWINDOW,        
        //表示X軸をOSに任せる
        CW_USEDEFAULT,              
        //表示Y軸をOSに任せる
        CW_USEDEFAULT,              
        //ウィンドウ幅
        wrc.right - wrc.left,       
        //ウィンドウ高
        wrc.bottom - wrc.top,       
        //親ウィンドウハンドル
        nullptr,                    
        //メニューハンドル
        nullptr,                    
        //呼び出しアプリケーションハンドル
        w.hInstance,                
        //追加パラメータ
        nullptr                     
    );

#ifdef _DEBUG
    EnableDebugLayer();
#endif // _DEBUG


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

    //feature Level の検索をする
    D3D_FEATURE_LEVEL featureLevel;
    for (auto& level : levels) 
    {
        result = D3D12CreateDevice(nullptr, level, IID_PPV_ARGS(&dev_));
        if (SUCCEEDED(result))
        {
            featureLevel = level;
            break;
        }
    }
    ExceptionHandlingFormatHresult(result);

    //コマンド アロケーター　作成
    result = dev_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdAllocator_));
    ExceptionHandlingFormatHresult(result);
    
    //コマンドリスト　作成
    result = dev_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAllocator_, nullptr, IID_PPV_ARGS(&cmdList_));
    ExceptionHandlingFormatHresult(result);

    //コマンド キュー設定&作成
    D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
    {
        //コマンド キューの作成時に使用するフラグを指定
        cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        //1つのGPUを使用する場合は0・複数のGPUを使用する場合はビットを設定する
        cmdQueueDesc.NodeMask = 0;
        //優先度または高優先度を選択
        cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
        //コマンドリストのタイプを設定する
        //今回はGPUが実行可能なコマンドバッファを指定する(ダイレクトコマンドリストはGPUの状態を一切引き継がない)
        cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

        //コマンド キューを作成
        result = dev_->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue_));
        ExceptionHandlingFormatHresult(result);
    }

    //スワップチェーン設定&作成
    DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {};
    {
        //解像度の幅を表す値
        swapchainDesc.Width = window_width;                         
        //解像度の高さを表す値
        swapchainDesc.Height = window_height;                       
        //表示形式を記述
        swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        //全画面表示モードまたはスワップチェーンバックバッファーがステレオかどうかを指定
        swapchainDesc.Stereo = false;                               
        //ピクセルあたりのマルチサンプル数。
        swapchainDesc.SampleDesc.Count = 1;                         
        //画質レベル。品質が高いほど、パフォーマンスは低くなる
        swapchainDesc.SampleDesc.Quality = 0;                       
        //バック バッファーのサーフェスの使用状況と CPU アクセス オプションを示す
        swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
        //スワップ チェーン内のバッファーの数を表す値(フロントバッファを含めて　今回は表・裏で合計2)
        swapchainDesc.BufferCount = 2;
        //バックバッファは伸び縮み可能
        swapchainDesc.Scaling = DXGI_SCALING_STRETCH;               
        //フリップ後は速やかに破棄
        swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;   
        //スワップ チェーン バック バッファーの透過性動作を識別する.
        swapchainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;      
        //ウィンドウ・フルスクリーン切り替え可能
        swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        //スワップチェーン作成
        result = dxgiFactory_->CreateSwapChainForHwnd(
            //dev_ errorになるので注意
            cmdQueue_,
            hwnd,
            &swapchainDesc,
            nullptr,
            nullptr,
            (IDXGISwapChain1**)&swapchain_);
        ExceptionHandlingFormatHresult(result);
    }

    //レンダーターゲットビューを作成
    D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
    
    //ヒープ内のディスクリプタの種類を指定
    heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    //ヒープ内の記述子の数。
    heapDesc.NodeMask = 0;
    //今回は裏・表の合計2
    heapDesc.NumDescriptors = 2;
    //特に指定無し
    heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    
    //ディスクリプタを纏めて扱うDescriptorHeapの作成
    
    result = dev_->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeaps));
    ExceptionHandlingFormatHresult(result);

    //スワップチェインと紐づけ
    DXGI_SWAP_CHAIN_DESC swcDesc = {};
    result = swapchain_->GetDesc(&swcDesc);
    ExceptionHandlingFormatHresult(result);

    //backBuffers(swcDesc.BufferCount);
    vector<ID3D12Resource*> backBuffers(swcDesc.BufferCount);
    D3D12_CPU_DESCRIPTOR_HANDLE handle = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
    for (size_t i = 0; i < swcDesc.BufferCount; i++)
    {
        result = swapchain_->GetBuffer(static_cast<UINT>(i), IID_PPV_ARGS(&backBuffers[i]));
        ExceptionHandlingFormatHresult(result);
        //レンダーターゲットビューを作成
        dev_->CreateRenderTargetView(backBuffers[i], nullptr, handle);
        //ポインタをずらす
        handle.ptr += dev_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    }
    
    ID3D12Fence* fence = nullptr;
    UINT64 fenceVal = 0;
    result = dev_->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));

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
        if (msg.message == WM_QUIT) break;

        //DirectX処理
        //バックバッファのインデックスを取得
        auto bbIdx = swapchain_->GetCurrentBackBufferIndex();

        D3D12_RESOURCE_BARRIER barrierDesc = {};
        barrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrierDesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        barrierDesc.Transition.pResource = backBuffers[bbIdx];
        barrierDesc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
        barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
        cmdList_->ResourceBarrier(1, &barrierDesc);

        //レンダーターゲットを指定
        auto rtvH = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
        rtvH.ptr += static_cast<ULONG_PTR>(bbIdx * dev_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
        cmdList_->OMSetRenderTargets(1, &rtvH, false, nullptr);

        //画面クリア
        float clearColor[] = { 1.0f,0.0f,0.0f,1.0f };
        cmdList_->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);

        barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
        barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
        cmdList_->ResourceBarrier(1, &barrierDesc);

        //命令を閉じる
        cmdList_->Close();

        //コマンドリスト　実行
        ID3D12CommandList* cmdLists[] = { cmdList_ };
        cmdQueue_->ExecuteCommandLists(1,cmdLists);
        //待機
        cmdQueue_->Signal(fence, ++fenceVal);

        if (fence->GetCompletedValue() != fenceVal)
        {
            auto event = CreateEvent(nullptr, false, false, nullptr);
            fence->SetEventOnCompletion(fenceVal, event);
            WaitForSingleObject(event, INFINITE);
            CloseHandle(event);
        }

        //キューのクリア
        cmdAllocator_->Reset();
        //コマンドリストを貯める準備
        cmdList_->Reset(cmdAllocator_, nullptr);

        //フリップ
        //第一引数は待ちフレーム数
        swapchain_->Present(1, 0);
    }
    //クラスの登録を解除する
    UnregisterClass(w.lpszClassName,w.hInstance);

    return 0;
}
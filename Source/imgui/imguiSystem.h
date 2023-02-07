#pragma once
#include <d3dx12.h>

class GraphicsEngine;

class imguiSystem
{
public:

    /// <summary>
    /// コンストラクタ
    /// </summary>
    imguiSystem();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~imguiSystem();

    static LRESULT WindowProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    /// <summary>
    /// imgui 初期化
    /// </summary>
    /// <param name="graphicsEngine"></param>
    /// <param name="hWnd"></param>
    void Init(const GraphicsEngine* graphicsEngine, HWND hWnd);

    /// <summary>
    /// 更新
    /// </summary>
    void Update();

    /// <summary>
    /// 描画
    /// </summary>
    /// <param name="graphicsEngine"></param>
    void Drow(GraphicsEngine* graphicsEngine);

private:
    /// <summary>
    /// imgui用　ディスクリプタヒープの生成
    /// </summary>
    /// <param name="device"></param>
    void CreateDescriptorHeapForImgui(ID3D12Device5* device);

private:
    //imgui用ディスクリプタヒープ
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> imgui_Heap_;

};


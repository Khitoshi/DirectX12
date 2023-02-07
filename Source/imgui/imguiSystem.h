#pragma once
#include <d3dx12.h>

class GraphicsEngine;

class imguiSystem
{
public:

    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    imguiSystem();

    /// <summary>
    /// �f�X�g���N�^
    /// </summary>
    ~imguiSystem();

    static LRESULT WindowProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    /// <summary>
    /// imgui ������
    /// </summary>
    /// <param name="graphicsEngine"></param>
    /// <param name="hWnd"></param>
    void Init(const GraphicsEngine* graphicsEngine, HWND hWnd);

    /// <summary>
    /// �X�V
    /// </summary>
    void Update();

    /// <summary>
    /// �`��
    /// </summary>
    /// <param name="graphicsEngine"></param>
    void Drow(GraphicsEngine* graphicsEngine);

private:
    /// <summary>
    /// imgui�p�@�f�B�X�N���v�^�q�[�v�̐���
    /// </summary>
    /// <param name="device"></param>
    void CreateDescriptorHeapForImgui(ID3D12Device5* device);

private:
    //imgui�p�f�B�X�N���v�^�q�[�v
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> imgui_Heap_;

};


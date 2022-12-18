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
        //OS�ɏI����`����
        PostQuitMessage(0);
        return 0;
    }

    //���̏������s��
    return DefWindowProc(hwnd, msg, wparam, lparam);
}

//�E�B���h�E�T�C�Y
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

    //window class ����
    WNDCLASSEX w = {};
    {
        
        w.cbSize = sizeof(WNDCLASSEX);
        //�R�[���o�b�N�֐��w��
        w.lpfnWndProc = (WNDPROC)WindowProcedure;
        //�A�v���P�[�V�����N���X�w��
        w.lpszClassName = _T("DirectX12 ���색�C�u����");
        //�n���h���擾
        w.hInstance = GetModuleHandle(0);
    }

    //OS �Ɂ@�A�v���P�[�V�����̐�����ʒB����
    RegisterClassEx(&w);



    //�E�B���h�E�T�C�Y��ݒ肷��
    RECT wrc{ 0,0,window_width,window_height };
    //�E�B���h�E�T�C�Y���֐����g�p���ĕ␳����
    AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW,false);

    //window object ����
    HWND hwnd = CreateWindow(
        w.lpszClassName,
        _T("DX12 ���색�C�u����"),   //�N���X���w��  
        WS_OVERLAPPEDWINDOW,        //�^�C�g���o�[�̕���
        CW_USEDEFAULT,              //�^�C�g���o�[�Ƌ��E��������E�B���h�E�Ƃ����ݒ�ɂ���
        CW_USEDEFAULT,              //�\��X����OS�ɔC����
        wrc.right - wrc.left,       //�\��Y����OS�ɔC����
        wrc.bottom - wrc.top,       //�E�B���h�E��
        nullptr,                    //�E�B���h�E��
        nullptr,                    //�e�E�B���h�E�n���h��
        w.hInstance,                //�Ăяo���A�v���P�[�V�����n���h��
        nullptr                     //�ǉ��p�����[�^
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

    //�A�_�v�^�[������
    for (auto& adapter : adapters)
    {
        DXGI_ADAPTER_DESC adapterdesc = {};
        adapter->GetDesc(&adapterdesc);
        wstring strdesc = adapterdesc.Description;
        //NVIDIA�Ɩ��O�̕t�����A�_�v�^�[�̊m�F
        if (strdesc.find(L"NVIDIA") != string::npos) 
        {
            tmpAdapter = adapter;
            break;
        }
    }


    //DX12����̏�����
    D3D_FEATURE_LEVEL levels[] = {
        D3D_FEATURE_LEVEL_12_1,
        D3D_FEATURE_LEVEL_12_0,
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0
    };

    D3D_FEATURE_LEVEL featureLevel;
    for (auto& level : levels) 
    {
        //feature Level �̌���������
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

        //�A�v���P�[�V�������I���Ƃ���message��WM_QUIT�ɂȂ�
        if (msg.message == WM_QUIT)
        {
            break;
        }
    }
    //�N���X�̓o�^����������
    UnregisterClass(w.lpszClassName,w.hInstance);
}
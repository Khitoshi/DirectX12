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
        //OS�ɏI����`����
        PostQuitMessage(0);
        return 0;
    }

    //�K��̏������s��
    return DefWindowProc(hwnd, msg, wparam, lparam);
}

//��O�𔻒肵�C�������郁�\�b�h
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

//�E�B���h�E�T�C�Y
const unsigned int window_width = 1280;
const unsigned int window_height = 729;

IDXGIFactory6*              dxgiFactory_ = nullptr;
ID3D12Device*               dev_ = nullptr;
ID3D12CommandAllocator*     cmdAllocator_ = nullptr;//Command List �{��
ID3D12GraphicsCommandList*  cmdList_ = nullptr;//GPU�ɑ΂��閽�߂̃C���^�[�t�F�[�X
ID3D12CommandQueue*         cmdQueue_ = nullptr;//command List ��p���Ē��߂��@command ���@�������s���Ă������߂̕ϐ�
IDXGISwapChain4*            swapchain_ = nullptr;
ID3D12DescriptorHeap*       rtvHeaps = nullptr;

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
        //�\���̂�size��ݒ�
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
        //�N���X���w��
        w.lpszClassName,            
        //�^�C�g���o�[�̕���
        _T("DX12 ���색�C�u����"),   
        //�^�C�g���o�[�Ƌ��E��������E�B���h�E�Ƃ����ݒ�ɂ���
        WS_OVERLAPPEDWINDOW,        
        //�\��X����OS�ɔC����
        CW_USEDEFAULT,              
        //�\��Y����OS�ɔC����
        CW_USEDEFAULT,              
        //�E�B���h�E��
        wrc.right - wrc.left,       
        //�E�B���h�E��
        wrc.bottom - wrc.top,       
        //�e�E�B���h�E�n���h��
        nullptr,                    
        //���j���[�n���h��
        nullptr,                    
        //�Ăяo���A�v���P�[�V�����n���h��
        w.hInstance,                
        //�ǉ��p�����[�^
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

    //feature Level �̌���������
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

    //�R�}���h �A���P�[�^�[�@�쐬
    result = dev_->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdAllocator_));
    ExceptionHandlingFormatHresult(result);
    
    //�R�}���h���X�g�@�쐬
    result = dev_->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAllocator_, nullptr, IID_PPV_ARGS(&cmdList_));
    ExceptionHandlingFormatHresult(result);

    //�R�}���h �L���[�ݒ�&�쐬
    D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
    {
        //�R�}���h �L���[�̍쐬���Ɏg�p����t���O���w��
        cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        //1��GPU���g�p����ꍇ��0�E������GPU���g�p����ꍇ�̓r�b�g��ݒ肷��
        cmdQueueDesc.NodeMask = 0;
        //�D��x�܂��͍��D��x��I��
        cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
        //�R�}���h���X�g�̃^�C�v��ݒ肷��
        //�����GPU�����s�\�ȃR�}���h�o�b�t�@���w�肷��(�_�C���N�g�R�}���h���X�g��GPU�̏�Ԃ���؈����p���Ȃ�)
        cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

        //�R�}���h �L���[���쐬
        result = dev_->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&cmdQueue_));
        ExceptionHandlingFormatHresult(result);
    }

    //�X���b�v�`�F�[���ݒ�&�쐬
    DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {};
    {
        //�𑜓x�̕���\���l
        swapchainDesc.Width = window_width;                         
        //�𑜓x�̍�����\���l
        swapchainDesc.Height = window_height;                       
        //�\���`�����L�q
        swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        //�S��ʕ\�����[�h�܂��̓X���b�v�`�F�[���o�b�N�o�b�t�@�[���X�e���I���ǂ������w��
        swapchainDesc.Stereo = false;                               
        //�s�N�Z��������̃}���`�T���v�����B
        swapchainDesc.SampleDesc.Count = 1;                         
        //�掿���x���B�i���������قǁA�p�t�H�[�}���X�͒Ⴍ�Ȃ�
        swapchainDesc.SampleDesc.Quality = 0;                       
        //�o�b�N �o�b�t�@�[�̃T�[�t�F�X�̎g�p�󋵂� CPU �A�N�Z�X �I�v�V����������
        swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
        //�X���b�v �`�F�[�����̃o�b�t�@�[�̐���\���l(�t�����g�o�b�t�@���܂߂ā@����͕\�E���ō��v2)
        swapchainDesc.BufferCount = 2;
        //�o�b�N�o�b�t�@�͐L�яk�݉\
        swapchainDesc.Scaling = DXGI_SCALING_STRETCH;               
        //�t���b�v��͑��₩�ɔj��
        swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;   
        //�X���b�v �`�F�[�� �o�b�N �o�b�t�@�[�̓��ߐ���������ʂ���.
        swapchainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;      
        //�E�B���h�E�E�t���X�N���[���؂�ւ��\
        swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        //�X���b�v�`�F�[���쐬
        result = dxgiFactory_->CreateSwapChainForHwnd(
            //dev_ error�ɂȂ�̂Œ���
            cmdQueue_,
            hwnd,
            &swapchainDesc,
            nullptr,
            nullptr,
            (IDXGISwapChain1**)&swapchain_);
        ExceptionHandlingFormatHresult(result);
    }

    //�����_�[�^�[�Q�b�g�r���[���쐬
    D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
    
    //�q�[�v���̃f�B�X�N���v�^�̎�ނ��w��
    heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    //�q�[�v���̋L�q�q�̐��B
    heapDesc.NodeMask = 0;
    //����͗��E�\�̍��v2
    heapDesc.NumDescriptors = 2;
    //���Ɏw�薳��
    heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    
    //�f�B�X�N���v�^��Z�߂Ĉ���DescriptorHeap�̍쐬
    
    result = dev_->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeaps));
    ExceptionHandlingFormatHresult(result);

    //�X���b�v�`�F�C���ƕR�Â�
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
        //�����_�[�^�[�Q�b�g�r���[���쐬
        dev_->CreateRenderTargetView(backBuffers[i], nullptr, handle);
        //�|�C���^�����炷
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

        //�A�v���P�[�V�������I���Ƃ���message��WM_QUIT�ɂȂ�
        if (msg.message == WM_QUIT) break;

        //DirectX����
        //�o�b�N�o�b�t�@�̃C���f�b�N�X���擾
        auto bbIdx = swapchain_->GetCurrentBackBufferIndex();

        D3D12_RESOURCE_BARRIER barrierDesc = {};
        barrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrierDesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        barrierDesc.Transition.pResource = backBuffers[bbIdx];
        barrierDesc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
        barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
        cmdList_->ResourceBarrier(1, &barrierDesc);

        //�����_�[�^�[�Q�b�g���w��
        auto rtvH = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
        rtvH.ptr += static_cast<ULONG_PTR>(bbIdx * dev_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
        cmdList_->OMSetRenderTargets(1, &rtvH, false, nullptr);

        //��ʃN���A
        float clearColor[] = { 1.0f,0.0f,0.0f,1.0f };
        cmdList_->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);

        barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
        barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
        cmdList_->ResourceBarrier(1, &barrierDesc);

        //���߂����
        cmdList_->Close();

        //�R�}���h���X�g�@���s
        ID3D12CommandList* cmdLists[] = { cmdList_ };
        cmdQueue_->ExecuteCommandLists(1,cmdLists);
        //�ҋ@
        cmdQueue_->Signal(fence, ++fenceVal);

        if (fence->GetCompletedValue() != fenceVal)
        {
            auto event = CreateEvent(nullptr, false, false, nullptr);
            fence->SetEventOnCompletion(fenceVal, event);
            WaitForSingleObject(event, INFINITE);
            CloseHandle(event);
        }

        //�L���[�̃N���A
        cmdAllocator_->Reset();
        //�R�}���h���X�g�𒙂߂鏀��
        cmdList_->Reset(cmdAllocator_, nullptr);

        //�t���b�v
        //�������͑҂��t���[����
        swapchain_->Present(1, 0);
    }
    //�N���X�̓o�^����������
    UnregisterClass(w.lpszClassName,w.hInstance);

    return 0;
}
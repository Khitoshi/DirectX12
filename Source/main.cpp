#include <Windows.h>
#include <string.h>
#include <vector>
#include <tchar.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <d3dcompiler.inl>

#include <DirectXTex.h>

#include "d3dx12.h"
#include <iostream>
#include <fbxsdk.h>
using namespace std;
using namespace DirectX;


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

struct Vertex
{
    XMFLOAT3 position;//xyz���W
    XMFLOAT2 uv;//uv���
};

struct TexRGBA
{
    unsigned char R, G, B, A;
};


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

    //OS�ɃA�v���P�[�V�����̐�����ʒB����
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
    
    //���p�\�ȑS�Ẵf�B�X�v���C �T�u�V�X�e�� (1 �ȏ�� GPU�ADAC�A�r�f�I ���������܂�)��adapters�ɐς�
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
    result = dev_->CreateCommandAllocator(
        D3D12_COMMAND_LIST_TYPE_DIRECT, 
        IID_PPV_ARGS(&cmdAllocator_)
    );
    ExceptionHandlingFormatHresult(result);
    
    //�R�}���h���X�g�@�쐬
    result = dev_->CreateCommandList(
        0, 
        D3D12_COMMAND_LIST_TYPE_DIRECT, 
        cmdAllocator_, 
        nullptr, 
        IID_PPV_ARGS(&cmdList_)
    );
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
    
    //fence�̍쐬
    //fence: CPU��1�ȏ��GPU�̓����Ɏg�p�����I�u�W�F�N�g��\���Ă���
    ID3D12Fence* fence = nullptr;
    UINT64 fenceVal = 0;
    result = dev_->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));

    //window output
    ShowWindow(hwnd,SW_SHOW);

    {//���f���ǂݍ���
        /*
        struct FBXHeader
        {
            float version;//
            char model_name[20];//���f����
            char comment[256];//���f���R�����g
        };

        FILE* fp;
        char signature[3];
        FBXHeader fbxHeader;

        fread(signature, sizeof(signature), 1, fp);
        fread(&fbxHeader, sizeof(fbxHeader), 1, fp);

        unsigned int vertNum;//���_��
        fread(&vertNum, sizeof(vertNum), 1, fp);
        */

        //error���N����ꍇ��https://yttm-work.jp/model_render/model_render_0006.html#head_line_01��
        //�udll�t�@�C���̐ݒ�v���m�F���Đݒ�
        fbxsdk::FbxManager* fbx_manager = fbxsdk::FbxManager::Create();

        //�G���[�`�F�b�N
        if (fbx_manager == NULL)
        {
            return -1;
        }
        
        //fbx�t�@�C���� �w��
        const string filePath("./Model/YBot.fbx");

        //���o�͐ݒ���쐬
        FbxIOSettings* ios = FbxIOSettings::Create(fbx_manager, IOSROOT);

        //�}�l�[�W���[�ɓ��o�͐ݒ���Z�b�g
        fbx_manager->SetIOSettings(ios);

        //FBX�C���|�[�^��������
        FbxImporter* importer = FbxImporter::Create(fbx_manager, "");

        //FBX�t�@�C���̓ǂݍ���
        if (!importer->Initialize(filePath.c_str(), -1, fbx_manager->GetIOSettings()))
        {
            //���s�����ꍇ,fbxManager��j�����ďI��
            fbx_manager->Destroy();
            return -1;
        }




        //fbxManager�j��
        fbx_manager->Destroy();
    }


    //�����ɍ��W������(����:���W�͎��v���ɂ���)
    Vertex vertices[] = {
        {{-0.4f,-0.7f,0.0f} , {0.0f,1.0f}  },//����
        { {-0.4f,0.7f,0.0f} ,  {0.0f,0.0f}  },//����
        { {0.4f,-0.7f,0.0f} ,  {1.0f,1.0f}  },//�E��
        { {0.4f,0.7f,0.0f} ,   {1.0f,0.0f}  },//�E��
    };

    //���_�o�b�t�@�[�ݒ�&����
    D3D12_VERTEX_BUFFER_VIEW vbView = {};
    D3D12_INDEX_BUFFER_VIEW ibView = {};
    D3D12_HEAP_PROPERTIES heapprop = {};
    {
        //���_�q�[�v�̐ݒ�
        //�q�[�v�̎�ނ��w��C�����CPU����A�N�Z�X�ł���悤�ɂ��Ă���
        heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;
        //CPU �y�[�W �v���p�e�B���w��,����͓��ɍl���Ȃ�
        heapprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        //�q�[�v�̃����� �v�[�����w��C����͓��ɍl���Ȃ�
        heapprop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

        //���_�o�b�t�@�[�ݒ�
        D3D12_RESOURCE_DESC resourceDesc = {};
        //�g�p���Ă��郊�\�[�X�̃^�C�v��ݒ� 
        resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        //���ł܂��Ȃ��̂őS���_(sizeof)�Ƃ���
        resourceDesc.Width = sizeof(vertices);
        //���ł܂��Ȃ��̂�1
        resourceDesc.Height = 1;
        //�񎟌��Ȃ̂�1
        resourceDesc.DepthOrArraySize = 1;
        //
        resourceDesc.MipLevels = 1;
        //�摜�ł͂Ȃ��̂� UNKNOWN
        resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
        //�s�N�Z��������̃}���`�T���v�����Ȃ̂�1
        resourceDesc.SampleDesc.Count = 1;
        //�I�v�V�����̎w��͓��ɂȂ��̂�NONE
        resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
        //�e�N�X�`���̃f�[�^���s�D�揇�Ŋi�[����Ă���̂�
        resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

        //���_�o�b�t�@�[����
        ID3D12Resource* vertBuff = nullptr;
        result = dev_->CreateCommittedResource(
            &heapprop,
            D3D12_HEAP_FLAG_NONE,
            &resourceDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&vertBuff)
        );
        ExceptionHandlingFormatHresult(result);


        //���_���̃R�s�[(map����)
        Vertex* vertMap = nullptr;
        result = vertBuff->Map(0, nullptr, (void**)&vertMap);
        ExceptionHandlingFormatHresult(result);
        copy(begin(vertices), end(vertices), vertMap);
        //map������
        vertBuff->Unmap(0, nullptr);

        //�o�b�t�@�̉��z�A�h���X
        vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
        //�S�o�C�g��
        vbView.SizeInBytes = sizeof(vertices);
        //1���_������̃o�C�g��
        vbView.StrideInBytes = sizeof(vertices[0]);

        unsigned short indices[] = { 0,1,2, 2,1,3 };

        ID3D12Resource* idxBuff = nullptr;
        //�ݒ�́A�o�b�t�@�̃T�C�Y�ȊO���_�o�b�t�@�̐ݒ���g���܂킵��
        result = dev_->CreateCommittedResource(
            &heapprop,
            D3D12_HEAP_FLAG_NONE,
            &resourceDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&idxBuff));

        //������o�b�t�@�ɃC���f�b�N�X�f�[�^���R�s�[
        unsigned short* mappedIdx = nullptr;
        idxBuff->Map(0, nullptr, (void**)&mappedIdx);
        copy(begin(indices), end(indices), mappedIdx);
        idxBuff->Unmap(0, nullptr);

        //�C���f�b�N�X�o�b�t�@�r���[���쐬
        ibView.BufferLocation = idxBuff->GetGPUVirtualAddress();
        ibView.Format = DXGI_FORMAT_R16_UINT;
        ibView.SizeInBytes = sizeof(indices);
    }


    //�V�F�[�_�[�ǂݍ���
    ID3DBlob* vsBlob = nullptr;
    ID3DBlob* psBlob = nullptr;

    ID3DBlob* errorBlob = nullptr;
    //vs �ǂݍ���
    {
        result = D3DCompileFromFile(
            L"Shader/BasicVertexShader.hlsl",
            nullptr,
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            "BasicVS",
            "vs_5_0",
            D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
            0,
            &vsBlob,
            &errorBlob
        );
        //error check
        if (FAILED(result))
        {
            if (result == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
            {
                ::OutputDebugStringA("�t�@�C������������܂���");
            }
            else
            {
                string errorStr;
                errorStr.resize(errorBlob->GetBufferSize());
                copy_n(
                    (char*)errorBlob->GetBufferPointer(),
                    errorBlob->GetBufferSize(),
                    errorStr.begin()
                );
                OutputDebugStringA(errorStr.c_str());
            }
            exit(1);
        }
    }

    //ps �ǂݍ���
    {
        result = D3DCompileFromFile(
            L"Shader/BasicPixelShader.hlsl",
            nullptr,
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            "BasicPS",
            "ps_5_0",
            D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
            0,
            &psBlob,
            &errorBlob
        );
        //error check
        if (FAILED(result))
        {
            if (result == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
            {
                ::OutputDebugStringA("�t�@�C������������܂���");
            }
            else
            {
                string errorStr;
                errorStr.resize(errorBlob->GetBufferSize());
                copy_n(
                    (char*)errorBlob->GetBufferPointer(),
                    errorBlob->GetBufferSize(),
                    errorStr.begin()
                );
                OutputDebugStringA(errorStr.c_str());
            }
            exit(1);
        }
    }


    ID3D12PipelineState* pipelinestate = nullptr;
    D3D12_VIEWPORT viewport = {};
    D3D12_RECT scissorrect = {};
    ID3D12RootSignature* rootsignature = nullptr;
    {
        D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
            {//�ʒu���
                    "POSITION",
                    0,
                    DXGI_FORMAT_R32G32B32_FLOAT,
                    0,
                    D3D12_APPEND_ALIGNED_ELEMENT,
                    D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
                    0
            },
            {//uv���
                    "TEXCOORD",
                    0,
                    DXGI_FORMAT_R32G32_FLOAT,
                    0,
                    D3D12_APPEND_ALIGNED_ELEMENT,
                    D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
                    0
            }
        };

        D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline = {};

        gpipeline.pRootSignature = nullptr;
        gpipeline.VS.pShaderBytecode = vsBlob->GetBufferPointer();
        gpipeline.VS.BytecodeLength = vsBlob->GetBufferSize();
        gpipeline.PS.pShaderBytecode = psBlob->GetBufferPointer();
        gpipeline.PS.BytecodeLength = psBlob->GetBufferSize();

        gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
        gpipeline.BlendState.AlphaToCoverageEnable = false;
        gpipeline.BlendState.IndependentBlendEnable = false;

        D3D12_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc = {};

        //���Z�C��Z��a�u�����h�͎g�p���Ȃ�
        renderTargetBlendDesc.BlendEnable = false;
        renderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
        renderTargetBlendDesc.LogicOpEnable = false;
        gpipeline.BlendState.RenderTarget[0] = renderTargetBlendDesc;

        //�A���`�F���͎g��Ȃ��̂�false
        gpipeline.RasterizerState.MultisampleEnable = false;
        //�J�����O���Ȃ��̂�NONE
        gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
        //���g��h��Ԃ�
        gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
        //�k�x�����̃N���b�s���O�͗L���ɂ���
        gpipeline.RasterizerState.DepthClipEnable = true;

        gpipeline.RasterizerState.FrontCounterClockwise = false;
        gpipeline.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
        gpipeline.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
        gpipeline.RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
        gpipeline.RasterizerState.AntialiasedLineEnable = false;
        gpipeline.RasterizerState.ForcedSampleCount = 0;
        gpipeline.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
    
        gpipeline.DepthStencilState.DepthEnable = false;
        gpipeline.DepthStencilState.StencilEnable = false;

        //���C�A�E�g�擪�A�h���X
        gpipeline.InputLayout.pInputElementDescs = inputLayout;
        //���C�A�E�g�z��
        gpipeline.InputLayout.NumElements = _countof(inputLayout);

        //�X�g���b�v���̃J�b�g�Ȃ�
        gpipeline.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
        //�O�p�`�ō\��
        gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

        //���͂P�̂�
        gpipeline.NumRenderTargets = 1;

        //0�`1�ɐ��K�����ꂽRGBA
        gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

        //�T���v�����O��1�s�N�Z���ɂ��P
        gpipeline.SampleDesc.Count = 1;
        //�N�I���e�B�͍Œ�
        gpipeline.SampleDesc.Quality = 0;

        D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
        {
            rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

            D3D12_DESCRIPTOR_RANGE descTblRange[2] = {};
            
            //�e�N�X�`���p���W�X�^0��
            descTblRange[0].NumDescriptors = 1;//�e�N�X�`���ЂƂ�
            descTblRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;//��ʂ̓e�N�X�`��
            descTblRange[0].BaseShaderRegister = 0;//0�ԃX���b�g����
            descTblRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

            //�萔�p���W�X�^0��
            descTblRange[1].NumDescriptors = 1;//�萔�͂ЂƂ�
            descTblRange[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
            descTblRange[1].BaseShaderRegister = 0;//0�ԃX���b�g����
            descTblRange[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

            D3D12_ROOT_PARAMETER rootparam = {};
            rootparam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            rootparam.DescriptorTable.pDescriptorRanges = &descTblRange[0];//�f�X�N���v�^�����W�̃A�h���X
            rootparam.DescriptorTable.NumDescriptorRanges = 2;//�f�X�N���v�^�����W��
            rootparam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//�s�N�Z���V�F�[�_���猩����

            rootSignatureDesc.pParameters = &rootparam;//���[�g�p�����[�^�̐擪�A�h���X
            rootSignatureDesc.NumParameters = 1;//���[�g�p�����[�^��

            D3D12_STATIC_SAMPLER_DESC samplerDesc = {};
            samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//���J��Ԃ�
            samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//�c�J��Ԃ�
            samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//���s�J��Ԃ�
            samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;//�{�[�_�[�̎��͍�
            samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;//��Ԃ��Ȃ�(�j�A���X�g�l�C�o�[)
            samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;//�~�b�v�}�b�v�ő�l
            samplerDesc.MinLOD = 0.0f;//�~�b�v�}�b�v�ŏ��l
            samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;//�I�[�o�[�T���v�����O�̍ۃ��T���v�����O���Ȃ��H
            samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//�s�N�Z���V�F�[�_����̂݉�

            rootSignatureDesc.pStaticSamplers = &samplerDesc;
            rootSignatureDesc.NumStaticSamplers = 1;
        }

        ID3DBlob* rootSigBlob = nullptr;
        result = D3D12SerializeRootSignature(
            &rootSignatureDesc, 
            D3D_ROOT_SIGNATURE_VERSION_1_0, 
            &rootSigBlob, 
            &errorBlob);
        ExceptionHandlingFormatHresult(result);

        
        result = dev_->CreateRootSignature(
            0, 
            rootSigBlob->GetBufferPointer(), 
            rootSigBlob->GetBufferSize(), 
            IID_PPV_ARGS(&rootsignature)
        );
        ExceptionHandlingFormatHresult(result);
        rootSigBlob->Release();

        gpipeline.pRootSignature = rootsignature;
        
        result = dev_->CreateGraphicsPipelineState(
            &gpipeline, 
            IID_PPV_ARGS(&pipelinestate)
        );
        ExceptionHandlingFormatHresult(result);

        
        viewport.Width = window_width;//�o�͐�̕�(�s�N�Z����)
        viewport.Height = window_height;//�o�͐�̍���(�s�N�Z����)
        viewport.TopLeftX = 0;//�o�͐�̍�����WX
        viewport.TopLeftY = 0;//�o�͐�̍�����WY
        viewport.MaxDepth = 1.0f;//�[�x�ő�l
        viewport.MinDepth = 0.0f;//�[�x�ŏ��l


        scissorrect.top = 0;//�؂蔲������W
        scissorrect.left = 0;//�؂蔲�������W
        scissorrect.right = scissorrect.left + window_width;//�؂蔲���E���W
        scissorrect.bottom = scissorrect.top + window_height;//�؂蔲�������W

    }

    TexMetadata metadata = {};
    ScratchImage scratchImg = {};
    result = LoadFromWICFile(L"Asset/Texture/textest.png", WIC_FLAGS_NONE, &metadata, scratchImg);
    auto img = scratchImg.GetImage(0, 0, 0);//���f�[�^���o

    //�e�N�X�`���o�b�t�@�[�쐬
    D3D12_HEAP_PROPERTIES texHeapProp = {};
    texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
    texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
    texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
    texHeapProp.CreationNodeMask = 0;
    texHeapProp.VisibleNodeMask = 0;

    D3D12_RESOURCE_DESC resDesc = {};
    //DXGI_FORMAT_R8G8B8A8_UNORM;//RGBA�t�H�[�}�b�g
    resDesc.Format = metadata.format;
    //��
    resDesc.Width = static_cast<UINT>(metadata.width);
    //����
    resDesc.Height = static_cast<UINT>(metadata.height);
    //2D�Ŕz��ł��Ȃ��̂łP
    resDesc.DepthOrArraySize = static_cast<uint16_t>(metadata.arraySize);
    //�ʏ�e�N�X�`���Ȃ̂ŃA���`�F�����Ȃ�
    resDesc.SampleDesc.Count = 1;
    resDesc.SampleDesc.Quality = 0;//
    //�~�b�v�}�b�v���Ȃ��̂Ń~�b�v���͂P��
    resDesc.MipLevels = static_cast<uint16_t>(metadata.mipLevels);
    //2D�e�N�X�`���p
    resDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadata.dimension);
    //���C�A�E�g�ɂ��Ă͌��肵�Ȃ�
    resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    //�Ƃ��Ƀt���O�Ȃ�
    resDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

    ID3D12Resource* texbuff = nullptr;
    result = dev_->CreateCommittedResource(
        &texHeapProp,
        D3D12_HEAP_FLAG_NONE,
        &resDesc,
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
        nullptr,
        IID_PPV_ARGS(&texbuff)
    );
    ExceptionHandlingFormatHresult(result);

    //�e�N�X�`���o�b�t�@�[�̓]��
    result = texbuff->WriteToSubresource(
        0,
        nullptr,//�S�̈�փR�s�[
        img->pixels,//���f�[�^�A�h���X
        static_cast<UINT>(img->rowPitch),//1���C���T�C�Y
        static_cast<UINT>(img->slicePitch)//�S�T�C�Y
    );
    ExceptionHandlingFormatHresult(result);


    //GPU�ɏ���n�����߂̒萔�o�b�t�@�쐬
    ID3D12Resource* constBuff = nullptr;
    

    XMFLOAT3 eye(0,0,-5);
    XMFLOAT3 target(0,0,0);
    XMFLOAT3 up(0,1,0);
    auto viewMat = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
    auto perspectMat = XMMatrixPerspectiveFovLH(
        XM_PIDIV2,
        static_cast<float>(window_width) / static_cast<float>(window_height),
        1.0f,
        10.0f
    );

    //�萔�o�b�t�@����
    heapprop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    resDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(XMMATRIX) + 0xff) & ~0xff);
    result = dev_->CreateCommittedResource(
        &heapprop,
        D3D12_HEAP_FLAG_NONE,
        &resDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&constBuff)
    );
    ExceptionHandlingFormatHresult(result);

    
    //�}�b�v�Œ萔�̃R�s�[
    XMMATRIX* mapMatrix;
    auto worldMat = XMMatrixRotationY(XM_PIDIV4);
    result = constBuff->Map(0, nullptr, (void**)&mapMatrix);
    ExceptionHandlingFormatHresult(result);
    *mapMatrix = worldMat * viewMat * perspectMat;

    ID3D12DescriptorHeap* basicDescHeap = nullptr;
    //�f�B�X�N���v�^�q�[�v����
    D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
    //�V�F�[�_�[���猩����悤��
    descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    //�}�X�N��0
    descHeapDesc.NodeMask = 0;
    //SRV1��CBV1��
    descHeapDesc.NumDescriptors = 2;
    //�f�B�X�N���v�^�q�[�v�̎��
    descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    result = dev_->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&basicDescHeap));
    ExceptionHandlingFormatHresult(result);
    

    //�V�F�[�_�[���\�[�X�r���[�쐬
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    //RGBA(0.0f ~ 1.0f�ɐ��K��)
    srvDesc.Format = metadata.format;
    //
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    //2D�e�N�X�`���Ȃ̂�
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    //�~�b�v�}�b�v�͍쐬���Ȃ��̂�
    srvDesc.Texture2D.MipLevels = 1;

    auto basicHeapHandle = basicDescHeap->GetCPUDescriptorHandleForHeapStart();

    dev_->CreateShaderResourceView(
        texbuff,
        &srvDesc,
        basicDescHeap->GetCPUDescriptorHandleForHeapStart()
    );

    basicHeapHandle.ptr += dev_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
    cbvDesc.BufferLocation = constBuff->GetGPUVirtualAddress();
    cbvDesc.SizeInBytes = static_cast<UINT>(constBuff->GetDesc().Width);

    //�萔�o�b�t�@�r���[�쐬
    dev_->CreateConstantBufferView(&cbvDesc, basicHeapHandle);

    MSG msg = {};
    unsigned int frame = 0;
    float angle = 0.0f;
    while (true)
    {
        //�A�v���P�[�V�����̏�Ԃ��X�V
        {
            if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }

            //�A�v���P�[�V�������I���Ƃ���message��WM_QUIT�ɂȂ�
            if (msg.message == WM_QUIT) break;
        }

        angle += 0.1f;
        worldMat = XMMatrixRotationY(angle);
        *mapMatrix = worldMat * viewMat * perspectMat;

        //DirectX����
        //�o�b�N�o�b�t�@�̃C���f�b�N�X���擾
        auto bbIdx = swapchain_->GetCurrentBackBufferIndex();

        //���\�[�X�g�p�̑J�ڂ�GPU�ɒʒB
        //���\�[�X�o���A�ݒ�&���s
        D3D12_RESOURCE_BARRIER barrierDesc = {};
        {
            //�o���A�̎w��,����͏�ԑJ�ڂȂ̂�TRANSITION
            barrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
            //�J�n�ƏI�����w��C�����NONE(���Ɏw��Ȃ�)
            barrierDesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
            //���\�[�X�̃A�h���X
            barrierDesc.Transition.pResource = backBuffers[bbIdx];
            //�T�u���\�[�X�ԍ�
            barrierDesc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
            //���̏��(���O��PRESENT���), PRESENT -> BENDER TARGET �����[�v�����Ă���
            barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
            //��̏��()
            barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
            //�o���A���g�w����s
            cmdList_->ResourceBarrier(1, &barrierDesc);

            cmdList_->SetPipelineState(pipelinestate);
        }

        //�����_�[�^�[�Q�b�g���w��
        auto rtvH = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
        rtvH.ptr += bbIdx * dev_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        cmdList_->OMSetRenderTargets(1, &rtvH, false, nullptr);

        //��ʃN���A
        //float clearColor[] = { 1.0f,0.0f,0.0f,1.0f };
        float r,g,b;
        r = (float)(0xff & frame >> 16) / 255.0f;
        g = (float)(0xff & frame >> 16) / 255.0f;
        b = (float)(0xff & frame >> 16) / 255.0f;
        float clearColor[] = { r,g,b,1.0f };

        cmdList_->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
        frame++;

        cmdList_->RSSetViewports(1, &viewport);
        cmdList_->RSSetScissorRects(1, &scissorrect);
        cmdList_->SetGraphicsRootSignature(rootsignature);

        cmdList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        cmdList_->IASetVertexBuffers(0, 1, &vbView);
        cmdList_->IASetIndexBuffer(&ibView);

        cmdList_->SetGraphicsRootSignature(rootsignature);
        cmdList_->SetDescriptorHeaps(1, &basicDescHeap);
        //cmdList_->SetGraphicsRootDescriptorTable(0, texDescHeap->GetGPUDescriptorHandleForHeapStart());
        cmdList_->SetGraphicsRootDescriptorTable(0, basicDescHeap->GetGPUDescriptorHandleForHeapStart());

        //auto heapHandle = basicDescHeap->GetGPUDescriptorHandleForHeapStart();
        //heapHandle.ptr += dev_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
        //cmdList_->SetGraphicsRootDescriptorTable(1, heapHandle);

        cmdList_->DrawIndexedInstanced(6, 1, 0, 0, 0);

        //���̏��(BENDER TARGET)
        barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
        //��̏��(�ŏ��ɖ߂�̂�PRESENT)
        barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
        //�o���A���g�w����s
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
            //�C�x���g�n���h���̎擾
            auto event = CreateEvent(nullptr, false, false, nullptr);
            //�t�F���X������̒l�ɒB�������ɔ�������C�x���g,�����fenceval�̒l�̎���event�𔭐�������
            fence->SetEventOnCompletion(fenceVal, event);
            //�I�u�W�F�N�g���V�O�i����ԂɂȂ�܂őҋ@
            WaitForSingleObject(event, INFINITE);
            //�n���h�������
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

    /*
    dxgiFactory_->Release();
    dev_->Release();
    cmdAllocator_->Release();
    cmdList_->Release();
    cmdQueue_->Release();
    swapchain_->Release();
    rtvHeaps->Release();

    for (auto& adapter : adapters)
    {
        adapter->Release();
    }
    tmpAdapter->Release();

    for (auto& buffer : backBuffers)
    {
        buffer->Release();
    }

    fence->Release();
    vsBlob->Release();
    psBlob->Release();
    errorBlob->Release();
    pipelinestate->Release();
    rootsignature->Release();
    */
    return 0;
}
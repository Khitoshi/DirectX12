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

struct Vertex
{
    XMFLOAT3 position;//xyz座標
    XMFLOAT2 uv;//uv情報
};

struct TexRGBA
{
    unsigned char R, G, B, A;
};


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

    //OSにアプリケーションの生成を通達する
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
    
    //利用可能な全てのディスプレイ サブシステム (1 つ以上の GPU、DAC、ビデオ メモリを含む)をadaptersに積む
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
    result = dev_->CreateCommandAllocator(
        D3D12_COMMAND_LIST_TYPE_DIRECT, 
        IID_PPV_ARGS(&cmdAllocator_)
    );
    ExceptionHandlingFormatHresult(result);
    
    //コマンドリスト　作成
    result = dev_->CreateCommandList(
        0, 
        D3D12_COMMAND_LIST_TYPE_DIRECT, 
        cmdAllocator_, 
        nullptr, 
        IID_PPV_ARGS(&cmdList_)
    );
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
    
    //fenceの作成
    //fence: CPUと1つ以上のGPUの同期に使用されるオブジェクトを表している
    ID3D12Fence* fence = nullptr;
    UINT64 fenceVal = 0;
    result = dev_->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));

    //window output
    ShowWindow(hwnd,SW_SHOW);

    {//モデル読み込み
        /*
        struct FBXHeader
        {
            float version;//
            char model_name[20];//モデル名
            char comment[256];//モデルコメント
        };

        FILE* fp;
        char signature[3];
        FBXHeader fbxHeader;

        fread(signature, sizeof(signature), 1, fp);
        fread(&fbxHeader, sizeof(fbxHeader), 1, fp);

        unsigned int vertNum;//頂点数
        fread(&vertNum, sizeof(vertNum), 1, fp);
        */

        //errorが起こる場合はhttps://yttm-work.jp/model_render/model_render_0006.html#head_line_01の
        //「dllファイルの設定」を確認して設定
        fbxsdk::FbxManager* fbx_manager = fbxsdk::FbxManager::Create();

        //エラーチェック
        if (fbx_manager == NULL)
        {
            return -1;
        }
        
        //fbxファイル名 指定
        const string filePath("./Model/YBot.fbx");

        //入出力設定を作成
        FbxIOSettings* ios = FbxIOSettings::Create(fbx_manager, IOSROOT);

        //マネージャーに入出力設定をセット
        fbx_manager->SetIOSettings(ios);

        //FBXインポータを初期化
        FbxImporter* importer = FbxImporter::Create(fbx_manager, "");

        //FBXファイルの読み込み
        if (!importer->Initialize(filePath.c_str(), -1, fbx_manager->GetIOSettings()))
        {
            //失敗した場合,fbxManagerを破棄して終了
            fbx_manager->Destroy();
            return -1;
        }




        //fbxManager破棄
        fbx_manager->Destroy();
    }


    //ここに座標を入れる(注意:座標は時計回りにする)
    Vertex vertices[] = {
        {{-0.4f,-0.7f,0.0f} , {0.0f,1.0f}  },//左下
        { {-0.4f,0.7f,0.0f} ,  {0.0f,0.0f}  },//左上
        { {0.4f,-0.7f,0.0f} ,  {1.0f,1.0f}  },//右下
        { {0.4f,0.7f,0.0f} ,   {1.0f,0.0f}  },//右上
    };

    //頂点バッファー設定&生成
    D3D12_VERTEX_BUFFER_VIEW vbView = {};
    D3D12_INDEX_BUFFER_VIEW ibView = {};
    D3D12_HEAP_PROPERTIES heapprop = {};
    {
        //頂点ヒープの設定
        //ヒープの種類を指定，今回はCPUからアクセスできるようにしている
        heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;
        //CPU ページ プロパティを指定,今回は特に考えない
        heapprop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        //ヒープのメモリ プールを指定，今回は特に考えない
        heapprop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

        //頂点バッファー設定
        D3D12_RESOURCE_DESC resourceDesc = {};
        //使用しているリソースのタイプを設定 
        resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        //幅でまかなうので全頂点(sizeof)とする
        resourceDesc.Width = sizeof(vertices);
        //幅でまかなうので1
        resourceDesc.Height = 1;
        //二次元なので1
        resourceDesc.DepthOrArraySize = 1;
        //
        resourceDesc.MipLevels = 1;
        //画像ではないので UNKNOWN
        resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
        //ピクセルあたりのマルチサンプル数なので1
        resourceDesc.SampleDesc.Count = 1;
        //オプションの指定は特にないのでNONE
        resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
        //テクスチャのデータが行優先順で格納されているので
        resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

        //頂点バッファー生成
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


        //頂点情報のコピー(mapする)
        Vertex* vertMap = nullptr;
        result = vertBuff->Map(0, nullptr, (void**)&vertMap);
        ExceptionHandlingFormatHresult(result);
        copy(begin(vertices), end(vertices), vertMap);
        //mapを解除
        vertBuff->Unmap(0, nullptr);

        //バッファの仮想アドレス
        vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
        //全バイト数
        vbView.SizeInBytes = sizeof(vertices);
        //1頂点あたりのバイト数
        vbView.StrideInBytes = sizeof(vertices[0]);

        unsigned short indices[] = { 0,1,2, 2,1,3 };

        ID3D12Resource* idxBuff = nullptr;
        //設定は、バッファのサイズ以外頂点バッファの設定を使いまわして
        result = dev_->CreateCommittedResource(
            &heapprop,
            D3D12_HEAP_FLAG_NONE,
            &resourceDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&idxBuff));

        //作ったバッファにインデックスデータをコピー
        unsigned short* mappedIdx = nullptr;
        idxBuff->Map(0, nullptr, (void**)&mappedIdx);
        copy(begin(indices), end(indices), mappedIdx);
        idxBuff->Unmap(0, nullptr);

        //インデックスバッファビューを作成
        ibView.BufferLocation = idxBuff->GetGPUVirtualAddress();
        ibView.Format = DXGI_FORMAT_R16_UINT;
        ibView.SizeInBytes = sizeof(indices);
    }


    //シェーダー読み込み
    ID3DBlob* vsBlob = nullptr;
    ID3DBlob* psBlob = nullptr;

    ID3DBlob* errorBlob = nullptr;
    //vs 読み込み
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
                ::OutputDebugStringA("ファイルが見当たりません");
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

    //ps 読み込み
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
                ::OutputDebugStringA("ファイルが見当たりません");
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
            {//位置情報
                    "POSITION",
                    0,
                    DXGI_FORMAT_R32G32B32_FLOAT,
                    0,
                    D3D12_APPEND_ALIGNED_ELEMENT,
                    D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
                    0
            },
            {//uv情報
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

        //加算，乗算やaブレンドは使用しない
        renderTargetBlendDesc.BlendEnable = false;
        renderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
        renderTargetBlendDesc.LogicOpEnable = false;
        gpipeline.BlendState.RenderTarget[0] = renderTargetBlendDesc;

        //アンチェリは使わないのでfalse
        gpipeline.RasterizerState.MultisampleEnable = false;
        //カリングしないのでNONE
        gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
        //中身を塗りつぶす
        gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
        //震度方向のクリッピングは有効にする
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

        //レイアウト先頭アドレス
        gpipeline.InputLayout.pInputElementDescs = inputLayout;
        //レイアウト配列数
        gpipeline.InputLayout.NumElements = _countof(inputLayout);

        //ストリップ時のカットなし
        gpipeline.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
        //三角形で構成
        gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

        //今は１つのみ
        gpipeline.NumRenderTargets = 1;

        //0～1に正規化されたRGBA
        gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

        //サンプリングは1ピクセルにつき１
        gpipeline.SampleDesc.Count = 1;
        //クオリティは最低
        gpipeline.SampleDesc.Quality = 0;

        D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
        {
            rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

            D3D12_DESCRIPTOR_RANGE descTblRange[2] = {};
            
            //テクスチャ用レジスタ0番
            descTblRange[0].NumDescriptors = 1;//テクスチャひとつ
            descTblRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;//種別はテクスチャ
            descTblRange[0].BaseShaderRegister = 0;//0番スロットから
            descTblRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

            //定数用レジスタ0番
            descTblRange[1].NumDescriptors = 1;//定数はひとつ
            descTblRange[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
            descTblRange[1].BaseShaderRegister = 0;//0番スロットから
            descTblRange[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

            D3D12_ROOT_PARAMETER rootparam = {};
            rootparam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            rootparam.DescriptorTable.pDescriptorRanges = &descTblRange[0];//デスクリプタレンジのアドレス
            rootparam.DescriptorTable.NumDescriptorRanges = 2;//デスクリプタレンジ数
            rootparam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;//ピクセルシェーダから見える

            rootSignatureDesc.pParameters = &rootparam;//ルートパラメータの先頭アドレス
            rootSignatureDesc.NumParameters = 1;//ルートパラメータ数

            D3D12_STATIC_SAMPLER_DESC samplerDesc = {};
            samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//横繰り返し
            samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//縦繰り返し
            samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//奥行繰り返し
            samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;//ボーダーの時は黒
            samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;//補間しない(ニアレストネイバー)
            samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;//ミップマップ最大値
            samplerDesc.MinLOD = 0.0f;//ミップマップ最小値
            samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;//オーバーサンプリングの際リサンプリングしない？
            samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//ピクセルシェーダからのみ可視

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

        
        viewport.Width = window_width;//出力先の幅(ピクセル数)
        viewport.Height = window_height;//出力先の高さ(ピクセル数)
        viewport.TopLeftX = 0;//出力先の左上座標X
        viewport.TopLeftY = 0;//出力先の左上座標Y
        viewport.MaxDepth = 1.0f;//深度最大値
        viewport.MinDepth = 0.0f;//深度最小値


        scissorrect.top = 0;//切り抜き上座標
        scissorrect.left = 0;//切り抜き左座標
        scissorrect.right = scissorrect.left + window_width;//切り抜き右座標
        scissorrect.bottom = scissorrect.top + window_height;//切り抜き下座標

    }

    TexMetadata metadata = {};
    ScratchImage scratchImg = {};
    result = LoadFromWICFile(L"Asset/Texture/textest.png", WIC_FLAGS_NONE, &metadata, scratchImg);
    auto img = scratchImg.GetImage(0, 0, 0);//生データ抽出

    //テクスチャバッファー作成
    D3D12_HEAP_PROPERTIES texHeapProp = {};
    texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
    texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
    texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
    texHeapProp.CreationNodeMask = 0;
    texHeapProp.VisibleNodeMask = 0;

    D3D12_RESOURCE_DESC resDesc = {};
    //DXGI_FORMAT_R8G8B8A8_UNORM;//RGBAフォーマット
    resDesc.Format = metadata.format;
    //幅
    resDesc.Width = static_cast<UINT>(metadata.width);
    //高さ
    resDesc.Height = static_cast<UINT>(metadata.height);
    //2Dで配列でもないので１
    resDesc.DepthOrArraySize = static_cast<uint16_t>(metadata.arraySize);
    //通常テクスチャなのでアンチェリしない
    resDesc.SampleDesc.Count = 1;
    resDesc.SampleDesc.Quality = 0;//
    //ミップマップしないのでミップ数は１つ
    resDesc.MipLevels = static_cast<uint16_t>(metadata.mipLevels);
    //2Dテクスチャ用
    resDesc.Dimension = static_cast<D3D12_RESOURCE_DIMENSION>(metadata.dimension);
    //レイアウトについては決定しない
    resDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    //とくにフラグなし
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

    //テクスチャバッファーの転送
    result = texbuff->WriteToSubresource(
        0,
        nullptr,//全領域へコピー
        img->pixels,//元データアドレス
        static_cast<UINT>(img->rowPitch),//1ラインサイズ
        static_cast<UINT>(img->slicePitch)//全サイズ
    );
    ExceptionHandlingFormatHresult(result);


    //GPUに情報を渡すための定数バッファ作成
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

    //定数バッファ生成
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

    
    //マップで定数のコピー
    XMMATRIX* mapMatrix;
    auto worldMat = XMMatrixRotationY(XM_PIDIV4);
    result = constBuff->Map(0, nullptr, (void**)&mapMatrix);
    ExceptionHandlingFormatHresult(result);
    *mapMatrix = worldMat * viewMat * perspectMat;

    ID3D12DescriptorHeap* basicDescHeap = nullptr;
    //ディスクリプタヒープ生成
    D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
    //シェーダーから見えるように
    descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    //マスクは0
    descHeapDesc.NodeMask = 0;
    //SRV1つとCBV1つ
    descHeapDesc.NumDescriptors = 2;
    //ディスクリプタヒープの種別
    descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    result = dev_->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&basicDescHeap));
    ExceptionHandlingFormatHresult(result);
    

    //シェーダーリソースビュー作成
    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    //RGBA(0.0f ~ 1.0fに正規化)
    srvDesc.Format = metadata.format;
    //
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    //2Dテクスチャなので
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    //ミップマップは作成しないので
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

    //定数バッファビュー作成
    dev_->CreateConstantBufferView(&cbvDesc, basicHeapHandle);

    MSG msg = {};
    unsigned int frame = 0;
    float angle = 0.0f;
    while (true)
    {
        //アプリケーションの状態を更新
        {
            if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }

            //アプリケーションが終わるときにmessageがWM_QUITになる
            if (msg.message == WM_QUIT) break;
        }

        angle += 0.1f;
        worldMat = XMMatrixRotationY(angle);
        *mapMatrix = worldMat * viewMat * perspectMat;

        //DirectX処理
        //バックバッファのインデックスを取得
        auto bbIdx = swapchain_->GetCurrentBackBufferIndex();

        //リソース使用の遷移をGPUに通達
        //リソースバリア設定&実行
        D3D12_RESOURCE_BARRIER barrierDesc = {};
        {
            //バリアの指定,今回は状態遷移なのでTRANSITION
            barrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
            //開始と終了を指定，今回はNONE(特に指定なし)
            barrierDesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
            //リソースのアドレス
            barrierDesc.Transition.pResource = backBuffers[bbIdx];
            //サブリソース番号
            barrierDesc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
            //元の状態(直前はPRESENT状態), PRESENT -> BENDER TARGET をループさせている
            barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
            //後の状態()
            barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
            //バリアント指定実行
            cmdList_->ResourceBarrier(1, &barrierDesc);

            cmdList_->SetPipelineState(pipelinestate);
        }

        //レンダーターゲットを指定
        auto rtvH = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
        rtvH.ptr += bbIdx * dev_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        cmdList_->OMSetRenderTargets(1, &rtvH, false, nullptr);

        //画面クリア
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

        //元の状態(BENDER TARGET)
        barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
        //後の状態(最初に戻るのでPRESENT)
        barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
        //バリアント指定実行
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
            //イベントハンドルの取得
            auto event = CreateEvent(nullptr, false, false, nullptr);
            //フェンスが特定の値に達した時に発生するイベント,今回はfencevalの値の時にeventを発生させる
            fence->SetEventOnCompletion(fenceVal, event);
            //オブジェクトがシグナル状態になるまで待機
            WaitForSingleObject(event, INFINITE);
            //ハンドルを閉じる
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
#include "tkEngine.h"
#include "GraphicsEngine.h"
//デフォルト コンストラクタ
tkEngine::tkEngine():
    graphics_Engine_(nullptr),
    tkm_File_Bank_(),
    shader_Bank_(),
    texture_Bank_(),
    pad_()
{
}

//デフォルト コンストラクタ
tkEngine::~tkEngine()
{
}

//フレームの開始時に呼ばれる処理
void tkEngine::BeginFrame()
{
}

//フレームの終了時に呼ばれる処理
void tkEngine::EndFrame()
{
    graphics_Engine_->EndRender();
}

//ゲームエンジンの初期化
void tkEngine::Init(
    HWND hwnd, 
    UINT frameBufferWidth, 
    UINT frameBufferHeight, 
    Camera& camera
)
{
    //this->graphics_Engine_.reset(&graphicsEngine);

    graphics_Engine_ = std::make_unique<GraphicsEngine>(hwnd, frameBufferWidth, frameBufferHeight);

    graphics_Engine_->Init(camera);
    //ゲームパッドの初期化。
    //for (int i = 0; i < GamePad::CONNECT_PAD_MAX; i++) {
    //    g_pad[i] = &m_pad[i];
    //}
}

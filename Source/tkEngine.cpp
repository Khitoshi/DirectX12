#include "tkEngine.h"

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
}

//ゲームエンジンの初期化
void tkEngine::Init(HWND hwnd, UINT frameBufferWidth, UINT frameBufferHeight)
{
}

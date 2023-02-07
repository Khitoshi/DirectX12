#include "tkEngine.h"
#include "GraphicsEngine.h"
//�f�t�H���g �R���X�g���N�^
tkEngine::tkEngine():
    graphics_Engine_(nullptr),
    tkm_File_Bank_(),
    shader_Bank_(),
    texture_Bank_(),
    pad_()
{
}

//�f�t�H���g �R���X�g���N�^
tkEngine::~tkEngine()
{
    if (graphics_Engine_)delete graphics_Engine_;
}

//�t���[���̊J�n���ɌĂ΂�鏈��
void tkEngine::BeginFrame(GraphicsEngine*& graphicsEngine, Camera& camera)
{
    graphicsEngine->BeginRender(camera);

    //pad�̏�����
    for (auto& pad : this->pad_)
    {
        pad.BeginFrame();
        pad.Update();
    }

}

//�t���[���̏I�����ɌĂ΂�鏈��
void tkEngine::EndFrame()
{
    graphics_Engine_->EndRender();
}

//�Q�[���G���W���̏�����
void tkEngine::Init(
    HWND hwnd, 
    UINT frameBufferWidth, 
    UINT frameBufferHeight, 
    Camera& camera
)
{
    //this->graphics_Engine_.reset(&graphicsEngine);

    graphics_Engine_ = new GraphicsEngine(hwnd, frameBufferWidth, frameBufferHeight);

    graphics_Engine_->Init(camera);
    //�Q�[���p�b�h�̏������B
    //for (int i = 0; i < GamePad::CONNECT_PAD_MAX; i++) {
    //    g_pad[i] = &m_pad[i];
    //}
}

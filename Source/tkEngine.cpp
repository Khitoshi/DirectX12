#include "tkEngine.h"

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
}

//�t���[���̊J�n���ɌĂ΂�鏈��
void tkEngine::BeginFrame()
{
}

//�t���[���̏I�����ɌĂ΂�鏈��
void tkEngine::EndFrame()
{
}

//�Q�[���G���W���̏�����
void tkEngine::Init(HWND hwnd, UINT frameBufferWidth, UINT frameBufferHeight)
{
}

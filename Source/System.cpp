#include "System.h"
#include "GraphicsEngine.h"
#include <memory>
#include "Camera.h"

//���b�Z�[�W�v���V�[�W��
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


System::System():
	hWnd_(NULL)
{
}

System::~System()
{
}

void System::InitGraphicSystem(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow, const TCHAR* appName,Camera& camera)
{
	InitWindow(hInstance, hPrevInstance, lpCmdLine, nCmdShow, appName);
	
	std::unique_ptr<GraphicsEngine>graphics_engine;
	graphics_engine = std::make_unique<GraphicsEngine>(hWnd_,frame_Buffer_width_, frame_Buffer_height_);
	graphics_engine->Init(camera);
	return;
}

void System::InitWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow, const TCHAR* appName)
{
	//�E�B���h�E�N���X�̃p�����[�^��ݒ�
	WNDCLASSEX wc;
    //�\���̂̃T�C�Y
	wc.cbSize = sizeof(WNDCLASSEX);
	//�E�B���h�E�̃X�^�C��
	wc.style = CS_CLASSDC;
    //�R�[���o�b�N�֐�
	wc.lpfnWndProc = (WNDPROC)WindowProcedure;
	//�E�B���h�E �N���X�\���̂̌�Ɋ��蓖�Ă�]���ȃo�C�g��
	wc.cbClsExtra = 0;
	//�E�B���h�E �C���X�^���X�̌�Ɋ��蓖�Ă�]���ȃo�C�g��
	wc.cbWndExtra = 0;
	//�N���X�̃E�B���h�E �v���V�[�W�����܂ރC���X�^���X�ւ̃n���h��
	wc.hInstance = GetModuleHandle(0);
	//�N���X �A�C�R���ւ̃n���h��
	//NULL�Ȃ̂ŁA�V�X�e���ɂ͊���̃A�C�R�����\������܂��B
	wc.hIcon = NULL;
	//�N���X �J�[�\���ւ̃n���h��
	//NULL�Ȃ̂ŁA�A�v���P�[�V�����́A�}�E�X���A�v���P�[�V�����̃E�B���h�E�Ɉړ����邽�тɁA�J�[�\���̐}�`�𖾎��I�ɐݒ肷��K�v������܂��B
	wc.hCursor = NULL;
	//�N���X�̔w�i�u���V�ւ̃n���h��
	//NULL�Ȃ̂ŁC�A�v���P�[�V�����̓N���C�A���g�̈�ŕ`���v������邽�тɁA�Ǝ��̔w�i��`�悷��K�v������
	wc.hbrBackground = NULL;
	//���\�[�X �t�@�C���ɖ��O���\�������悤��
	//NULL�Ȃ̂ŁC�N���X�ɑ�����E�B���h�E�ɂ͊���̃��j���[�͂���܂���
	wc.lpszMenuName = NULL;
    //�A�v���P�[�V�����N���X
    wc.lpszClassName = appName;
	//�E�B���h�E �N���X�Ɋ֘A�t�����Ă��鏬���ȃA�C�R���ւ̃n���h��
	//NULL�Ȃ̂ŁC�����œK�؂ȃT�C�Y�̃A�C�R������������
	wc.hIconSm = NULL;
	//�E�B���h�E�N���X�̓o�^�B
	RegisterClassEx(&wc);


	//�E�B���h�E�쐬
	// �E�B���h�E�̍쐬�B
	hWnd_ = CreateWindow(
		appName,				//�E�B���h�E�N���X�̖��O
		appName,				//�E�B���h�E�̖��O�B�E�B���h�E�N���X�̖��O�ƕʖ��ł��悢
		WS_OVERLAPPEDWINDOW,	//�E�B���h�E�X�^�C���B�Q�[���ł͊�{�I��WS_OVERLAPPEDWINDOW�ł���
		0,						//�E�B���h�E�̏���X���W
		0,						//�E�B���h�E�̏���Y���W
		frame_Buffer_width_,	//�E�B���h�E�̕�
		frame_Buffer_height_,	//�E�B���h�E�̍���
		NULL,					//�e�E�B���h�E�B�Q�[���ł͊�{�I��NULL�ł���
		NULL,					//���j���[
		hInstance,				//�A�v���P�[�V�����̃C���X�^���X
		NULL
	);

	//window�\��
	ShowWindow(hWnd_, nCmdShow);
}

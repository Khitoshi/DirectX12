#include <Windows.h>
#include "GamePad.h"
#include <array>

#define INPUT_DEADZONE  ( 0.24f * FLOAT(0x7FFF) )
//Default to 24% of the +/- 32767 range.   This is a reasonable default value but can be altered if needed.
std::array<GamePad*, GamePad::CONNECT_PAD_MAX> g_pad;

namespace {
	
	/// <summary>
	/// ���z�{�^����XBox�R���g���[���̃{�^���Ƃ̊֘A�t����\���\����
	/// </summary>
	struct VirtualPadToXPad {
		EnButton vButton;		//!<���z�{�^���B
		DWORD	 xButton;		//!<XBox�R���g���[���̃{�^���B
	};

	const VirtualPadToXPad vPadToXPadTable[enButtonNum] = {
		{ enButtonUp		, XINPUT_GAMEPAD_DPAD_UP },
		{ enButtonDown		, XINPUT_GAMEPAD_DPAD_DOWN },
		{ enButtonLeft		, XINPUT_GAMEPAD_DPAD_LEFT },
		{ enButtonRight		, XINPUT_GAMEPAD_DPAD_RIGHT },
		{ enButtonA			, XINPUT_GAMEPAD_A },
		{ enButtonB			, XINPUT_GAMEPAD_B },
		{ enButtonY			, XINPUT_GAMEPAD_Y },
		{ enButtonX			, XINPUT_GAMEPAD_X },
		{ enButtonSelect	, XINPUT_GAMEPAD_BACK },
		{ enButtonStart		, XINPUT_GAMEPAD_START },
		{ enButtonRB1		, XINPUT_GAMEPAD_RIGHT_SHOULDER },
		{ enButtonRB2		, 0 },
		{ enButtonRB3		, XINPUT_GAMEPAD_RIGHT_THUMB },
		{ enButtonLB1		, XINPUT_GAMEPAD_LEFT_SHOULDER },
		{ enButtonLB2		, 0 },
		{ enButtonLB3		, XINPUT_GAMEPAD_LEFT_THUMB },
	};
	
	/// <summary>
	/// ���z�{�^���ƃL�[�{�[�h�Ƃ̊֘A�t����\���\����
	/// </summary>
	struct VirtualPadToKeyboard {
		EnButton vButton;		//!<���z�{�^��
		DWORD keyCoord1;		//!<�L�[�{�[�h�̃L�[�R�[�h�B
		DWORD keyCoord2;		//!<�L�[�{�[�h�̃L�[�R�[�h�B
	};
	const VirtualPadToKeyboard vPadToKeyboardTable[enButtonNum] = {
		{ enButtonUp		, VK_NUMPAD8,	'8' 		},
		{ enButtonDown		, VK_NUMPAD2,	'2' 		},
		{ enButtonLeft		, VK_NUMPAD4,	'4' 		},
		{ enButtonRight		, VK_NUMPAD6,	'6' 		},
		{ enButtonA			, 'J',			0xFFFFFFFF	},
		{ enButtonB			, 'K',			0xFFFFFFFF	},
		{ enButtonY			, 'L',			0xFFFFFFFF	},
		{ enButtonX			, 'I',			0xFFFFFFFF	},
		{ enButtonSelect	, VK_SPACE,		0xFFFFFFFF	},
		{ enButtonStart		, VK_RETURN,	0xFFFFFFFF	},
		{ enButtonRB1		, '7',			0xFFFFFFFF	},
		{ enButtonRB2		, '8',			0xFFFFFFFF	},
		{ enButtonRB3		, '9',			0xFFFFFFFF	},
		{ enButtonLB1		, 'B',			0xFFFFFFFF	},
		{ enButtonLB2		, 'N',			0xFFFFFFFF	},
		{ enButtonLB3		, 'M',			0xFFFFFFFF	},
	};
}
GamePad::EnXInputPadState GamePad::pad_States_[4] = { GamePad::EnXInputPadState::Undef };


//�f�t�H���g �R���X�g���N�^
GamePad::GamePad():
	state_(),
	pad_No_(0),
	trigger_(),
	press_(),
	L_Stick_X_(),
	L_Stick_Y_(),
	R_Stick_X_(),
	R_Stick_Y_()
{
	memset(&this->state_, 0, sizeof(this->state_));
	memset(&this->trigger_, 0, sizeof(this->trigger_));
	memset(&this->press_, 0, sizeof(this->press_));
}

//�f�t�H���g �f�X�g���N�^
GamePad::~GamePad()
{
}

//������
void GamePad::Init(int padNo)
{
	this->pad_No_ = padNo;
}

//�X�V
void GamePad::Update()
{
	//�A�N�e�B�u�p�b�h��T���B
	DWORD result = ERROR_DEVICE_NOT_CONNECTED;

	for (int i = pad_No_; i < MAX_PAD; i++)
	{
		//�������̃p�b�h
		result = XInputGetState(i, &this->state_.state);
		if (result == ERROR_SUCCESS)
		{
			//�ڑ�����
			this->pad_States_[i] = EnXInputPadState::Connect;
		}
		else
		{
			//�q�����Ă��Ȃ��̂Ŏ��̃p�b�h�𒲂ׂ�
			this->pad_States_[i] = EnXInputPadState::Disconnect;
		}
	}

	if (result == ERROR_SUCCESS)
	{
		//�ڑ�����Ă���
		this->state_.bConnected = true;

		for (const VirtualPadToXPad& vPadToXPad : vPadToXPadTable) {
			if (this->state_.state.Gamepad.wButtons & vPadToXPad.xButton) {
				this->trigger_[vPadToXPad.vButton] = 1 ^ this->press_[vPadToXPad.vButton];
				this->press_[vPadToXPad.vButton] = 1;
			}
			else {
				this->trigger_[vPadToXPad.vButton] = 0;
				this->press_[vPadToXPad.vButton] = 0;
			}
		}

		//���g���K�[
		if (this->state_.state.Gamepad.bLeftTrigger != 0) {
			this->trigger_[enButtonLB2] = 1 ^ this->press_[enButtonLB2];
			this->press_[enButtonLB2] = 1;
		}
		else {
			this->trigger_[enButtonLB2] = 0;
			this->press_[enButtonLB2] = 0;
		}
		//�E�g���K�[
		if (this->state_.state.Gamepad.bRightTrigger != 0) {
			this->trigger_[enButtonRB2] = 1 ^ this->press_[enButtonRB2];
			this->press_[enButtonRB2] = 1;
		}
		else {
			this->trigger_[enButtonRB2] = 0;
			this->press_[enButtonRB2] = 0;
		}
		if ((this->state_.state.Gamepad.sThumbLX < INPUT_DEADZONE &&
			this->state_.state.Gamepad.sThumbLX > -INPUT_DEADZONE) &&
			(this->state_.state.Gamepad.sThumbLY < INPUT_DEADZONE &&
				this->state_.state.Gamepad.sThumbLY > -INPUT_DEADZONE))
		{
			this->state_.state.Gamepad.sThumbLX = 0;
			this->state_.state.Gamepad.sThumbLY = 0;
			this->L_Stick_X_ = 0.0f;
			this->L_Stick_Y_ = 0.0f;
		}
		else {
			//���X�e�B�b�N�̓��͗�
			if (this->state_.state.Gamepad.sThumbLX > 0) {
				this->L_Stick_X_ = static_cast<float>(this->state_.state.Gamepad.sThumbLX) / SHRT_MAX;
			}
			else {
				this->L_Stick_X_ = static_cast<float>(this->state_.state.Gamepad.sThumbLX) / -SHRT_MIN;
			}
			if (this->state_.state.Gamepad.sThumbLY > 0) {
				this->L_Stick_Y_ = static_cast<float>(this->state_.state.Gamepad.sThumbLY) / SHRT_MAX;
			}
			else {
				this->L_Stick_Y_ = static_cast<float>(this->state_.state.Gamepad.sThumbLY) / -SHRT_MIN;
			}
		}

		if ((this->state_.state.Gamepad.sThumbRX < INPUT_DEADZONE &&
			this->state_.state.Gamepad.sThumbRX > -INPUT_DEADZONE) &&
			(this->state_.state.Gamepad.sThumbRY < INPUT_DEADZONE &&
				this->state_.state.Gamepad.sThumbRY > -INPUT_DEADZONE))
		{
			this->state_.state.Gamepad.sThumbRX = 0;
			this->state_.state.Gamepad.sThumbRY = 0;
			this->R_Stick_X_ = 0.0f;
			this->R_Stick_Y_ = 0.0f;
		}
		else {
			//�E�X�e�B�b�N�̓��͗ʁB
			if (this->state_.state.Gamepad.sThumbRX > 0) {
				this->R_Stick_X_ = static_cast<float>(this->state_.state.Gamepad.sThumbRX) / SHRT_MAX;
			}
			else {
				this->R_Stick_X_ = static_cast<float>(this->state_.state.Gamepad.sThumbRX) / -SHRT_MIN;
			}
			if (this->state_.state.Gamepad.sThumbRY > 0) {
				this->R_Stick_Y_ = static_cast<float>(this->state_.state.Gamepad.sThumbRY) / SHRT_MAX;
			}
			else {
				this->R_Stick_Y_ = static_cast<float>(this->state_.state.Gamepad.sThumbRY) / -SHRT_MIN;
			}
		}


	}
	else
	{
		//�ڑ�����Ă��Ȃ��ꍇ�̓L�[�{�[�h�̓��͂ŃG�~�����[�g����B
		if (this->state_.bConnected) {
			//���ڑ��ɂȂ����B
			memset(&this->state_, 0, sizeof(this->state_));
			memset(this->trigger_, 0, sizeof(this->trigger_));
			memset(this->press_, 0, sizeof(this->press_));
		}
		this->L_Stick_X_ = 0.0f;
		this->L_Stick_Y_ = 0.0f;
		this->R_Stick_X_ = 0.0f;
		this->R_Stick_Y_ = 0.0f;

		if (GetAsyncKeyState(VK_LEFT)) {
			this->R_Stick_X_ = -1.0f;
		}
		else if (GetAsyncKeyState(VK_RIGHT)) {
			this->R_Stick_X_ = 1.0f;
		}
		if (GetAsyncKeyState(VK_UP)) {
			this->R_Stick_Y_ = 1.0f;
		}
		else if (GetAsyncKeyState(VK_DOWN)) {
			this->R_Stick_Y_ = -1.0f;
		}
		//�X�e�B�b�N�̓��͗ʂ𐳋K���B
		float t = fabsf(this->R_Stick_X_) + fabsf(this->R_Stick_Y_);
		if (t > 0.0f) {
			this->R_Stick_X_ /= t;
			this->R_Stick_Y_ /= t;
		}

		if (GetAsyncKeyState('A')) {
			this->L_Stick_X_ = -1.0f;
		}
		else if (GetAsyncKeyState('D')) {
			this->L_Stick_X_ = 1.0f;
		}
		if (GetAsyncKeyState('W')) {
			this->L_Stick_Y_ = 1.0f;
		}
		else if (GetAsyncKeyState('S')) {
			this->L_Stick_Y_ = -1.0f;
		}
		//�X�e�B�b�N�̓��͗ʂ𐳋K���B
		t = fabsf(this->L_Stick_X_) + fabsf(this->L_Stick_Y_);
		if (t > 0.0f) {
			this->L_Stick_X_ /= t;
			this->L_Stick_Y_ /= t;
		}

		for (const VirtualPadToKeyboard& vPadToKeyboard : vPadToKeyboardTable) {
			if (GetAsyncKeyState(vPadToKeyboard.keyCoord1)) {
				//�D�揇�ʂP�Ԗڂ̃L�[�B
				this->trigger_[vPadToKeyboard.vButton] = 1 ^ this->press_[vPadToKeyboard.vButton];
				this->press_[vPadToKeyboard.vButton] = 1;
			}
			else if (vPadToKeyboard.keyCoord2 != 0xFFFFFFFF
				&& GetAsyncKeyState(vPadToKeyboard.keyCoord2)) {
				//�D�揇�ʂQ�Ԗڂ̃L�[�B
				this->trigger_[vPadToKeyboard.vButton] = 1 ^ this->press_[vPadToKeyboard.vButton];
				this->press_[vPadToKeyboard.vButton] = 1;
			}
			else {
				this->trigger_[vPadToKeyboard.vButton] = 0;
				this->press_[vPadToKeyboard.vButton] = 0;
			}
		}
	}
}

//�t���[���̊J�n���ɌĂ΂��֐�
void GamePad::BeginFrame()
{
	//�S�Ė���`�ɂ���
	for (auto& pad_state : GamePad::pad_States_)
	{
		pad_state = EnXInputPadState::Undef;
	}
}


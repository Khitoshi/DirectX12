#pragma once

#define WIN32_LEAN_AND_MEAN 

#include <Xinput.h>

/// <summary>
/// ���z�{�^����`
/// </summary>
enum EnButton {
	enButtonUp = 0,		//��B
	enButtonDown,		//��
	enButtonLeft,		//��
	enButtonRight,		//�E
	enButtonA,			//A�{�^��
	enButtonB,			//B�{�^��
	enButtonX,			//X�{�^��
	enButtonY,			//Y�{�^��
	enButtonSelect,		//�Z���N�g�{�^��
	enButtonStart,		//�X�^�[�g�{�^��
	enButtonRB1,		//RB1�{�^��
	enButtonRB2,		//RB2�{�^��
	enButtonRB3,		//RB3�{�^��
	enButtonLB1,		//LB1�{�^��
	enButtonLB2,		//LB2�{�^��
	enButtonLB3,		//LB3�{�^��
	enButtonNum,		//�{�^���̐�
};

/// <summary>
/// �Q�[���p�b�h
/// </summary>
class GamePad
{
public:
	//�p�b�h�X�e�[�g
	struct PAD_STATE
	{
		XINPUT_STATE state;
		bool bConnected;
	};

	enum { MAX_PAD = 4 };

	enum class EnXInputPadState {
		Undef,		//�s���B
		Connect,	//�ڑ�����Ă���B
		Disconnect,	//�ڑ�����Ă��Ȃ��B
	};

public:
	/// <summary>
	/// �f�t�H���g �R���X�g���N�^
	/// </summary>
	GamePad();

	/// <summary>
	/// �f�t�H���g �f�X�g���N�^
	/// </summary>
	~GamePad();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="padNo">�p�b�h�ԍ�</param>
	void Init(int padNo);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	//�t���[���̊J�n���ɌĂ΂��֐�
	static void BeginFrame();

public:
#pragma region Get Method
	//�ڑ��\�ȃp�b�h�̍ő吔�B
	//static const int GetCONNECTPADMAX() { return 4; };
	//�ڑ��\�ȃp�b�h�̍ő吔
	static const int CONNECT_PAD_MAX = 4;

	/// <summary>
	/// �{�^���̃g���K�[����
	/// </summary>
	/// <param name="button"></param>
	/// <returns>true�̏ꍇ�g���K�[����</returns>
	bool IsTrigger(EnButton button)const
	{
		return this->trigger_[button] != 0;
	}

	/// <summary>
	/// �{�^����������Ă��邩����
	/// </summary>
	/// <param name="button"></param>
	/// <returns>true���Ԃ��Ă����牟����Ă���</returns>
	bool IsPress(EnButton button)const
	{
		return this->press_[button] != 0;
	}

	/// <summary>
	/// �����̃{�^����������Ă��邩����
	/// </summary>
	/// <returns>true�̏ꍇ�͉�����Ă���</returns>
	bool IsPressAnyKey() const
	{
		for (int i = 0; i < EnButton::enButtonNum; i++)
		{
			if (IsPress(static_cast<EnButton>(i)))
			{
				return true;
			}
		}
		return false;
	}

	/// <summary>
	/// L�X�e�B�b�N��X���̓��͗ʂ��擾
	/// </summary>
	/// <returns>-1.0�`1.0�̐��K�����ꂽ�l��Ԃ�</returns>
	float GetLStickXF() const
	{
		return this->L_Stick_X_;
	}

	/// <summary>
	/// L�X�e�B�b�N��Y���̓��͗ʂ��擾
	/// </summary>
	/// <returns>-1.0�`1.0�̐��K�����ꂽ�l��Ԃ�</returns>
	float GetLStickYF()const
	{
		return this->L_Stick_Y_;
	}

	/// <summary>
	/// R�X�e�B�b�N��X���̓��͗ʂ��擾
	/// </summary>
	/// <returns>-1.0�`1.0�̐��K�����ꂽ�l��Ԃ�</returns>
	float GetRStickXF()const
	{
		return this->R_Stick_X_;
	}

	/// <summary>
	/// R�X�e�B�b�N��Y���̓��͗ʂ��擾
	/// </summary>
	/// <returns>-1.0�`1.0�̐��K�����ꂽ�l��Ԃ�</returns>
	float GetRStickYF()const
	{
		return this->R_Stick_Y_;
	}

#pragma endregion

	static EnXInputPadState pad_States_[MAX_PAD];
private:
	//�p�b�h�X�e�[�g
	PAD_STATE state_;

	//�p�b�h�ԍ�
	int pad_No_;

	//�g���K�[���͂̃t���O
	int trigger_[enButtonNum];

	//press���͂̃t���O
	int press_[enButtonNum];

	//���X�e�B�b�N��X���̓��͗�
	float L_Stick_X_ = 0.0f;

	//���X�e�B�b�N��Y���̓��͗�
	float L_Stick_Y_ = 0.0f;

	//�E�X�e�B�b�N��X���̓��͗�
	float R_Stick_X_ = 0.0f;

	//�E�X�e�B�b�N��Y���̓��͗�
	float R_Stick_Y_ = 0.0f;

};


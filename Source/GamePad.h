#pragma once

#define WIN32_LEAN_AND_MEAN 

#include <Xinput.h>

/// <summary>
/// 仮想ボタン定義
/// </summary>
enum EnButton {
	enButtonUp = 0,		//上。
	enButtonDown,		//下
	enButtonLeft,		//左
	enButtonRight,		//右
	enButtonA,			//Aボタン
	enButtonB,			//Bボタン
	enButtonX,			//Xボタン
	enButtonY,			//Yボタン
	enButtonSelect,		//セレクトボタン
	enButtonStart,		//スタートボタン
	enButtonRB1,		//RB1ボタン
	enButtonRB2,		//RB2ボタン
	enButtonRB3,		//RB3ボタン
	enButtonLB1,		//LB1ボタン
	enButtonLB2,		//LB2ボタン
	enButtonLB3,		//LB3ボタン
	enButtonNum,		//ボタンの数
};

/// <summary>
/// ゲームパッド
/// </summary>
class GamePad
{
public:
	//パッドステート
	struct PAD_STATE
	{
		XINPUT_STATE state;
		bool bConnected;
	};

	enum { MAX_PAD = 4 };

	enum class EnXInputPadState {
		Undef,		//不明。
		Connect,	//接続されている。
		Disconnect,	//接続されていない。
	};

public:
	/// <summary>
	/// デフォルト コンストラクタ
	/// </summary>
	GamePad();

	/// <summary>
	/// デフォルト デストラクタ
	/// </summary>
	~GamePad();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="padNo">パッド番号</param>
	void Init(int padNo);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	//フレームの開始時に呼ばれる関数
	static void BeginFrame();

public:
#pragma region Get Method
	//接続可能なパッドの最大数。
	//static const int GetCONNECTPADMAX() { return 4; };
	//接続可能なパッドの最大数
	static const int CONNECT_PAD_MAX = 4;

	/// <summary>
	/// ボタンのトリガー判定
	/// </summary>
	/// <param name="button"></param>
	/// <returns>trueの場合トリガー入力</returns>
	bool IsTrigger(EnButton button)const
	{
		return this->trigger_[button] != 0;
	}

	/// <summary>
	/// ボタンが押されているか判定
	/// </summary>
	/// <param name="button"></param>
	/// <returns>trueが返ってきたら押されている</returns>
	bool IsPress(EnButton button)const
	{
		return this->press_[button] != 0;
	}

	/// <summary>
	/// 何かのボタンが押されているか判定
	/// </summary>
	/// <returns>trueの場合は押されている</returns>
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
	/// LスティックのX軸の入力量を取得
	/// </summary>
	/// <returns>-1.0～1.0の正規化された値を返す</returns>
	float GetLStickXF() const
	{
		return this->L_Stick_X_;
	}

	/// <summary>
	/// LスティックのY軸の入力量を取得
	/// </summary>
	/// <returns>-1.0～1.0の正規化された値を返す</returns>
	float GetLStickYF()const
	{
		return this->L_Stick_Y_;
	}

	/// <summary>
	/// RスティックのX軸の入力量を取得
	/// </summary>
	/// <returns>-1.0～1.0の正規化された値を返す</returns>
	float GetRStickXF()const
	{
		return this->R_Stick_X_;
	}

	/// <summary>
	/// RスティックのY軸の入力量を取得
	/// </summary>
	/// <returns>-1.0～1.0の正規化された値を返す</returns>
	float GetRStickYF()const
	{
		return this->R_Stick_Y_;
	}

#pragma endregion

	static EnXInputPadState pad_States_[MAX_PAD];
private:
	//パッドステート
	PAD_STATE state_;

	//パッド番号
	int pad_No_;

	//トリガー入力のフラグ
	int trigger_[enButtonNum];

	//press入力のフラグ
	int press_[enButtonNum];

	//左スティックのX軸の入力量
	float L_Stick_X_ = 0.0f;

	//左スティックのY軸の入力量
	float L_Stick_Y_ = 0.0f;

	//右スティックのX軸の入力量
	float R_Stick_X_ = 0.0f;

	//右スティックのY軸の入力量
	float R_Stick_Y_ = 0.0f;

};


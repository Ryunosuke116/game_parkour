
#include "Input.h"
#include "DebugDrawer.h"

/// <summary>
/// コンストラクタ
/// </summary>
Input::Input()
	: nowFrameInput(0),
	nowFrameNewInput(0),
	input_direct({NULL}),
	joyPad(new JoyPad{ 0.0f, 0.0f, 0.0f, 0.0f }),
	OldJoyPad(new JoyPad{ 0.0f, 0.0f, 0.0f, 0.0f })
{
	SetJoypadDeadZone(DX_INPUT_KEY_PAD1, 0.4f);
}

/// <summary>
/// デストラクタ
/// </summary>
Input::~Input()
{
	// 処理なし
}

/// <summary>
/// 更新
/// </summary>
void Input::Update()
{
	// ひとつ前のフレームの入力を変数にとっておく
	int Old = nowFrameInput;

	// 現在の入力状態を取得
	nowFrameInput = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	nowDirectInput = GetJoypadDirectInputState(DX_INPUT_KEY_PAD1, &input_direct);

	// 今のフレームで新たに押されたボタンのビットだけ立っている値を nowFrameNewInput に代入する
	nowFrameNewInput = nowFrameInput & ~Old;

	int inputXLeft;
	int inputYLeft;
	int inputXRight;
	int inputYRight;

	//左スティック
	GetJoypadAnalogInput(&inputXLeft, &inputYLeft, DX_INPUT_KEY_PAD1);
	//右スティック
	GetJoypadAnalogInputRight(&inputXRight, &inputYRight, DX_INPUT_KEY_PAD1);

	OldJoyPad->inputXLeft = joyPad->inputXLeft;
	OldJoyPad->inputYLeft = joyPad->inputYLeft;

	joyPad->inputXLeft = static_cast<float>(inputXLeft);
	joyPad->inputYLeft = static_cast<float>(inputYLeft);
	joyPad->inputXRight = static_cast<float>(inputXRight);
	joyPad->inputYRight = static_cast<float>(inputYRight);
}
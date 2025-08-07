
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
	joyPad_old(new JoyPad{ 0.0f, 0.0f, 0.0f, 0.0f })
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
	DebugDrawer::Instance().InformationInput_string_int("input_direct.z %d\n", input_direct.Z);
	DebugDrawer::Instance().InformationInput_string_int("input %d\n", nowFrameInput);

	// 今のフレームで新たに押されたボタンのビットだけ立っている値を nowFrameNewInput に代入する
	nowFrameNewInput = nowFrameInput & ~Old;

	int input_x_left;
	int input_y_left;
	int input_x_Right;
	int input_y_Right;

	//左スティック
	GetJoypadAnalogInput(&input_x_left, &input_y_left, DX_INPUT_KEY_PAD1);
	//右スティック
	GetJoypadAnalogInputRight(&input_x_Right, &input_y_Right, DX_INPUT_KEY_PAD1);

	joyPad_old->input_x_left = joyPad->input_x_left;
	joyPad_old->input_y_left = joyPad->input_y_left;

	joyPad->input_x_left = static_cast<float>(input_x_left);
	joyPad->input_y_left = static_cast<float>(input_y_left);
	joyPad->input_x_Right = static_cast<float>(input_x_Right);
	joyPad->input_y_Right = static_cast<float>(input_y_Right);
}
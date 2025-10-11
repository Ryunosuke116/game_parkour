#pragma once
#include "DxLib.h"

class Input
{
private:
	int		nowFrameInput;				// 現在のフレームで押されているボタン
	int		nowFrameNewInput;			// 現在のフレームで新たに押されたボタン
	int		nowDirectInput;

	DINPUT_JOYSTATE input_direct;
	

	struct JoyPad
	{
		float input_x_left;
		float input_y_left;
		float input_x_Right;
		float input_y_Right;
	};

	JoyPad* joyPad;
	JoyPad* joyPad_old;
public:
	
	Input();
	~Input();

	void Update();

	int GetNowFrameInput() const { return nowFrameInput; }
	int GetNowFrameNewInput() const { return nowFrameNewInput; }
	int GetNowFrameInput_direct_Z()const { return input_direct.Z; }
	float GetJoyPadXLeft() const { return joyPad->input_x_left; }
	float GetJoyPadYLeft() const { return joyPad->input_y_left; }
	float GetJoyPadXRight() const { return joyPad->input_x_Right; }
	float GetJoyPadYRight() const { return joyPad->input_y_Right; }
	float GetOldJoyPadXLeft() const { return joyPad_old->input_x_left; }
	float GetOldJoyPadYLeft() const { return joyPad_old->input_y_left; }
	float GetOldJoyPadXRight() const { return joyPad_old->input_x_Right; }
	float GetOldJoyPadYRight() const { return joyPad_old->input_y_Right; }
};


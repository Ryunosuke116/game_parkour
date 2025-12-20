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
		float inputXLeft;
		float inputYLeft;
		float inputXRight;
		float inputYRight;
	};

	JoyPad* joyPad;
	JoyPad* OldJoyPad;
public:
	
	Input();
	~Input();

	void Update();

	int GetNowFrameInput() const { return nowFrameInput; }
	int GetNowFrameNewInput() const { return nowFrameNewInput; }
	int GetNowFrameInput_direct_Z()const { return input_direct.Z; }
	float GetJoyPadXLeft() const { return joyPad->inputXLeft; }
	float GetJoyPadYLeft() const { return joyPad->inputYLeft; }
	float GetJoyPadXRight() const { return joyPad->inputXRight; }
	float GetJoyPadYRight() const { return joyPad->inputYRight; }
	float GetOldJoyPadXLeft() const { return OldJoyPad->inputXLeft; }
	float GetOldJoyPadYLeft() const { return OldJoyPad->inputYLeft; }
	float GetOldJoyPadXRight() const { return OldJoyPad->inputXRight; }
	float GetOldJoyPadYRight() const { return OldJoyPad->inputYRight; }
};


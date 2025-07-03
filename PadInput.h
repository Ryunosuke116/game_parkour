#pragma once
#include "Input.h"

class PadInput
{
private:

	//int nowFrameInput;
	//int nowFrameNewInput;

	PadInput() = default;
	~PadInput() = default;

	static PadInput* instance;
public:

	/*PadInput();
	~PadInput();

	void Update() override;

	int GetNowFrameInput() const override { return nowFrameInput; }
	int GetNowFrameNewInput() const override { return nowFrameNewInput; }*/

	static Input* input;

	static void Initialize();
	static void Update();

	static bool isUp();
	static bool isDown();
	static bool isRight();
	static bool isLeft();
	static bool isJump();
	static bool isRoll();
	static float GetJoyPad_x_left();
	static float GetJoyPad_y_left();
	static float GetJoyPad_x_right();
	static float GetJoyPad_y_right();
};


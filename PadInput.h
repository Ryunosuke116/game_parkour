#pragma once
#include "Input.h"

class PadInput
{
private:

	PadInput() = default;
	~PadInput() = default;

	static PadInput* instance;
public:

	static Input* input;

	static void Initialize();
	static void Update();

	static bool isUp();
	static bool isDown();
	static bool isRight();
	static bool isLeft();
	static bool isJump();
	static bool isRoll();
};


#pragma once
#include "Input.h"

///////////////////////////////////////
// 一人プレイ前提の作り
///////////////////////////////////////

class PadInput
{
private:
	PadInput() = default;
	~PadInput() = default;

	static Input* input;

public:
	static void Initialize();
	static void finalize();
	static void Update();

	static bool isUp();
	static bool isDown();
	static bool isRight();
	static bool isLeft();
	static bool isJump();
	static bool IsPushRT();
	static bool IsPushA();
	static bool IsPushR();
	static bool IsPushLT();
	static float GetJoyPadXLeft();
	static float GetJoyPadYLeft();
	static float GetJoyPadXRight();
	static float GetJoyPadYRight();
	static float GetOldJoyPadXLeft();
	static float GetOldJoyPadYLeft();
	static float GetOldJoyPadXRight();
	static float GetOldJoyPadYRight();
};


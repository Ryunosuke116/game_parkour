#include "PadInput.h"
#include "DxLib.h"  

void PadInput::Initialize()
{
    input = new Input;
}

void PadInput::finalize()
{
    delete(input);
}

void PadInput::Update()
{
    input->Update();
}

/// <summary>
/// 上入力
/// </summary>
/// <param name="input"></param>
/// <returns></returns>
bool PadInput::isUp()
{
    if (input->GetNowFrameInput() & PAD_INPUT_UP ||
        CheckHitKey(KEY_INPUT_UP))
    {
        return true;
    }

    return false;
}

/// <summary>
/// 下入力
/// </summary>
/// <param name="input"></param>
/// <returns></returns>
bool PadInput::isDown()
{
    if (input->GetNowFrameInput() & PAD_INPUT_DOWN ||
        CheckHitKey(KEY_INPUT_DOWN))
    {
        return true;
    }
    return false;
}

/// <summary>
/// 右入力
/// </summary>
/// <param name="input"></param>
/// <returns></returns>
bool PadInput::isRight()
{
    if (input->GetNowFrameInput() & PAD_INPUT_RIGHT ||
        CheckHitKey(KEY_INPUT_RIGHT))
    {
        return true;
    }
    return false;
}

/// <summary>
/// 左入力
/// </summary>
/// <param name="input"></param>
/// <returns></returns>
bool PadInput::isLeft()
{
    if (input->GetNowFrameInput() & PAD_INPUT_LEFT ||
        CheckHitKey(KEY_INPUT_LEFT))
    {
        return true;
    }
    return false;
}

/// <summary>
/// ジャンプ入力
/// </summary>
/// <param name="input"></param>
/// <returns></returns>
bool PadInput::isJump()
{
    if (CheckHitKey(KEY_INPUT_SPACE) ||
        input->GetNowFrameNewInput() & PAD_INPUT_B)
    {
        return true;
    }
    return false;
}

/// <summary>
/// ロール入力
/// </summary>
/// <param name="input"></param>
/// <returns></returns>
bool PadInput::isRoll()
{
    if (CheckHitKey(KEY_INPUT_F) ||
        input->GetNowFrameNewInput() & PAD_INPUT_R)
    {
        return true;
    }
    return false;
}

float PadInput::GetJoyPad_x_left()
{
    return input->GetJoyPad_x_left();
}

float PadInput::GetJoyPad_y_left()
{
    return input->GetJoyPad_y_left();
}

float PadInput::GetJoyPad_x_right()
{
    return input->GetJoyPad_x_right();
}

float PadInput::GetJoyPad_y_right()
{
    return input->GetJoyPad_y_right();
}

//PadInput* PadInput::instance = nullptr;
Input* PadInput::input = nullptr;
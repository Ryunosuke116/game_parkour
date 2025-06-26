#include "PadInput.h"
#include "DxLib.h"  

void PadInput::Initialize()
{
    input = new Input;
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
bool PadInput::PadInput::isUp()
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
bool PadInput::PadInput::isDown()
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
bool PadInput::PadInput::isRight()
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
bool PadInput::PadInput::isLeft()
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
bool PadInput::PadInput::isJump()
{
    if (CheckHitKey(KEY_INPUT_SPACE) ||
        input->GetNowFrameNewInput() & PAD_INPUT_A)
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
bool PadInput::PadInput::isRoll()
{
    if (CheckHitKey(KEY_INPUT_F) ||
        input->GetNowFrameNewInput() & PAD_INPUT_B)
    {
        return true;
    }
    return false;
}

PadInput* PadInput::instance = nullptr;
Input* PadInput::input = nullptr;
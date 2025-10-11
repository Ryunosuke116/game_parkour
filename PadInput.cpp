#include "PadInput.h"
#include "DxLib.h"  

void PadInput::Initialize()
{
    if (input == NULL)
    {
        input = new Input;
    }
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
bool PadInput::IsPushRT()
{
    if (CheckHitKey(KEY_INPUT_F) ||
        input->GetNowFrameInput_direct_Z() < 0)
    {
        return true;
    }
    return false;
}

bool PadInput::IsPushLT()
{
    if (CheckHitKey(KEY_INPUT_L) ||
        input->GetNowFrameInput_direct_Z() > 0)
    {
        return true;
    }
    return false;
}

bool PadInput::IsPushA()
{
    if ((input->GetNowFrameNewInput() & PAD_INPUT_A ||
        CheckHitKey(KEY_INPUT_SPACE)))
    {
        return true;
    }
    return false;
}

bool PadInput::IsPushR()
{
    if ((input->GetNowFrameNewInput() & PAD_INPUT_6 ||
        CheckHitKey(KEY_INPUT_SPACE)))
    {
        return true;
    }
    return false;
}

float PadInput::GetJoyPadXLeft()
{
    return input->GetJoyPadXLeft();
}

float PadInput::GetJoyPadYLeft()
{
    return input->GetJoyPadYLeft();
}

float PadInput::GetJoyPadXRight()
{
    return input->GetJoyPadXRight();
}

float PadInput::GetJoyPadYRight()
{
    return input->GetJoyPadYRight();
}

////////////////////////////////////
//前フレーム
////////////////////////////////////

float PadInput::GetOldJoyPadXLeft()
{
    return input->GetOldJoyPadXLeft();
}

float PadInput::GetOldJoyPadYLeft()
{
    return input->GetOldJoyPadYLeft();
}

float PadInput::GetOldJoyPadXRight()
{
    return input->GetOldJoyPadXRight();
}

float PadInput::GetOldJoyPadYRight()
{
    return input->GetOldJoyPadYRight();
}

Input* PadInput::input = nullptr;
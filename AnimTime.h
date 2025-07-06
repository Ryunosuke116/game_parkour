#pragma once

enum animNum : int
{
	braced_Hang_To_Crouch,		//しゃがんでぶらさがる
	falling_Idle,				//落ちているとき
	falling_To_Roll,			//着地して転がる
	hangring_Idle,				//ぶらさがる
	hard_Landing,				//着地する
	idle,						//静止時
	idle_To_Sprint,				//走り出し
	jump,						//ジャンプ
	jump_Over,					//ロールジャンプ
	quick_Roll,					//転がる
	run,						//走る
	run_Jump,					//走りながらジャンプ
	run_To_Stop,				//止まる
	running_Forward_Flip		//走りながら回転ジャンプ
};

////上入力されたとき
 //if (PadInput::isUp())
 //{
 //    if (animNumber_Now != animNum::run && !playerData.isJump)
 //    {
 //       // ChangeMotion(animNum::run, PlayAnimSpeed);
 //    }
 //    moveVec = VAdd(moveVec, upMove);
 //    playerData.isMove = true;
 //    playerData.isStopRun = true;
 //}

 ////下入力されたとき
 //if (PadInput::isDown())
 //{
 //    if (animNumber_Now != animNum::run && !playerData.isJump)
 //    {
 //       // ChangeMotion(animNum::run, PlayAnimSpeed);
 //    }
 //    moveVec = VAdd(moveVec, VScale(upMove, -1.0f));
 //    playerData.isMove = true;
 //    playerData.isStopRun = true;
 //}

 ////左入力されたとき
 //if (PadInput::isLeft())
 //{
 //    if (animNumber_Now != animNum::run && !playerData.isJump)
 //    {
 //       // ChangeMotion(animNum::run, PlayAnimSpeed);
 //    }
 //    moveVec = VAdd(moveVec, rightMove);
 //    playerData.isMove = true;
 //    playerData.isStopRun = true;
 //}

 ////右入力されたとき
 //if (PadInput::isRight())
 //{
 //    if (animNumber_Now != animNum::run && !playerData.isJump)
 //    {
 //        //ChangeMotion(animNum::run, PlayAnimSpeed);
 //    }
 //    moveVec = VAdd(moveVec, VScale(rightMove, -1.0f));
 //    playerData.isMove = true;
 //    playerData.isStopRun = true;
 //}

/*   moveVec = VGet(PadInput::GetJoyPad_x_left(),
       0.0f, -PadInput::GetJoyPad_y_left());*/
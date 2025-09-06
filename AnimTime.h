#pragma once

enum animNum : int
{
	braced_Hang_To_Crouch,		//しゃがんでぶらさがる
	falling_Idle,				//落ちているとき
	falling_To_Roll,			//着地して転がる
	hanging_Idle,				//ぶらさがる
	hard_Landing,				//着地する
	idle,						//静止時
	idle_To_Sprint,				//走り出し
	jump,						//ジャンプ
	roll,					//ロールジャンプ
	quick_Roll,					//転がる
	run,						//走る
	run_Jump,					//走りながらジャンプ
	run_To_Stop,				//止まる
	running_Forward_Flip,		//走りながら回転ジャンプ
	running_turn_left,			//急回転
	running_turn_right,
	walk,						//歩く
	wallClimb					//駆け上がる
};

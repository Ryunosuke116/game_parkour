#pragma once

enum animNum : int
{
	bracedHangToCrouch,			//しゃがんでぶらさがる
	fallingIdle,				//落ちているとき
	fallingToRoll,				//着地して転がる
	hangingIdle,				//ぶらさがる
	hardLanding,				//着地する
	idle,						//静止時
	idleToSprint,				//走り出し
	jump,						//ジャンプ
	roll,						//ロールジャンプ
	quickRoll,					//転がる
	run,						//走る
	runJump,					//走りながらジャンプ
	runToStop,					//止まる
	runningForwardFlip,			//走りながら回転ジャンプ
	runningTurnLeft,			//急回転
	runningTurnRight,
	walk,						//歩く
	wallClimb					//駆け上がる
};

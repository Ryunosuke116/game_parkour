#pragma once
struct PlayerData
{
	bool isIdle;					//止まっているか
	bool isMove;					//動いているか
	bool isRun;						//走っているか
	bool isJump;					//ジャンプしたか
	bool isJump_first;				//一回目のジャンプをしたか
	bool isJump_second;				//二段ジャンプしたか
	bool isJump_PlayAnim;			//ジャンプのアニメを再生するか
	bool isJump_run_playAnim;		//ランジャンプのアニメを再生するか
	bool isRoll;					//転がるか
	bool isUse_Roll;				//ロールアクションを終えたか
	bool isGround;					//接地しているか
	bool isHitWall;					//壁に当たっているか
	bool isSprint;					//走り出しか
	bool isStopRun;					//走り終わったか
	bool isJumpAll;					//全てのジャンプが完了したか
	bool isFalling;					//落下中か
	bool isHanging;					//崖つかみ中か
	bool isHang_to_Crouch;			//上に上がる
	bool isSlip;
	bool isTurn_right;
	bool isRun_wall;
	bool isLanding;					//着地したか
	bool isUse_wallJump;			//壁ジャンプを使用できるか
};
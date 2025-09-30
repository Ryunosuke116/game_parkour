#pragma once
#include <compare> 
struct PlayerData
{
	bool isGround;					//接地しているか
	bool isIdle;					//止まっているか
	bool isMove;					//動いているか
	bool isWalk;
	bool isRun;						//走っているか
	bool isRunWall;
	bool isDash;
	bool isJump;					//ジャンプしたか
	bool isJumpFirst;				//一回目のジャンプをしたか
	bool isJumpSecond;				//二段ジャンプしたか
	bool isJumpPlayAnim;			//ジャンプのアニメを再生するか
	bool isJumpAll;					//全てのジャンプが完了したか
	bool isUseWallJump;				//壁ジャンプを使用できるか
	bool isWalljump;				//壁ジャンプしているか
	bool isRoll;					//転がるか
	bool isUseRoll;					//ロールアクションを終えたか
	bool isSprint;					//走り出しか
	bool isStopRun;					//走り終わったか
	bool isFalling;					//落下中か
	bool isHanging;					//崖をつかむか
	bool isHanging_now;				//崖つかみ中か
	bool isUseHanging;				//崖掴みが可能か
	bool isHangToCrouch;			//上に上がる
	bool isLanding;					//着地したか
	bool isWallClimb;
	bool isVictory;					//勝利モーションをしているか

	auto operator<=>(const PlayerData&) const = default;
};
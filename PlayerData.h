#pragma once
#include <compare> 
struct PlayerData
{
	bool isGround;					//接地しているか
	bool isIdle;					//止まっているか
	bool isMove;					//動いているか
	bool isWalk;
	bool isRun;						//走っているか
	bool isRun_wall;
	bool isJump;					//ジャンプしたか
	bool isJump_first;				//一回目のジャンプをしたか
	bool isJump_second;				//二段ジャンプしたか
	bool isJump_PlayAnim;			//ジャンプのアニメを再生するか
	bool isJumpAll;					//全てのジャンプが完了したか
	bool isUse_wallJump;			//壁ジャンプを使用できるか
	bool isWalljump;				//壁ジャンプしているか
	bool isRoll;					//転がるか
	bool isUse_Roll;				//ロールアクションを終えたか
	bool isPossible_wallRun;		//壁走りが可能か
	bool isSprint;					//走り出しか
	bool isStopRun;					//走り終わったか
	bool isFalling;					//落下中か
	bool isHanging;					//崖をつかむか
	bool isHanging_now;				//崖つかみ中か
	bool isUse_Hanging;				//崖掴みが可能か
	bool isHang_to_Crouch;			//上に上がる
	bool isLanding;					//着地したか

	auto operator<=>(const PlayerData&) const = default;

};
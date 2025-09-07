#pragma once
#include <memory>
#include <vector>
#include "BaseObject.h"
#include "PlayerData.h"

class Input;
class Player;

class PlayerStateBase
{
public:
	struct AnimState
	{
		int AttachIndex;			//アニメーション情報
		float PlayTime_anim;		//再生時間
		float TotalPlayTime_anim;	//総再生時間
		float PlayAnimSpeed;		//アニメーションスピード
	};

	PlayerStateBase(int& modelHandle,
		AnimState& oldAnimState,
		AnimState& nowAnimState);
	~PlayerStateBase() {};

	//純粋仮想関数
	virtual void Initialize(int& modelHandle,const int changeNum, Player& player);
	virtual std::pair<VECTOR, PlayerData> Update(const VECTOR& cameraDirection,
		const std::vector<std::weak_ptr<BaseObject>>& fieldObjects, Player& player)abstract;
	virtual VECTOR Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)abstract;
	virtual void Enter(PlayerData& playerData) abstract;		//状態に入ったとき
	virtual void Exit(PlayerData& playerData) abstract;			//状態を抜けるとき

	virtual bool MotionUpdate(PlayerData& playerData);
	virtual VECTOR Move(const VECTOR& cameraDirection, PlayerData& playerData);
	virtual void RollMove(PlayerData& playerData);
	virtual void JumpMove(PlayerData& playerData, Player& player);
	virtual void WallRunMove(PlayerData& playerData, Player& player);

	void SetOldAnimState();
	void ResetOldAnimState();
	void ResetNowAnimState();
	bool Draw();
	void SwitchingAnimation(const int& animNum);
	void FlagReset_jump(PlayerData& playerData);

	//////////////////////////////////////////////
	// ゲッター
	//////////////////////////////////////////////
	//int GetPrevAttachIndex() { return oldAnimState.AttachIndex; }
	AnimState GetOldAnimState() const { return oldAnimState; }
	AnimState GetNowAnimState() const { return nowAnimState; }
	float GetAnimBlendRate()const { return animBlendRate; }
	bool GetIsChangeState()const { return isChangeState; }

	void SetPlayAnimSpeed_now(const float set) { nowAnimState.PlayTime_anim = set; }
	void SetAnimNumber_old(const int num) { animNumber_old = num; }
	void SetIsChangeState(const bool set) { isChangeState = set; }
	

protected:
	static constexpr float	AnimBlendSpeed = 0.1f;		// アニメーションのブレンド率変化速度
	static constexpr float run_wall_rotate_x = 30.0f;
	static constexpr float cliff_radius = 4.0f;

	int modelHandle;			//モデルハンドル
	int animNumber_old;
	float animBlendRate;

	bool isPush;
	bool isChangeState;

	VECTOR moveDirection;

	AnimState oldAnimState;
	AnimState nowAnimState;

	//移動
	//VECTOR moveVec;

};


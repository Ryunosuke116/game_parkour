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
		int attachIndex;			//アニメーション情報
		float playAnimTime;		//再生時間
		float TotalPlayTime_anim;	//総再生時間
		float PlayAnimSpeed;		//アニメーションスピード
	};

	PlayerStateBase(const int modelHandle);
	~PlayerStateBase() {};

	//純粋仮想関数
	virtual void Initialize(const int modelHandle,const int changeNum, Player& player);
	virtual std::pair<VECTOR, PlayerData> Update(const VECTOR& cameraDirection,
		const std::vector<std::weak_ptr<BaseObject>>& fieldObjects, Player& player)abstract;
	virtual VECTOR Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)abstract;
	virtual void Enter(AnimState& oldAnimState, AnimState& nowAnimState);		//状態に入ったとき
	virtual void Exit(PlayerData& playerData) abstract;			//状態を抜けるとき

	virtual bool MotionUpdate(PlayerData& playerData);
	virtual VECTOR Move(const VECTOR& cameraDirection, PlayerData& playerData);
	virtual void RollMove(PlayerData& playerData);
	virtual void JumpMove(PlayerData& playerData, Player& player);
	virtual void WallRunMove(PlayerData& playerData, Player& player, const std::shared_ptr<BaseObject>& collisionObjects);

	void SetOldAnimState();
	void ResetOldAnimState();
	void ResetNowAnimState();
	void Draw();
	void SwitchingAnimation(const int& animNum);
	void ResetIsJumps(PlayerData& playerData);

	//////////////////////////////////////////////
	// ゲッター
	//////////////////////////////////////////////
	//int GetPrevAttachIndex() { return oldAnimState.attachIndex; }
	AnimState GetOldAnimState() const { return oldAnimState; }
	AnimState GetNowAnimState() const { return nowAnimState; }
	float GetAnimBlendRate()const { return animBlendRate; }
	bool GetIsChangeState()const { return isChangeState; }

	void SetPlayAnimSpeed_now(const float set) { nowAnimState.playAnimTime = set; }
	void SetAnimNumber_old(const int num) { animNumber_old = num; }
	void SetIsChangeState(const bool set) { isChangeState = set; }
	
protected:
	static constexpr float	AnimBlendSpeed = 0.1f;		// アニメーションのブレンド率変化速度
	static constexpr float runWallRotateX = 30.0f;
	static constexpr float kCliffRadius = 4.0f;			//崖掴み判定用の半径

	int modelHandle;			//モデルハンドル
	int animNumber_old;
	float animBlendRate;

	bool isPush;
	bool isChangeState;

	VECTOR moveDirection;

	AnimState oldAnimState;
	AnimState nowAnimState;
};


#pragma once
#include "PlayerStateActionBase.h"
#include "PlayerCalculation.h"
#include "BaseChara.h"
#include "CollisionManager.h"
#include "Calculation.h"
#include  "nlohmann/json.hpp"

class Player : public BaseChara
{
private:
	static constexpr float modelScale = 0.06f;
	static constexpr float MaxMoveSpeed = 1.6f;	    // 移動速度
	static constexpr float rollMoveSpeed_max = 2.5f;	//ロール速度
	static constexpr float angleSpeed = 0.3f;
	static constexpr float addJumpPower = 2.0f;		//ジャンプパワー
	static constexpr float gravity = -0.06f;
	static constexpr float addTopPos = 17.0f;
	static constexpr float addBottomPos = 3.0f;
	static constexpr float radius = 3.5f;

	VECTOR linePos_end;
	VECTOR footPosition;
	VECTOR centerPosition;
	VECTOR topPosition;
	VECTOR bottomPosition;
	VECTOR moveVec;
	VECTOR hangingPoint;
	VECTOR headPos;
	VECTOR handPos_right;
	VECTOR handPos_left;
	VECTOR handCenterPos;
	VECTOR moveDirection_now;

	bool isPush;					//ボタンを押したか
	bool isChangeState;				//アニメーションを変更するか
	bool isCalc;
	bool isCalc_moveVec;

	//他クラス
	std::shared_ptr<PlayerStateActionBase> nowState = NULL;
	PlayerStateActionBase::AnimState oldAnimState;
	PlayerStateActionBase::AnimState nowAnimState;
	PlayerStateActionBase::PlayerData playerData;
	std::shared_ptr<CollisionManager> collisionManager = NULL;
	std::shared_ptr<PlayerCalculation> playerCalculation = NULL;
public:
	Player(nlohmann::json jsonData);
	~Player();

	void Initialize();
	void Update()override;
	void Update(const VECTOR& cameraDirection, const int mapHandle);
	bool Draw();
	VECTOR Move(VECTOR& moveVec, const VECTOR& cameraDirection);
	void JumpMove();
	void RollMove();
	void HangingMove();
	void Hang_to_CrouchMove(const int mapHandle);
	void ChangeState();
	void SettingRay();
	void HangingCheck(const int mapHandle);
	void NormalMove(const int mapHandle);
	void Reset();

	void Command(const VECTOR& cameraDirection);

	void SetOldAnimState(PlayerStateActionBase::AnimState animState);
	void SetNowAnimState(PlayerStateActionBase::AnimState animState);

	

	//////////////////////////////////
	//　ゲッター
	///////////////////////////////////

	VECTOR GetCenterPos() { return centerPosition; }
	VECTOR GetFootPos() { return footPosition; }
	VECTOR GetTopPos() { return topPosition; }
	VECTOR GetBottomPos() { return bottomPosition; }
	VECTOR GetMoveVec() { return moveVec; }
	VECTOR GetlinePos_end() { return linePos_end; }
	VECTOR GetHangingPoint() { return hangingPoint; }
	VECTOR GetHeadPos() { return headPos; }
	VECTOR GetHandPos_right() { return handPos_right; }
	VECTOR GetHandPos_left() { return handPos_left; }
	bool GetIsGround() { return playerData.isGround; }
	int GetModelHandle() { return modelHandle; }
	PlayerStateActionBase::PlayerData GetData() { return playerData; }
	float GetAddTopPos() { return addTopPos; }
	float GetAddBottomPos() { return addBottomPos; }
	float GetRadius() { return radius; }

	//////////////////////////////////
	/// セッター
	/////////////////////////////////
	void SetIsGround(bool flag) { playerData.isGround = flag; }
	void SetPos(VECTOR newPos) { position = newPos; }
};


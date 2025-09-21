#pragma once
#include "PlayerStateBase.h"
#include "PlayerCalculation.h"
#include "BaseChara.h"
#include "CollisionManager.h"
#include "Calculation.h"
#include "AnimationChanger.h"
#include  "nlohmann/json.hpp"
#include "CoinObserver.h"
#include "ISoundPlayer.h"
#include "IEffectManager.h"

class EffectManager;

class Player : 
	public BaseChara,
	public CoinObserver
{
public:
	Player();
	~Player();

	void Create()override;
	void Load(const nlohmann::json& jsonData)override;
	void Initialize()override;
	void Update()override;
	void ResultCreate()override;
	void ResultInitialize()override;
	void ResultUpdate()override;

	void Update_start(const float& timer);
	void Update_finish(const float& timer);
	void DebugUpdate();
	void MoveDirectionUpdate();
	void Receive_CollisionResult();
	void ChangeState();
	void CollisionUpdate();
	void OnCoinPicked(int amount)override { coinCount += amount; }

	//////////////////////////////////
	//　ゲッター
	///////////////////////////////////

	VECTOR GetTopPos() const { return positionData.capsuleTopPosition; }
	VECTOR GetBottomPos() const { return positionData.capsuleBottomPosition; }
	VECTOR GetVelocity() const { return velocity; }
	VECTOR GetlinePos_end() const { return linePos_end; }
	VECTOR GetHeadPos() const { return headPos; }
	VECTOR GetNowMoveDirection() const { return nowMoveDirection; }
	VECTOR GetFaceDirection()const { return faceDirection; }
	bool GetIsGround() const { return playerData.isGround; }
	int GetNowStateNumber() const { return animationChanger->GetAnimNumber_now(); }
	PlayerData GetData() const { return playerData; }
	PlayerStateBase::AnimState GetNowAnimState() const { return nowState->GetNowAnimState(); }
	float GetRadius()const override { return radius; }
	AABB GetAABB()const { return mAABB; }

	//////////////////////////////////
	/// セッター
	/////////////////////////////////
	void SetIsGround(bool flag) { playerData.isGround = flag; }
	void SetPos(VECTOR newPos) { position = newPos; }
	void SetNowMoveDirection(const VECTOR& set) { nowMoveDirection = set; }
	void SetFaceDirection(const VECTOR& set) { faceDirection = set; }
	std::shared_ptr<PlayerCalculation> playerCalculation = NULL;
private:

	static constexpr float modelScale = 0.06f;
	static constexpr float MaxMoveSpeed = 1.4f;	    // 移動速度
	static constexpr float rollMoveSpeed_max = 2.5f;	//ロール速度
	static constexpr float rotationSpeedDegree = 0.3f;
	static constexpr float addJumpPower = 1.7f;		//ジャンプパワー
	static constexpr float gravity = -0.06f;
	static constexpr float runWallRotateX = 30.0f;
	static constexpr float entryDegree_wallRun = 30.0f;
	static constexpr float radius = 3.5f;
	static constexpr float height = 10.0f;

	VECTOR linePos_end;
	VECTOR topPosition;
	VECTOR bottomPosition;
	VECTOR moveVec_normal;
	VECTOR headPos;
	VECTOR handCenterPos;
	VECTOR nowMoveDirection;		//現在向いている方向
	VECTOR faceDirection;
	VECTOR padInput_now;

	float radian_wall;
	float degree_pad_now;
	float degree_difference;
	float degree_pad_wall_difference;
	float effectTimer;
	float start_walkTime;

	bool isPush;					//ボタンを押したか
	bool isChange_falling;				//アニメーションを変更するか
	bool isCalc;
	bool isCalcMoveVec;

	//他クラス
	PlayerData playerData;
	std::shared_ptr<PlayerStateBase>		nowState = NULL;
	std::shared_ptr<AnimationChanger>		animationChanger = NULL;
	AABB mAABB;
};


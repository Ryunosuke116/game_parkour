#pragma once
#include "PlayerStateBase.h"
#include "PlayerCalculation.h"
#include "BaseChara.h"
#include "CollisionManager.h"
#include "Calculation.h"
#include "AnimationChanger.h"
#include  "nlohmann/json.hpp"
#include "CoinObserver.h"

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

	void StartUpdate(const float timer);
	void FinishUpdate(const float timer);
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
	VECTOR GetNowMoveDirection() const { return nowMoveDirection; }
	VECTOR GetFaceDirection()const { return faceDirection; }
	bool GetIsGround() const { return playerData.isGround; }
	int GetNowStateNumber() const { return animationChanger->NowGetAnimNumber(); }
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
	void CounterplanBug();
	void EffectUpdate();

private:

	static constexpr float kModelScale = 0.06f;
	static constexpr float kMaxMoveSpeed = 1.4f;	    // 移動速度
	static constexpr float kMaxRollMoveSpeed = 2.5f;	//ロール速度
	static constexpr float kRunWallRotateX = 30.0f;
	static constexpr float radius = 3.5f;
	static constexpr float height = 10.0f;


	VECTOR normalVelocity;			//通常時の移動量
	VECTOR nowMoveDirection;		//現在向いている方向
	VECTOR faceDirection;			//キャラクターの向いている方向

	float effectTimer;
	float startWalkTime;

	bool isPush;					//ボタンを押したか
	bool isCalc;
	bool isCalcMoveVec;

	//他クラス
	PlayerData playerData;
	std::shared_ptr<PlayerStateBase>		nowState = NULL;
	std::shared_ptr<AnimationChanger>		animationChanger = NULL;
	AABB mAABB;
};


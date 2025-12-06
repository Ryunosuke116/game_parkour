#pragma once
#include "BaseChara.h"
#include "nlohmann/json.hpp"
#include "CoinObserver.h"
#include "PlayerData.h"
#include "AnimationChanger.h"
#include "PlayerStateBase.h"
#include "EffectColor.h"

class EffectManager;
class PlayerCalculation;

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
	void ReceiveCollisionResult();
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
	void OnIsChangeRank();

private:
	static constexpr float kModelScale = 0.06f;
	static constexpr float kMaxMoveSpeed = 1.4f;	    // 移動速度
	static constexpr float kMaxRollMoveSpeed = 2.5f;	//ロール速度
	static constexpr float kRunWallRotateX = 30.0f;
	static constexpr float radius = 3.5f;
	static constexpr float height = 10.0f;
	static constexpr EffectColor scoreAColor = { 255,100,100,255 };
	static constexpr EffectColor scoreBColor = { 255,100,100,255 };
	static constexpr EffectColor scoreCColor = { 255,100,100,255 };
	static constexpr EffectColor scoreDColor = { 255,100,100,255 };
	static constexpr EffectColor scoreSColor = { 255,100,100,255 };
	static constexpr EffectColor scoreSSColor = { 255,100,100,255 };
	static constexpr EffectColor scoreSSSColor = { 255,100,100,255 };

	VECTOR normalVelocity;			//通常時の移動量
	VECTOR nowMoveDirection;		//現在向いている方向
	VECTOR faceDirection;			//キャラクターの向いている方向

	float effectTimer;

	bool isPush;					//ボタンを押したか
	bool isCalcMoveVec;

	//他クラス
	PlayerData playerData;
	std::shared_ptr<PlayerStateBase>		nowState = NULL;
	std::shared_ptr<AnimationChanger>		animationChanger = NULL;
	AABB mAABB;
	std::weak_ptr<EffectManager> wpEffectManager;
};


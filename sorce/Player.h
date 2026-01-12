#pragma once
#include "BaseChara.h"
#include "nlohmann/json.hpp"
#include "CoinObserver.h"
#include "PlayerData.h"
#include "AnimationChanger.h"
#include "PlayerStateBase.h"

class EffectManager;
class PlayerCalculation;
class PlayerEffectController;

class Player : 
	public BaseChara,
	public CoinObserver
{
public:
	Player();
	~Player();

	void Create()								override;
	void Load(const nlohmann::json& jsonData)	override;
	void Initialize()							override;
	void Update()								override;
	void ResultCreate()							override;
	void ResultInitialize()						override;
	void ResultUpdate()							override;

	void StartUpdate(const float timer);
	void FinishUpdate(const float timer);
	void MoveDirectionUpdate();
	void ReceiveCollisionResult();
	void OnCoinPicked(int amount)override;

	//////////////////////////////////
	//　ゲッター
	///////////////////////////////////

	VECTOR GetTopPos()				const				{ return positionData.capsuleTopPosition; }
	VECTOR GetBottomPos()			const				{ return positionData.capsuleBottomPosition; }
	VECTOR GetVelocity()			const				{ return velocity; }
	VECTOR GetNowMoveDirection()	const				{ return nowMoveDirection; }
	VECTOR GetFaceDirection()		const				{ return faceDirection; }
	bool   GetIsGround()			const				{ return playerData.isGround; }
	int    GetNowStateNumber()		const				{ return animationChanger->NowGetAnimNumber(); }
	float  GetRadius()				const override		{ return radius; }
	PlayerData GetData()			const				{ return playerData; }
	AABB GetAABB()					const				{ return mAABB; }
	PlayerStateBase::AnimState GetNowAnimState() const	{ return nowState->GetNowAnimState(); }

	//////////////////////////////////
	/// セッター
	/////////////////////////////////
	void SetIsGround(bool flag)					{ playerData.isGround = flag; }
	void SetPos(VECTOR newPos)					{ position = newPos; }
	void SetNowMoveDirection(const VECTOR& set) { nowMoveDirection = set; }
	void SetFaceDirection(const VECTOR& set)	{ faceDirection = set; }

	std::shared_ptr<PlayerCalculation> playerCalculation = NULL;

private:
	void CounterplanBug();
	void CollisionUpdate();
	void ChangeState();
	void DebugUpdate();

private:
	static constexpr float kModelScale			= 0.06f;
	static constexpr float kMaxMoveSpeed		= 1.4f;	    // 移動速度
	static constexpr float kMaxRollMoveSpeed	= 2.5f;	//ロール速度
	static constexpr float kRunWallRotateX		= 30.0f;
	static constexpr float radius				= 3.5f;
	static constexpr float height				= 10.0f;

	VECTOR normalVelocity;			//通常時の移動量
	VECTOR nowMoveDirection;		//現在向いている方向
	VECTOR faceDirection;			//キャラクターの向いている方向

	float effectTimer;

	bool isPush;					//ボタンを押したか
	bool isCalcMoveVec;

	//他クラス
	PlayerData								playerData;
	AABB									mAABB;
	std::shared_ptr<PlayerStateBase>		nowState		 = NULL;
	std::shared_ptr<AnimationChanger>		animationChanger = NULL;
	std::shared_ptr<PlayerEffectController> effectController = NULL;
};


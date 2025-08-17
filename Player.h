#pragma once
#include "PlayerStateActionBase.h"
#include "PlayerCalculation.h"
#include "BaseChara.h"
#include "CollisionManager.h"
#include "Calculation.h"
#include "AnimationChanger.h"
#include  "nlohmann/json.hpp"
#include "CoinObserver.h"

class EffectManager;

class Player : public BaseChara, public CoinObserver
{
private:

	static constexpr float modelScale = 0.06f;
	static constexpr float MaxMoveSpeed = 1.6f;	    // 移動速度
	static constexpr float rollMoveSpeed_max = 2.5f;	//ロール速度
	static constexpr float angleSpeed = 0.3f;
	static constexpr float addJumpPower = 1.7f;		//ジャンプパワー
	static constexpr float gravity = -0.06f;
	static constexpr float run_wall_rotate_x = 30.0f;
	static constexpr float entryDegree_wallRun = 30.0f;
	static constexpr float radius = 3.5f;
	static constexpr float height = 10.0f;

	VECTOR linePos_end;
	VECTOR centerPosition;
	VECTOR topPosition;
	VECTOR bottomPosition;
	VECTOR moveVec_normal;
	VECTOR headPos;
	VECTOR handCenterPos;
	VECTOR moveDirection_now;		//現在向いている方向
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
	bool isCalc_moveVec;

	//他クラス
	PlayerData playerData;
	std::shared_ptr<PlayerStateActionBase> nowState = NULL;
	std::shared_ptr<AnimationChanger> animationChanger = NULL;

public:
	Player();
	~Player();

	void Load(const nlohmann::json& jsonData)override;
	void Initialize()override;
	void Update()override;
	void Update(const VECTOR& cameraDirection,
		std::shared_ptr<EffectManager>& effectManager,
		const std::vector<std::shared_ptr<BaseObject>>& fieldObjects);
	

	void Update_start(const float& timer);
	void Update_finish(const float& timer);
	void DebugUpdate();
	void Receive_CollisionResult();
	void ChangeState();
	void CollisionUpdate();
	void OnCoinPicked(int amount)override { coinCount += amount; }

	//////////////////////////////////
	//　ゲッター
	///////////////////////////////////

	VECTOR GetCenterPos() { return centerPosition; }
	VECTOR GetTopPos() { return positionData.position_top_Capsule; }
	VECTOR GetBottomPos() { return positionData.position_bottom_Capsule; }
	VECTOR GetMoveVec() { return moveVec; }
	VECTOR GetlinePos_end() { return linePos_end; }
	VECTOR GetHeadPos() { return headPos; }
	VECTOR GetMoveDirection_now() { return moveDirection_now; }
	bool GetIsGround() { return playerData.isGround; }
	int GetModelHandle() { return modelHandle; }
	int GetNowStateNumber() { return animationChanger->GetAnimNumber_now(); }
	PlayerData GetData() { return playerData; }
	float GetRadius()const override { return radius; }

	//////////////////////////////////
	/// セッター
	/////////////////////////////////
	void SetIsGround(bool flag) { playerData.isGround = flag; }
	void SetPos(VECTOR newPos) { position = newPos; }
	void SetMoveDirection_now(const VECTOR& set) { moveDirection_now = set; }
	std::shared_ptr<PlayerCalculation> playerCalculation = NULL;
};


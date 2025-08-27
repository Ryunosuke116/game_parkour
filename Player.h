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

class Player : public BaseChara, public CoinObserver
{
public:
	Player();
	~Player();

	void Create()override {}
	void Load(const nlohmann::json& jsonData)override;
	void Initialize()override;
	void Update(ObjectMediator& objectMediator)override;

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

	VECTOR GetCenterPos() const { return centerPosition; }
	VECTOR GetTopPos() const { return positionData.position_top_Capsule; }
	VECTOR GetBottomPos() const { return positionData.position_bottom_Capsule; }
	VECTOR GetMoveVec() const { return moveVec; }
	VECTOR GetlinePos_end() const { return linePos_end; }
	VECTOR GetHeadPos() const { return headPos; }
	VECTOR GetMoveDirection_now() const { return moveDirection_now; }
	bool GetIsGround() const { return playerData.isGround; }
	int GetModelHandle() const { return modelHandle; }
	int GetNowStateNumber() const { return animationChanger->GetAnimNumber_now(); }
	PlayerData GetData() const { return playerData; }
	float GetRadius()const override { return radius; }

	//////////////////////////////////
	/// セッター
	/////////////////////////////////
	void SetIsGround(bool flag) { playerData.isGround = flag; }
	void SetPos(VECTOR newPos) { position = newPos; }
	void SetMoveDirection_now(const VECTOR& set) { moveDirection_now = set; }
	std::shared_ptr<PlayerCalculation> playerCalculation = NULL;
private:

	static constexpr float modelScale = 0.06f;
	static constexpr float MaxMoveSpeed = 1.4f;	    // 移動速度
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
	std::shared_ptr<PlayerStateBase>		nowState = NULL;
	std::shared_ptr<AnimationChanger>		animationChanger = NULL;

};


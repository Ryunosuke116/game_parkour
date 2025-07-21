#pragma once
#include "PlayerStateActionBase.h"
#include "PlayerCalculation.h"
#include "BaseChara.h"
#include "CollisionManager.h"
#include "Calculation.h"
#include "AnimationChanger.h"
#include  "nlohmann/json.hpp"

class Player : public BaseChara
{
private:

	using ObjectList = std::vector<std::shared_ptr<BaseObject>>;

	static constexpr float modelScale = 0.06f;
	static constexpr float MaxMoveSpeed = 1.6f;	    // 移動速度
	static constexpr float rollMoveSpeed_max = 2.5f;	//ロール速度
	static constexpr float angleSpeed = 0.3f;
	static constexpr float addJumpPower = 1.7f;		//ジャンプパワー
	static constexpr float gravity = -0.06f;
	static constexpr float run_wall_rotate_x = 30.0f;
	static constexpr float entryDegree_wallRun = 20.0f;

	VECTOR linePos_end;
	VECTOR centerPosition;
	VECTOR topPosition;
	VECTOR bottomPosition;
	VECTOR moveVec;
	VECTOR moveVec_normal;
	VECTOR hangingPoint;
	VECTOR headPos;
	VECTOR handPos_right;
	VECTOR handPos_left;
	VECTOR handCenterPos;
	VECTOR moveDirection_now;		//現在向いている方向
	VECTOR padInput_now;

	float radian_wall;
	float degree_pad_now;
	float degree_difference;
	float degree_pad_wall_difference;

	bool isPush;					//ボタンを押したか
	bool isChange_falling;				//アニメーションを変更するか
	bool isCalc;
	bool isCalc_moveVec;

	//他クラス
	std::shared_ptr<PlayerStateActionBase> nowState = NULL;
	PlayerStateActionBase::AnimState oldAnimState;
	PlayerStateActionBase::AnimState nowAnimState;
	PlayerStateActionBase::PlayerData playerData;
	std::shared_ptr<CollisionManager> collisionManager = NULL;
	std::shared_ptr<PlayerCalculation> playerCalculation = NULL;
	CollisionManager::PositionData positionData;
	std::shared_ptr<AnimationChanger> animationChecger = NULL;

public:
	Player(nlohmann::json jsonData);
	~Player();

	void Initialize();
	void Update()override;
	void Update(const VECTOR& cameraDirection,
		const std::vector<std::shared_ptr<BaseObject>>& fieldObjects);
	bool Draw();
	VECTOR Move(VECTOR& moveVec, const VECTOR& cameraDirection);
	void JumpMove();
	void RollMove();
	void HangingMove();
	void Hang_to_CrouchMove(const std::vector<std::shared_ptr<BaseObject>>& fieldObjects);
	void SettingRay();
	void HangingCheck(const std::vector<std::shared_ptr<BaseObject>>& fieldObjects);
	void NormalMove(const std::vector<std::shared_ptr<BaseObject>>& fieldObjects);
	void StateUpdate(const std::vector<std::shared_ptr<BaseObject>>& fieldObjects);
	void Command(const VECTOR& cameraDirection);
	void WallRunUpdate(const std::vector<std::shared_ptr<BaseObject>>& fieldObjects);

	//////////////////////////////////
	//　ゲッター
	///////////////////////////////////

	VECTOR GetCenterPos() { return centerPosition; }
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
	float GetRadius() { return positionData.radius; }

	//////////////////////////////////
	/// セッター
	/////////////////////////////////
	void SetIsGround(bool flag) { playerData.isGround = flag; }
	void SetPos(VECTOR newPos) { position = newPos; }
};


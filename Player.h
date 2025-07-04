#pragma once
#include "PlayerStateActionBase.h"
#include "PlayerCalclation.h"
#include "BaseChara.h"
#include "CollisionManager.h"

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
	VECTOR moveVec_memory;
	VECTOR hangringPoint;
	VECTOR headPos;
	VECTOR handPos_right;
	VECTOR handPos_left;
	VECTOR nearestPoint;
	VECTOR handCenterPos;

	bool isPush;					//ボタンを押したか
	bool isChageState;				//アニメーションを変更するか
	bool isCalc;

	/*struct PadInput
	{
		bool isUp(Input& input);
		bool isDown(Input& input);
		bool isRight(Input& input);
		bool isLeft(Input& input);
		bool isJump(Input& input);
		bool isRoll(Input& input);
	};*/

	//PadInput padInput;

	//他クラス
	std::shared_ptr<PlayerStateActionBase> nowState = NULL;
	PlayerStateActionBase::OldAnimState oldAnimState;
	PlayerStateActionBase::NowAnimState nowAnimState;
	PlayerStateActionBase::PlayerData playerData;
	std::shared_ptr<CollisionManager> collisionManager = NULL;
	std::shared_ptr<PlayerCalclation> playerCalclation = NULL;

public:
	Player();
	~Player();

	void Initialize();
	void Update()override;
	void Update(const VECTOR& cameraDirection, const int mapHandle);
	bool Draw();
	VECTOR Move(VECTOR& moveVec, const VECTOR& cameraDirection);
	void JumpMove();
	void RollMove();
	void HangringMove();
	void ChangeState();
	void SettingRay();

	void SetOldAnimState(PlayerStateActionBase::OldAnimState animState);
	void SetNowAnimState(PlayerStateActionBase::NowAnimState animState);

	

	//////////////////////////////////
	//　ゲッター
	///////////////////////////////////

	VECTOR GetCenterPos() { return centerPosition; }
	VECTOR GetFootPos() { return footPosition; }
	VECTOR GetTopPos() { return topPosition; }
	VECTOR GetBottomPos() { return bottomPosition; }
	VECTOR GetMoveVec() { return moveVec; }
	VECTOR GetlinePos_end() { return linePos_end; }
	VECTOR GetHangringPoint() { return hangringPoint; }
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


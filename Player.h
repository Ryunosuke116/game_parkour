#pragma once
#include "PlayerStateActionBase.h"
#include "MoveCalclation.h"

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

	bool isPush;					//ボタンを押したか
	bool isChageState;				//アニメーションを変更するか

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
	std::shared_ptr<Input> input = NULL;
	PlayerStateActionBase::PlayerData playerData;
	std::shared_ptr<CollisionManager> collisionManager = NULL;
	std::shared_ptr<MoveCalclation> moveCalclation = NULL;

public:
	Player();
	~Player();

	void Initialize();
	void Update()override;
	void Update(const VECTOR& cameraDirection, const int mapHandle);
	void Draw();
	VECTOR Move(VECTOR& moveVec, const VECTOR& cameraDirection);
	void JumpMove();
	void RollMove();
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


#pragma once
#include <memory>

class Input;

class PlayerStateActionBase
{
public:

	struct PlayerData
	{
		bool isMove;					//動いているか
		bool isJump;					//ジャンプしたか
		bool isJump_second;				//二段ジャンプしたか
		bool isJump_PlayAnim;			//ジャンプのアニメを再生するか
		bool isRoll;					//転がるか
		bool isRoll_PlayAnim;			//ロールアクションのアニメを再生するか
		bool isGround;					//接地しているか
		bool isSprint;					//走り出しか
		bool isStopRun;					//走り終わったか
		bool isJumpAll;					//全てのジャンプが完了したか
		bool isFalling;					//落下中か
		bool isHangring;				//崖つかみ中か
		bool isHang_to_Crouch;			//上に上がる
	};

	struct AnimState
	{
		int AttachIndex;			//アニメーション情報
		float PlayTime_anim;		//再生時間
		float TotalPlayTime_anim;	//総再生時間
		float PlayAnimSpeed;		//アニメーションスピード
	};

	PlayerStateActionBase(int& modelHandle,
		AnimState& oldAnimState, AnimState& nowAnimState);
	~PlayerStateActionBase() {};

	virtual bool MotionUpdate(PlayerData& playerData);
	void SetOldAnimState();
	void ResetOldAnimState();
	void ResetNowAnimState();
	bool Draw();


	//////////////////////////////////////////////
	// ゲッター
	//////////////////////////////////////////////
	//int GetPrevAttachIndex() { return oldAnimState.AttachIndex; }
	const AnimState GetOldAnimState() const { return oldAnimState; }
	const AnimState GetNowAnimState() const { return nowAnimState; }
	const float GetAnimBlendRate() { return animBlendRate; }

	

protected:


	static constexpr float	AnimBlendSpeed = 0.1f;		// アニメーションのブレンド率変化速度
	

	int modelHandle;			//モデルハンドル
	float animBlendRate;
	int animNumber_old;

	AnimState oldAnimState;
	AnimState nowAnimState;
	PlayerData  playerData;

	//移動
	//VECTOR moveVec;

	std::shared_ptr<Input> input = NULL;

};


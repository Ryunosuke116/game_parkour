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
		bool isRool;					//転がるか
		bool isGround;					//接地しているか
	};

	struct OldAnimState
	{
		int AttachIndex;			//前のアニメーション情報
		float PlayTime_anim;		//前回の再生時間
		float TotalPlayTime_anim;	//前回の総再生時間
		float PlayAnimSpeed;		//前回のアニメーションスピード
	};

	struct NowAnimState
	{
		int AttachIndex;				//現在のアニメーション情報
		float PlayTime_anim;			//現在の再生時間
		float TotalPlayTime_anim;	//現在の総再生時間
		float PlayAnimSpeed;			//前回のアニメーションスピード
	};

	PlayerStateActionBase(int& modelHandle,
		OldAnimState& oldAnimState, NowAnimState& nowAnimState);
	~PlayerStateActionBase() {};

	virtual bool MotionUpdate(PlayerData& playerData);
	void SetOldAnimState();
	void ResetOldAnimState();
	void ResetNowAnimState();


	//////////////////////////////////////////////
	// ゲッター
	//////////////////////////////////////////////
	//int GetPrevAttachIndex() { return oldAnimState.AttachIndex; }
	const OldAnimState GetOldAnimState() const { return oldAnimState; }
	const NowAnimState GetNowAnimState() const { return nowAnimState; }

protected:


	static constexpr float	AnimBlendSpeed = 0.1f;		// アニメーションのブレンド率変化速度
	

	int modelHandle;			//モデルハンドル
	float animBlendRate;

	OldAnimState oldAnimState;
	NowAnimState nowAnimState;
	PlayerData  playerData;

	//移動
	//VECTOR moveVec;

	enum animNum : int
	{
		braced_Hang_To_Crouch,		//しゃがんでぶらさがる
		falling_Idle,				//落ちているとき
		falling_To_Roll,			//着地して転がる
		hangring_Idle,				//ぶらさがる
		hard_Landing,				//着地する
		idle,						//静止時
		jump,						//ジャンプ
		jump_Over,					//ロールジャンプ
		quick_Roll,					//転がる
		run,						//走る
		run_Jump,					//走りながらジャンプ
		run_To_Stop,				//止まる
		running_Forward_Flip,		//走りながら回転ジャンプ
	};

	std::shared_ptr<Input> input = NULL;

};


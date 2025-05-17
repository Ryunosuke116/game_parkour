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
		Braced_Hang_To_Crouch,		//しゃがんでぶらさがる
		Falling_To_Roll,			//着地して転がる
		Hangring_Idle,				//ぶらさがる
		Hard_Landing,				//着地する
		Idle,						//静止時
		Jump,						//ジャンプ
		Quick_Roll,					//転がる
		Run,						//走る
		Run_Jump,					//走りながらジャンプ
		Run_To_Stop,				//止まる
		Running_Forward_Flip,		//走りながら回転ジャンプ
		Falling_Idle				//落ちているとき
	};

	std::shared_ptr<Input> input = NULL;

};


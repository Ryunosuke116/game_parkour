#pragma once
class PlayerStateActionBase
{
public:
	PlayerStateActionBase(int modelHandle);
	~PlayerStateActionBase() {};

	virtual void MotionUpdate();

protected:

	static constexpr float	AnimBlendSpeed = 0.1f;		// アニメーションのブレンド率変化速度

	int modelHandle;			//モデルハンドル
	float animBlendRate;

	int prevAttachIndex;			//前のアニメーション情報
	float prevPlayTime_anim;		//前回の再生時間
	float prevTotalPlayTime_anim;	//前回の総再生時間
	float prevPlayAnimSpeed;		//前回のアニメーションスピード

	int currentAttachIndex;				//現在のアニメーション情報
	float currentPlayTime_anim;			//現在の再生時間
	float currentTotalPlayTime_anim;	//現在の総再生時間
	float currentPlayAnimSpeed;			//前回のアニメーションスピード


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
};


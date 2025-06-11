#pragma once
class BaseChara
{
protected:

	int modelHandle;
	int motionNum;
	int HP;
	int nowFrameNumber;
	int animNumber_Now;

	int prevAttachIndex;			//前のアニメーション情報
	float prevPlayTime_anim;		//前回の再生時間
	float prevPlayAnimSpeed;		//前回のアニメーションスピード

	int currentAttachIndex;			//現在のアニメーション情報
	float currentPlayTime_anim;		//現在の再生時間
	float currentPlayAnimSpeed;		//前回のアニメーションスピード

	float animBlendRate;
	float keepPlayTime_anim;

	float totalTime_anim;
	float angle;

	VECTOR targetMoveDirection;
	VECTOR framePosition;
	VECTOR position;

	static constexpr float	MoveSpeed = 0.4f;	    // 移動速度
	static constexpr float angleSpeed = 0.2f;
	static constexpr float	AnimBlendSpeed = 0.1f;		// アニメーションのブレンド率変化速度
	static constexpr float PlayAnimSpeed = 0.5f;	//アニメ再生速度
public:

	BaseChara();
	~BaseChara();

	virtual void Initialize()abstract;
	virtual void Update()abstract;
	virtual void Draw()abstract;

	virtual void ChangeMotion(const int& motionNum, const float playAnimSpeed);
	virtual void MotionUpdate();
	virtual void Move(const Input& input, VECTOR& moveVec, const VECTOR& cameraDirection);
	void UpdateAngle(const VECTOR direction);

	/////////////////////////////////////////////
	// ゲッター
	/////////////////////////////////////////////
	const VECTOR GetPosition() { return position; }
	const VECTOR GetFramePosition()const { return framePosition; }
};


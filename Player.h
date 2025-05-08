#pragma once
class Player : public BaseChara
{
private:
	static constexpr float modelScale = 0.06f;
	static constexpr float MoveSpeed = 0.4f;	    // 移動速度
	static constexpr float runSpeed = 0.4f;
	static constexpr float Max_DashMoveSpeed = 1.0f;
	static constexpr float dashSpeed = 0.7f;
	static constexpr float attackAnimSpeed = 1.0f;	//攻撃速度
	static constexpr float rollAnimSpeed = 1.5f;	//攻撃速度
	static constexpr float angleSpeed = 0.3f;
	static constexpr float addJumpPower = 1.5f;		//ジャンプパワー
	static constexpr float Gravity = 0.1f;

	bool isMove;

	enum animationNumber
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
		Running_Forward_Flip		//走りながら回転ジャンプ
	};
public:
	Player();
	~Player();

	void Initialize();
	void Update(const Input& input, const VECTOR& cameraDirection);
	void Draw();
	void Move(const Input& input, VECTOR& moveVec, const VECTOR& cameraDirection);
	void UpdateAngle(const VECTOR direction);
	void ChangeMotion(const int& motionNum, const float playAnimSpeed)override;

	struct PadInput
	{
		bool isUp(const Input& input);
		bool isDown(const Input& input);
		bool isRight(const Input& input);
		bool isLeft(const Input& input);
	};

	PadInput padInput;
};


#pragma once
class Player : public BaseChara
{
private:
	static constexpr float modelScale = 0.06f;
	static constexpr float MoveSpeed = 0.4f;	    // �ړ����x
	static constexpr float runSpeed = 0.4f;
	static constexpr float Max_DashMoveSpeed = 1.0f;
	static constexpr float dashSpeed = 0.7f;
	static constexpr float attackAnimSpeed = 1.0f;	//�U�����x
	static constexpr float rollAnimSpeed = 1.5f;	//�U�����x
	static constexpr float angleSpeed = 0.3f;
	static constexpr float addJumpPower = 1.5f;		//�W�����v�p���[
	static constexpr float Gravity = 0.1f;

	bool isMove;

	enum animationNumber
	{
		Braced_Hang_To_Crouch,		//���Ⴊ��łԂ炳����
		Falling_To_Roll,			//���n���ē]����
		Hangring_Idle,				//�Ԃ炳����
		Hard_Landing,				//���n����
		Idle,						//�Î~��
		Jump,						//�W�����v
		Quick_Roll,					//�]����
		Run,						//����
		Run_Jump,					//����Ȃ���W�����v
		Run_To_Stop,				//�~�܂�
		Running_Forward_Flip		//����Ȃ����]�W�����v
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


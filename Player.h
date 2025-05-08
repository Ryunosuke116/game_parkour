#pragma once
class Player : public BaseChara
{
private:
	static constexpr float modelScale = 0.06f;
	static constexpr float MoveSpeed = 0.4f;	    // �ړ����x
	static constexpr float attackAnimSpeed = 1.0f;	//�U�����x
	static constexpr float rollAnimSpeed = 1.5f;	//�U�����x
	static constexpr float angleSpeed = 0.3f;
	static constexpr float addJumpPower = 1.9f;		//�W�����v�p���[
	static constexpr float gravity = 0.1f;


	float currentJumpSpeed;			//���݂̃W�����v�X�s�[�h

	bool isMove;					//�����Ă��邩
	bool isJump;					//�W�����v������
	bool isJump_second;				//��i�W�����v������
	bool isPush;					//�{�^������������

	enum class animNum : int
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
		Running_Forward_Flip,		//����Ȃ����]�W�����v
		Falling_Idle				//�����Ă���Ƃ�
	};
public:
	Player();
	~Player();

	void Initialize();
	void Update(const Input& input, const VECTOR& cameraDirection);
	void Draw();
	void Move(const Input& input, VECTOR& moveVec, const VECTOR& cameraDirection);
	void Jump(const Input& input);
	void JumpCalclation();
	void ChangeMotion(const int& motionNum, const float playAnimSpeed)override;
	void MotionUpdate() override;

	struct PadInput
	{
		bool isUp(const Input& input);
		bool isDown(const Input& input);
		bool isRight(const Input& input);
		bool isLeft(const Input& input);
		bool isJump(const Input& input);
	};

	PadInput padInput;
};


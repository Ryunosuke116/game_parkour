#pragma once
class BaseChara
{
protected:

	int modelHandle;
	int motionNum;
	int HP;
	int nowFrameNumber;
	int animNumber_Now;

	int prevAttachIndex;			//�O�̃A�j���[�V�������
	float prevPlayTime_anim;		//�O��̍Đ�����
	float prevTotalTime_anim;		//�O��̑��Đ�����
	float prevPlayAnimSpeed;

	int currentAttachIndex;
	float currentPlayTime_anim;			//���݂̍Đ�����
	float currentPlayAnimSpeed;
	float animBlendRate;
	float keepPlayTime_anim;

	float totalTime_anim;
	float angle;

	VECTOR targetMoveDirection;
	VECTOR framePosition;
	VECTOR position;

	static constexpr float	MoveSpeed = 0.4f;	    // �ړ����x
	static constexpr float angleSpeed = 0.2f;
	static constexpr float	AnimBlendSpeed = 0.1f;		// �A�j���[�V�����̃u�����h���ω����x
	static constexpr float PlayAnimSpeed = 0.5f;	//�A�j���Đ����x
public:

	BaseChara();
	~BaseChara();

	virtual void Initialize();
	virtual void Update();
	virtual void Draw();

	virtual void ChangeMotion(const int& motionNum, const float playAnimSpeed);
	virtual void MotionUpdate();
	virtual void Move(const Input& input, VECTOR& moveVec, const VECTOR& cameraDirection);
	void UpdateAngle(const VECTOR direction);

	/////////////////////////////////////////////
	// �Q�b�^�[
	/////////////////////////////////////////////
	const VECTOR GetPosition() { return position; }
	const VECTOR GetFramePosition()const { return framePosition; }
};


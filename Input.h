#pragma once
class Input
{
public:
	Input();
	~Input();

	void Update();

	int GetNowFrameInput() const { return nowFrameInput; }
	int GetNowFrameNewInput() const { return nowFrameNewInput; }

private:
	int		nowFrameInput;				// ���݂̃t���[���ŉ�����Ă���{�^��
	int		nowFrameNewInput;			// ���݂̃t���[���ŐV���ɉ����ꂽ�{�^��
};


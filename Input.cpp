#include "Include.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Input::Input()
	: nowFrameInput(0)
	, nowFrameNewInput(0)
{
	// �����Ȃ�
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Input::~Input()
{
	// �����Ȃ�
}

/// <summary>
/// �X�V
/// </summary>
void Input::Update()
{
	// �ЂƂO�̃t���[���̓��͂�ϐ��ɂƂ��Ă���
	int Old = nowFrameInput;

	// ���݂̓��͏�Ԃ��擾
	nowFrameInput = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	// ���̃t���[���ŐV���ɉ����ꂽ�{�^���̃r�b�g���������Ă���l�� nowFrameNewInput �ɑ������
	nowFrameNewInput = nowFrameInput & ~Old;
}
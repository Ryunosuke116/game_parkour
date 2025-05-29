#include "Include.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
Field::Field()
{
	//modelHandle = MV1LoadModel("material/mv1/field.mv1");
	modelHandle = MV1LoadModel("material/mv1/city/city_floor.mv1");
	position = VGet(0, 0, 0);
	//���f���̑傫������
	//MV1SetScale(modelHandle, VGet(1.0f, 0.17f, 1.0f));
	MV1SetScale(modelHandle, VGet(0.05f, 0.05f, 0.05f));
	MV1SetPosition(modelHandle, position);

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Field::~Field()
{

}

/// <summary>
/// ������
/// </summary>
void Field::Initialize()
{
	// ���f���̂O�Ԗڂ̃t���[���̃R���W���������\�z
	MV1SetupCollInfo(modelHandle, -1, 1, 1, 1);
}

/// <summary>
/// �X�V
/// </summary>
void Field::Update()
{
	if (CheckHitKey(KEY_INPUT_1))
	{
		if (!isPush)
		{
			if (!isPoly)
			{
				isPoly = true;
			}
			else
			{
				isPoly = false;
			}
			isPush = true;
		}
	}
	else
	{
		isPush = false;
	}

}

/// <summary>
/// �`��
/// </summary>
void Field::Draw()
{
	if (isPoly)
	{
		MV1SetWireFrameDrawFlag(modelHandle, TRUE);
	}
	else
	{
		MV1SetWireFrameDrawFlag(modelHandle, FALSE);
	}
	MV1DrawModel(modelHandle);
}
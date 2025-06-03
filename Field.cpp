#include "Include.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
Field::Field()
{
	//modelHandle = MV1LoadModel("material/mv1/field.mv1");
	//modelHandle = MV1LoadModel("material/mv1/city/city_0525.mv1");
	modelHandle = MV1LoadModel("material/mv1/new_city/0603.mv1");

	position = VGet(0, 0, 0);
	y = 0.0f;
	//���f���̑傫������
	//MV1SetScale(modelHandle, VGet(1.0f, 0.17f, 1.0f));
	//MV1SetScale(modelHandle, VGet(0.05f, 0.05f, 0.05f));
	MV1SetScale(modelHandle, VGet(1.0f, 1.0f, 1.0f));
	MV1SetRotationXYZ(modelHandle, VGet(0.0f, y * DX_PI_F / 180.0f, 0.0f));

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
	SetLightPosition(VGet(0.0f, -200.0f, 0.0f));
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

	if (CheckHitKey(KEY_INPUT_W))
	{
		y += 0.1f;
	}
	if (CheckHitKey(KEY_INPUT_S))
	{
		y -= 0.1f;
	}

	MV1SetRotationXYZ(modelHandle, VGet(0.0f, y * DX_PI_F / 180.0f, 0.0f));
	// ���f���̂O�Ԗڂ̃t���[���̃R���W���������\�z
	MV1SetupCollInfo(modelHandle, -1, 1, 1, 1);
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
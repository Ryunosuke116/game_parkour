#include "Include.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
Field::Field()
{
	//modelHandle = MV1LoadModel("material/mv1/field.mv1");
	//modelHandle = MV1LoadModel("material/mv1/city/city_0525.mv1");
	modelHandle = MV1LoadModel("material/mv1/new_city/0604.mv1");
	boxHandle = MV1LoadModel("material/mv1/new_city/0604_box.mv1");
	meshHandle = MV1LoadModel("material/mv1/new_city/0610_mesh.mv1");

	position = VGet(0, 0, 0);
	//���f���̑傫������
	//MV1SetScale(modelHandle, VGet(1.0f, 0.17f, 1.0f));
	//MV1SetScale(modelHandle, VGet(0.05f, 0.05f, 0.05f));
	MV1SetScale(modelHandle, VGet(1.0f, 1.0f, 1.0f));

	MV1SetPosition(modelHandle, position);
	MV1SetPosition(meshHandle, position);
	MV1SetPosition(boxHandle, position);

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
	// ���f���̂O�Ԗڂ̃t���[���̃R���W���������\�z
	MV1SetupCollInfo(meshHandle, -1, 1, 1, 1);

	int a = MV1GetMaterialNum(meshHandle);

	for (int i = 0; i < a; i++)
	{
		// �R�c���f���Ɋ܂܂��O�Ԗڂ̃}�e���A���̕`��u�����h���[�h�� DX_BLENDMODE_ADD �ɕύX����
		MV1SetMaterialDrawBlendMode(meshHandle, i, DX_BLENDMODE_ADD);
		// �}�e���A���̃u�����h�p�����[�^�� 128 �ɕύX����
		MV1SetMaterialDrawBlendParam(meshHandle, i, 0);
	}
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

	// ���f���̂O�Ԗڂ̃t���[���̃R���W���������\�z
	//MV1SetupCollInfo(modelHandle, -1, 1, 1, 1);

}

/// <summary>
/// �`��
/// </summary>
void Field::Draw()
{
	if (isPoly)
	{
		MV1SetWireFrameDrawFlag(modelHandle, TRUE);
		MV1SetWireFrameDrawFlag(meshHandle, TRUE);
	}
	else
	{
		MV1SetWireFrameDrawFlag(modelHandle, FALSE);
		MV1SetWireFrameDrawFlag(meshHandle, FALSE);
	}

	MV1DrawModel(modelHandle);
	 
	MV1DrawModel(meshHandle);
}
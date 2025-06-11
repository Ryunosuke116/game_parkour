#include "FieldMesh.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="path"></param>
FieldMesh::FieldMesh(const char* path):
	isPoly(true),
	isPush(true)
{
	modelHandle = MV1LoadModel(path);
	position = VGet(0, 0, 0);
	MV1SetPosition(modelHandle, position);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
FieldMesh::~FieldMesh()
{

}

/// <summary>
/// ������
/// </summary>
void FieldMesh::Initialize()
{
	// ���f���̂O�Ԗڂ̃t���[���̃R���W���������\�z
	MV1SetupCollInfo(modelHandle, -1, 1, 1, 1);

	int a = MV1GetMaterialNum(modelHandle);

	for (int i = 0; i < a; i++)
	{
		// �R�c���f���Ɋ܂܂��O�Ԗڂ̃}�e���A���̕`��u�����h���[�h�� DX_BLENDMODE_ADD �ɕύX����
		MV1SetMaterialDrawBlendMode(modelHandle, i, DX_BLENDMODE_ADD);
		// �}�e���A���̃u�����h�p�����[�^�� 128 �ɕύX����
		MV1SetMaterialDrawBlendParam(modelHandle, i, 0);
	}
}

/// <summary>
/// �X�V
/// </summary>
void FieldMesh::Update()
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
void FieldMesh::Draw()
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
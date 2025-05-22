#include "Include.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
Field::Field()
{
	//modelHandle = MV1LoadModel("material/mv1/field.mv1");
	modelHandle = MV1LoadModel("material/mv1/city/city.mv1");
	position = VGet(0, 0, 0);
	//���f���̑傫������
	//MV1SetScale(modelHandle, VGet(1.0f, 0.17f, 1.0f));
	MV1SetScale(modelHandle, VGet(0.1f, 0.1f, 0.1f));
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
	MV1SetupCollInfo(modelHandle, -1, 8, 8, 8);
}

/// <summary>
/// �X�V
/// </summary>
void Field::Update()
{

}

/// <summary>
/// �`��
/// </summary>
void Field::Draw()
{
	//MV1SetWireFrameDrawFlag(modelHandle, TRUE);
	MV1DrawModel(modelHandle);
}
#include "Include.h"


/// <summary>
/// �R���X�g���N�^
/// </summary>
Field::Field()
{
	modelHandle = MV1LoadModel("material/mv1/field.mv1");
	position = VGet(0, 0, 0);
	//���f���̑傫������
	MV1SetScale(modelHandle, VGet(1.0f, 0.17f, 1.0f));
	MV1SetPosition(modelHandle, position);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Field::~Field()
{

}

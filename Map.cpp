#include "Include.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Map::Map()
{
	modelHandle = MV1LoadModel("material/skyDome/sunSet.mv1");
	MV1SetScale(modelHandle, VGet(2.0f, 2.0f, 2.0f));
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Map::~Map()
{

}

/// <summary>
/// ������
/// </summary>
void Map::Initialize()
{

}

/// <summary>
/// �X�V
/// </summary>
void Map::Update()
{

}

/// <summary>
/// �`��
/// </summary>
void Map::Draw()
{
	MV1DrawModel(modelHandle);
}
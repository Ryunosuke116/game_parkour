#include "CoinObject.h"

/// @brief �R���X�g���N�^
/// @param path 
CoinObject::CoinObject(const char* path)
{
	modelHandle = MV1LoadModel(path);
	position = VGet(0.0f, 0.0f, 0.0f);
	MV1SetScale(modelHandle, VGet(0.1f, 0.1f, 0.1f));
}

/// @brief �f�X�g���N�^
CoinObject::~CoinObject()
{

}

/// @brief ������
void CoinObject::Initialize()
{
	position = VGet(5.0f, 8.0f, 5.0f);
	MV1SetPosition(modelHandle, position);
}

/// @brief �X�V
void CoinObject::Update()
{

}

/// @brief �`��
void CoinObject::Draw()
{
	MV1DrawModel(modelHandle);
}


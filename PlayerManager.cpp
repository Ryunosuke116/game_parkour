#include "Include.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerManager::PlayerManager()
{
	collisionManager = std::make_shared<CollisionManager>();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerManager::~PlayerManager()
{

}

/// <summary>
/// �X�V
/// </summary>
/// <param name="mapHandle"></param>
/// <param name="player"></param>
void PlayerManager::Update(int mapHandle, Player& player)
{
	collisionManager->Update(player, mapHandle);
}

/// <summary>
/// �`��
/// </summary>
void PlayerManager::Draw()
{
	collisionManager->Draw();
}
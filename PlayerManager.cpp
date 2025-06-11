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
	/*auto result = collisionManager->Update(player, mapHandle);
	player.SetIsGround(result.first);
	player.SetPos(result.second);*/

	// �v���C���[�̃��f���̍��W���X�V����
	MV1SetPosition(player.GetModelHandle(), player.GetPosition());
}

/// <summary>
/// �`��
/// </summary>
void PlayerManager::Draw()
{
	collisionManager->Draw();
}
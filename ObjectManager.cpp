#include "BaseManager.h"
#include "FieldMesh.h"
#include "Include.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
ObjectManager::ObjectManager()
{
	
}

/// <summary>
/// �f�X�g���N�^	
/// </summary>
ObjectManager::~ObjectManager()
{

}

/// <summary>
/// �I�u�W�F�N�g����
/// </summary>
void ObjectManager::Create()
{
	map = std::make_shared<Map>("material/skyDome/sunSet.mv1");
	field = std::make_shared<Field>("material/mv1/new_city/0604.mv1");
	fieldMesh = std::make_shared<FieldMesh>("material/mv1/new_city/0610_mesh.mv1");
	player = std::make_shared<Player>();
	camera = std::make_shared<Camera>();
	//playerManager = std::make_shared<PlayerManager>();
	actualPlayer = std::dynamic_pointer_cast<Player>(player);
}

/// <summary>
/// ������
/// </summary>
void ObjectManager::Initialize()
{
	map->Initialize();
	field->Initialize();
	fieldMesh->Initialize();
	player->Initialize();
	camera->Initialize();
}

/// <summary>
/// �X�V
/// </summary>
void ObjectManager::Update()
{
	actualPlayer->Update(camera->GetCameraDirection(),fieldMesh->GetModelHandle());
	camera->Update(player->GetPosition());
	//playerManager->Update(fieldMesh->GetModelHandle(), *actualPlayer);
	map->Update();
	field->Update();
	fieldMesh->Update();
}

/// <summary>
/// �`��
/// </summary>
void ObjectManager::Draw()
{
	player->Draw();
	camera->Draw();
	//playerManager->Draw();
	map->Draw();
	field->Draw();
	fieldMesh->Draw();
}
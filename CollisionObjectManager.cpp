#include "common.h"
#include <vector>
#include <memory>
#include "CollisionObjectManager.h"
#include "FieldMesh.h"
#include "Floor_sky.h"
#include "JsonManager.h"
#include "WorldSubSystem.h"

CollisionObjectManager::CollisionObjectManager()
{

}

CollisionObjectManager::~CollisionObjectManager()
{

}

void CollisionObjectManager::Create()
{
	const nlohmann::json data_floor_sky = JsonManager::GetInstance().GetJsons("floor_sky");
	const std::string path_floor_sky = data_floor_sky["modelPath"];
	const int modelHandle = MV1LoadModel(path_floor_sky.c_str());
	const auto self = shared_from_this();

	collisionObjects.push_back(std::make_shared<FieldMesh>());

	//floor_skyを追加
	for (auto& data : data_floor_sky["list"])
	{
		collisionObjects.push_back(std::make_shared<Floor_sky>(
			modelHandle,
			data
		));
	}

	for (auto& object : collisionObjects)
	{
		object->Create();
	}

	//サブシステムに追加
	WorldSubSystem::Instance().AddSubSystem<CollisionObjectManager>(self);
}

void CollisionObjectManager::Initialize()
{
	for (auto& object : collisionObjects)
	{
		object->Initialize();
	}
}

void CollisionObjectManager::Update()
{
	for (auto& object : collisionObjects)
	{
		object->Update();
	}
}

void CollisionObjectManager::Draw()
{
	for (auto& object : collisionObjects)
	{
		object->Draw();
	}
}
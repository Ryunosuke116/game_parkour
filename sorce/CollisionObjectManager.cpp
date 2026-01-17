#include "Common.h"
#include <vector>
#include <memory>
#include "CollisionObjectManager.h"
#include "FieldMesh.h"
#include "SkyFloor.h"
#include "JsonManager.h"
#include "WorldSubSystem.h"

CollisionObjectManager::CollisionObjectManager()
{

}

CollisionObjectManager::~CollisionObjectManager()
{
	collisionObjects.clear();
}


void CollisionObjectManager::Create()
{
	const nlohmann::json dataFloorSky	= JsonManager::GetInstance().GetJsons("floorSky");
	const std::string path_floor_sky	= dataFloorSky["modelPath"];
	const int modelHandle				= MV1LoadModel(path_floor_sky.c_str());
	const auto self						= shared_from_this();

	collisionObjects.push_back(std::make_shared<FieldMesh>());

	//floor_skyを追加
	for (auto& data : dataFloorSky["list"])
	{
		collisionObjects.push_back(std::make_shared<SkyFloor>(
			modelHandle,
			data));
	}

	for (auto& object : collisionObjects)
	{
		object->Create();
	}

	//サブシステムに追加
	WorldSubSystem::GetInstance().AddSubSystem<CollisionObjectManager>(self);
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

void CollisionObjectManager::ResultCreate()
{
	const auto self = shared_from_this();

	collisionObjects.push_back(std::make_shared<FieldMesh>());

	for (auto& object : collisionObjects)
	{
		object->Create();
	}

	//サブシステムに追加
	WorldSubSystem::GetInstance().AddSubSystem<CollisionObjectManager>(self);
}

void CollisionObjectManager::ResultInitialize()
{
	for (auto& object : collisionObjects)
	{
		object->Initialize();
	}
}

void CollisionObjectManager::ResultUpdate()
{
	for (auto& object : collisionObjects)
	{
		object->Update();
	}
}
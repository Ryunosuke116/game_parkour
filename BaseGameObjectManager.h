#pragma once
#include "common.h"
#include <nlohmann/json.hpp>
#include "IObject.h"
#include "ObjectMediator.h"

class BaseGameObjectManager : public IObject
{
public:
	BaseGameObjectManager() = default;
	~BaseGameObjectManager() = default;

	void HandOver(nlohmann::json j)
	{
		jsonData = j;
	}

	void Load(const nlohmann::json& jsonData)override {}

	void CreateMediator(ISoundPlayer& sound,
		IEffectManager& effect,
		Player& player,
		Camera& camera)override
	{
		for (auto& object : objects)
		{
			object->CreateMediator(sound, effect, player, camera);
		}
	}

	std::string GetTag() { return tag; }

protected:

	nlohmann::json	jsonData;
	VECTOR pos_addObject;
	std::string tag;

	std::vector<std::shared_ptr<IObject>> objects;

};


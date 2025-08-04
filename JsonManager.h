#pragma once
#include "BaseGameObjectManager.h"
#include <unordered_map>
#include <nlohmann/json.hpp>

class JsonManager : public BaseGameObjectManager
{
public:
	void Create()		override;
	void Initialize()	override;
	void Update()		override;
	void Draw()			override;
	void Add()			override;

	void Add(const std::string name, const nlohmann::json file);

	nlohmann::json GetJsons(std::string name) const { return jsons.at(name); }

private:
	int handlePath;

	std::unordered_map<std::string, nlohmann::json> jsons;
};

	
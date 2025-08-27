#pragma once
#include "BaseGameObjectManager.h"
#include <unordered_map>
#include <nlohmann/json.hpp>

class JsonManager : public BaseGameObjectManager
{
public:
	static JsonManager& Instance();  // ƒVƒ“ƒOƒ‹ƒgƒ“Žæ“¾

	JsonManager() = default;
	~JsonManager() = default;

	void Create()		override;
	void Initialize()	override;
	void Update(ObjectMediator& objectMediator)		override;
	void Draw()			override;
	void Add()			override;

	static void Add(const std::string name, const nlohmann::json file);

	static nlohmann::json GetJsons(std::string name) { return jsons.at(name); }

private:
	int handlePath;

	static std::unordered_map<std::string, nlohmann::json> jsons;
};

	
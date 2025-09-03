#pragma once
#include <unordered_map>
#include <nlohmann/json.hpp>

class JsonManager
{
public:
	static JsonManager& GetInstance();  // ƒVƒ“ƒOƒ‹ƒgƒ“Žæ“¾

	JsonManager() = default;
	~JsonManager() = default;

	void Create(const std::string& sceneName);
	
	static void Shutdown();
	static nlohmann::json GetJsons(std::string name) { return jsons.at(name); }

private:
	int handlePath;

	static std::unordered_map<std::string, nlohmann::json> jsons;
};

	
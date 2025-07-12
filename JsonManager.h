#pragma once
#include "BaseManager.h"
#include <unordered_map>
#include <nlohmann/json.hpp>

class JsonManager : public BaseManager
{
public:
	void Initialize()override;
	void Update()override;
	bool Draw()override;
	void Create()override;

	void Add(const std::string_view name, const char* path);

private:
	int handlePath;

	std::unordered_map<std::string_view, nlohmann::json> jsons;
	std::vector<std::ifstream*> files;
	std::vector<std::string> names;
};

	
#include <iostream>
#include <fstream>
#include <filesystem>
#include "DxLib.h"

#include "JsonManager.h"

namespace fs = std::filesystem;

void JsonManager::Initialize()
{
	for (const auto& entry : std::filesystem::directory_iterator("Json"))
	{
		std::ifstream file(entry.path());

		files.push_back(&file);

		std::string name = entry.path().filename().string();

		name = name.substr(0, name.size() - 5);

		names.push_back(name);
	}

	for(auto& file : files)
	{

	}

}

void JsonManager::Update()
{

}

bool JsonManager::Draw()
{
	return false;
}

void JsonManager::Add(const std::string_view name ,const char* path)
{
	std::ifstream ifs(path);

	jsons[name] = nlohmann::json::parse(ifs);
}

void JsonManager::Create(){}
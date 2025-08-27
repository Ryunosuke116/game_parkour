#include "common.h"
#include <fstream>
#include <filesystem>
#include "JsonManager.h"

JsonManager& JsonManager::Instance()
{
	static JsonManager instance;
	return instance;
}

void JsonManager::Initialize()
{
	//要素が0の場合だけ生成
	if (jsons.size() != 0)return;

	//Jsonディレクトリ内の全ファイルを読み込む
	for (const auto& entry : std::filesystem::directory_iterator("Json"))
	{
		//Jsonファイル
		std::ifstream file(entry.path());
		nlohmann::json j = nlohmann::json::parse(file);
		
		//ファイル名をキーにする
		std::string name = entry.path().filename().string();
		name = name.substr(0, name.size() - 5);

		//ディレクトリの分追加
		Add(name, j);
	}
}

void JsonManager::Update(ObjectMediator& objectMediator){}

void JsonManager::Draw(){}

void JsonManager::Add(const std::string name ,const nlohmann::json file)
{
	jsons[name] = file;
}

void JsonManager::Create(){}
void JsonManager::Add(){}

std::unordered_map<std::string, nlohmann::json> JsonManager::jsons;
#include "Common.h"
#include <fstream>
#include <filesystem>
#include "JsonManager.h"

JsonManager& JsonManager::GetInstance()
{
	static JsonManager instance;
	return instance;
}

void JsonManager::Create(const std::string& sceneName)
{
	//要素が0の場合だけ生成
	if (jsons.size() != 0)return;

	//Jsonディレクトリ内の全ファイルを読み込む
	for (const auto& entry : std::filesystem::directory_iterator(sceneName))
	{
		//Jsonファイル
		std::ifstream file(entry.path());
		nlohmann::json jsonData = nlohmann::json::parse(file);

		//ファイル名をキーにする
		std::string name = entry.path().filename().string();
		name = name.substr(0, name.size() - 5);

		//ディレクトリの分追加
		jsons[name] = jsonData;
	}
}

/// <summary>
/// 明示的に要素を削除できるように
/// </summary>
void JsonManager::Shutdown()
{
	jsons.clear();
}


std::unordered_map<std::string, nlohmann::json> JsonManager::jsons;
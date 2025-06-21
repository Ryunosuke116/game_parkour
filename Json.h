#pragma once
#include <nlohmann/json.hpp>

class JsonFile
{
public:
	static void Initialize(const char* path);
	static void UnInitialize();

	static nlohmann::json& GetJson() { return instance->j; }

	// コピー・代入を禁止
	JsonFile(const JsonFile&) = delete;
	JsonFile& operator=(const JsonFile&) = delete;

private:

	JsonFile() = default;
	~JsonFile() = default;

	static JsonFile* instance;

	using Json = nlohmann::json;
	nlohmann::json j;
};


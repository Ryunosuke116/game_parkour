#pragma once
class JsonFile
{
public:
	static void Initialize();
	static void UnInitialize();

private:

	JsonFile(){}
	~JsonFile(){}

	static JsonFile* instance;

	using Json = nlohmann::json;
	Json j;
};


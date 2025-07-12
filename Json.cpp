#include <iostream>
#include <fstream>
#include "DxLib.h"
#include "nlohmann/json.hpp"
#include "Json.h"

void JsonFile::Initialize(const char* path)
{
    if (!instance) {
        instance = new JsonFile();
    }

    std::ifstream ifs(path);

    if (ifs) {
        instance->j = nlohmann::json::parse(ifs);
    }
}

void JsonFile::UnInitialize()
{
    if (instance)
    {
        delete instance;
        instance = nullptr;
    }
}

JsonFile* JsonFile::instance = nullptr;
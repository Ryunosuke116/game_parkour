#include <iostream>
#include <fstream>
#include "DxLib.h"
#include "nlohmann/json.hpp"
#include "Json.h"

void JsonFile::Initialize()
{
    std::ifstream ifs("Json/path.json");
    if (ifs) {
        this->j = nlohmann::json::parse(ifs);
    }
}
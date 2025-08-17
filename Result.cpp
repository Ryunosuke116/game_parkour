#include "common.h"
#include <memory>
#include <string>
#include "BaseScene.h"
#include "Result.h"
#include "PadInput.h"
#include "JsonManager.h"
#include "Result_object.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="manager"></param>
Result::Result(SceneManager& manager) : BaseScene{ manager },
modelHandle(-1),
coinHandle(-1),
crossHandle(-1),
rankHandle(-1),
num(""),
coin_x(-1),
coin_y(-1),
isPush(false)
{
    jsonTag = "png";
}

/// <summary>
/// デストラクタ
/// </summary>
Result::~Result()
{

}

void Result::Create()
{
    nlohmann::json jsonData = JsonManager::GetJsons(jsonTag);

    Add(std::make_shared<Result_backGround>(), jsonData);
    Add(std::make_shared<UI_coin>(), jsonData);
    Add(std::make_shared<Rank>(), jsonData);

    ////UI
    //std::unordered_map<std::string, std::string> uiPath;
    ////ランク
    //std::unordered_map<std::string, std::string> rankPath;
    ////背景
    //std::unordered_map<std::string, std::string> resultPath;

    //for (auto& data : jsonData["coin"])
    //{
    //    std::string path = data[0];     //HandlePath
    //    std::string name = data[1];     //pathの名前

    //    uiPath[name] = path;
    //}

    //for (auto& data : jsonData["rank"])
    //{
    //    std::string path = data[0];     //HandlePath
    //    std::string name = data[1];     //pathの名前

    //    rankPath[name] = path;
    //}

    //for (auto& data : jsonData["result"])
    //{
    //    std::string path = data[0];     //HandlePath
    //    std::string name = data[1];     //pathの名前

    //    resultPath[name] = path;
    //}

    //coinHandle = LoadGraph(uiPath.at("coin").c_str());
    //crossHandle = LoadGraph(uiPath.at("cross").c_str());
    //LoadDivGraph(uiPath.at("number").c_str(),
    //    10, 10, 1, 32, 64, numberHandle);

    //backGroundHandle = LoadGraph(resultPath.at("backGround").c_str());
    //blackOut = std::make_shared<BlackOut>();

    //if (coinCount <= 20)
    //{
    //    rankHandle = LoadGraph(rankPath.at("D").c_str());
    //}
    //else if (coinCount <= 40)
    //{
    //    rankHandle = LoadGraph(rankPath.at("C").c_str());
    //}
    //else if (coinCount <= 60)
    //{
    //    rankHandle = LoadGraph(rankPath.at("B").c_str());
    //}
    //else if (coinCount <= 90)
    //{
    //    rankHandle = LoadGraph(rankPath.at("A").c_str());
    //}
    //else if (coinCount == 100)
    //{
    //    rankHandle = LoadGraph(rankPath.at("S").c_str());
    //}
}

/// <summary>
/// 初期化
/// </summary>
void Result::Initialize()
{
    //num = std::to_string(coinCount);

    ////一文字しか入ってない場合先頭に0を挿入する
    //if (num.length() == 1)
    //{
    //    num.insert(0, "0");
    //}
    const int coin_x = 30;
    const int coin_y = 700;

    for (auto& UI : ui_list)
    {
        if (auto ui_coin = std::dynamic_pointer_cast<UI_coin>(UI))
        {
            ui_coin->SetCoinPos(coin_x, coin_y);
        }
        UI->Initialize();
    }

}

/// <summary>
/// 更新処理
/// </summary>
void Result::Update()
{
    PadInput::Update();

    if (PadInput::IsPush_A() && !isPush)
    {
        isPush = true;
    }

    if (isPush)
    {
        blackOut->BlackOutUpdate(4.5f);
        if (blackOut->GetAlpha() >= 300)
        {
            ChangeScene("Title", 0);
        }
    }
}

/// <summary>
/// 描画
/// </summary>
void Result::Draw()
{
    //DrawGraph(0, 0, backGroundHandle, TRUE);
    ////blackOut->Draw();

    //DrawGraph(coin_x, coin_y, coinHandle, TRUE);

    //int num_x = 170;

    //for (char c : num)
    //{
    //    int digit = c - '0';
    //    DrawGraph(num_x, 730, numberHandle[digit], TRUE);
    //    num_x += 32;
    //}
    //DrawGraph(400, 150, rankHandle, TRUE);

    //DrawGraph(135, 730, crossHandle, TRUE);

    for (auto& UI : ui_list)
    {
        UI->Draw();
    }
}

void Result::Add(std::shared_ptr<BaseUI> ui,
    nlohmann::json& jsonData)
{
    ui_list.push_back(ui);
    ui_list.back()->Load(jsonData[ui_list.back()->GetJsonTag()]);
}
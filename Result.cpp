#include "common.h"
#include <memory>
#include <string>
#include "BaseScene.h"
#include "Result.h"
#include "PadInput.h"
#include "JsonManager.h"
#include "Result_object.h"
#include "BlackOut.h"

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
}

/// <summary>
/// 初期化
/// </summary>
void Result::Initialize()
{
    const int coin_x = 30;
    const int coin_y = 700;

    BlackOut::GetInstance().Initialize();

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
        BlackOut::GetInstance().BlackOutUpdate(5);
        if (BlackOut::GetInstance().GetAlpha() >= 300)
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
    for (auto& UI : ui_list)
    {
        UI->Draw();
    }
    BlackOut::GetInstance().Draw();
}

void Result::Add(std::shared_ptr<BaseUI> ui,
    nlohmann::json& jsonData)
{
    ui_list.push_back(ui);
    ui_list.back()->Load(jsonData[ui_list.back()->GetJsonTag()]);
}
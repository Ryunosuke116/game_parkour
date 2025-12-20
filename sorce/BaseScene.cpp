#include "Common.h"
#include <string_view>
#include "SceneManager.h"
#include "BaseScene.h"
#include "JsonManager.h"
#include "GameInstanceSubSystem.h"
#include "WorldSubSystem.h"

/// <summary>
/// インスタンス化
/// </summary>
BaseScene::BaseScene(SceneManager& manager) :manager{ manager } {}

/// <summary>
/// デストラクタ
/// </summary>
BaseScene::~BaseScene()
{

}

void BaseScene::Create()
{

}

/// <summary>
/// 初期化
/// </summary>
void BaseScene::Initialize()
{

}

/// <summary>
/// 更新
/// </summary>
void BaseScene::Update()
{

}

/// <summary>
/// 描画
/// </summary>
void BaseScene::Draw()
{

}

void BaseScene::Shutdown()
{
    JsonManager::GetInstance().Shutdown();
    GameInstanceSubSystem::GetInstance().ShutdownAll();
    WorldSubSystem::GetInstance().ShutdownAll();
}

void BaseScene::ChangeScene(const std::string_view name, const int coinCount)
{
    manager.ChangeScene(name,coinCount);
}

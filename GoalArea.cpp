#include "DxLib.h"
#include "GoalArea.h"
#include "DebugDrawer.h"

/// <summary>
/// コンストラクタ
/// </summary>
GoalArea::GoalArea()
{
	VECTOR min = VGet(153.0f, 8.0f, 750.0f);
	VECTOR max = VGet(245.0f, 80.0f, 815.0f);
	DebugDrawer::Instance().InformationInput_AABB(min, max, GetColor(255, 0, 0));
	goalArea.min = VGet(153.0f, 8.0f, 750.0f);
	goalArea.max = VGet(245.0f, 80.0f, 815.0f);
}

/// <summary>
/// デストラクタ
/// </summary>
GoalArea::~GoalArea()
{

}

/// <summary>
/// 初期化
/// </summary>
void GoalArea::Initialize()
{

}

/// <summary>
/// 更新処理
/// </summary>
void GoalArea::Update()
{

}
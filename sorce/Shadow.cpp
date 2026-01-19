#include "Common.h"
#include "Shadow.h"
#include "WorldSubSystem.h"
#include "PlayerManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
Shadow::Shadow() :
	shadowMapHandle(-1)
{
	
}

/// <summary>
/// デストラクタ
/// </summary>
Shadow::~Shadow()
{
	DeleteShadowMap(shadowMapHandle);
}

/// <summary>
/// 初期化
/// </summary>
void Shadow::Initialize()
{
	const VECTOR kInitLightDirection = VGet(0.5f, -1.0f, 0.5f);

	shadowMapHandle = MakeShadowMap(shadowQuality, shadowQuality);
	lightDirection	= kInitLightDirection;

	//ライトの方向を設定
	SetLightDirection(lightDirection);

	// シャドウマップが想定するライトの方向もセット
	SetShadowMapLightDirection(shadowMapHandle, lightDirection);
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="pos"></param>
void Shadow::Update()
{
	const VECTOR drawRange								= VGet(100.0f, 50.0f, 100.0f);
	const std::shared_ptr<PlayerManager> playerManager	= WorldSubSystem::GetInstance().GetSubSystem<PlayerManager>();

	//シャドウを描画する範囲を指定
	VECTOR minPos = VSub(playerManager->GetPosition(), drawRange);
	VECTOR maxPos = VAdd(playerManager->GetPosition(), drawRange);

	SetShadowMapDrawArea(shadowMapHandle, minPos, maxPos);
}

void Shadow::LayoutUpdate(const VECTOR& pos)
{
	const VECTOR drawRange = VGet(100.0f, 50.0f, 100.0f);

	//シャドウを描画する範囲を指定
	VECTOR minPos = VSub(pos, drawRange);
	VECTOR maxPos = VAdd(pos, drawRange);

	SetShadowMapDrawArea(shadowMapHandle, minPos, maxPos);
}

/// <summary>
/// リザルトシーン時の生成
/// </summary>
/// <param name="coinCount"></param>
void Shadow::ResultCreate()
{
	//処理なし
}

/// <summary>
/// リザルトシーン時の初期化
/// </summary>
void Shadow::ResultInitialize()
{
	Initialize();
}

/// <summary>
/// リザルトシーン時の更新処理
/// </summary>
void Shadow::ResultUpdate()
{
	Update();
}
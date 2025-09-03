#include "common.h"
#include "Shadow.h"
#include "MediatorInclude.h"
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

}

/// <summary>
/// 初期化
/// </summary>
void Shadow::Initialize()
{
	const VECTOR initLightDirection = VGet(0.5f, -1.0f, 0.5f);

	shadowMapHandle = MakeShadowMap(Shadow_Quality, Shadow_Quality);
	lightDirection = initLightDirection;

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
	const VECTOR drawRange = VGet(100.0f, 50.0f, 100.0f);
	const std::shared_ptr<PlayerManager> playerManager = WorldSubSystem::Instance().GetSubSystem<PlayerManager>();

	//シャドウを描画する範囲を指定
	VECTOR minPos = VSub(playerManager->GetPosition(), drawRange);
	VECTOR maxPos = VAdd(playerManager->GetPosition(), drawRange);

	SetShadowMapDrawArea(shadowMapHandle, minPos, maxPos);
}



#include "common.h"
#include "Shadow.h"

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
	shadowMapHandle = MakeShadowMap(Shadow_Quality, Shadow_Quality);
	lightDirection = VGet(0.5f, -1.0f, 0.5f);

	//ライトの方向を設定
	SetLightDirection(lightDirection);

	// シャドウマップが想定するライトの方向もセット
	SetShadowMapLightDirection(shadowMapHandle, lightDirection);
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="pos"></param>
void Shadow::Update(const VECTOR& pos)
{
	//シャドウを描画する範囲を指定
	VECTOR minPos = VSub(pos, VGet(100.0f, 50.0f, 100.0f));
	VECTOR maxPos = VAdd(pos, VGet(100.0f, 25.0f, 100.0f));

	SetShadowMapDrawArea(shadowMapHandle, minPos, maxPos);
}



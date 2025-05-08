#include "Include.h"

/// <summary>
/// インスタンス
/// </summary>
BaseObject::BaseObject()
{
	
}

/// <summary>
/// デストラクタ
/// </summary>
BaseObject::~BaseObject()
{

}

/// <summary>
/// 初期化
/// </summary>
void BaseObject::Initialize()
{

}

/// <summary>
/// 更新
/// </summary>
void BaseObject::Update()
{

}

/// <summary>
/// 描画
/// </summary>
void BaseObject::Draw()
{
	MV1DrawModel(modelHandle);
}

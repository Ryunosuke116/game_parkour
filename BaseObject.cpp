#include "common.h"
#include "BaseObject.h"

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
bool BaseObject::Draw()
{
	//MV1SetWireFrameDrawFlag(modelHandle,TRUE);
	MV1DrawModel(modelHandle);
	return true;
}

#include "BaseObject.h"

/// <summary>
/// インスタンス
/// </summary>
BaseObject::BaseObject():
	modelHandle(-1),
	position(VGet(-1.0f, -1.0f, -1.0f)),
	pos_difference(VGet(-1.0f, -1.0f, -1.0f)),
	tag("")
{
}

/// <summary>
/// デストラクタ
/// </summary>
BaseObject::~BaseObject()
{

}

/// <summary>
/// 描画
/// </summary>
void BaseObject::Draw()
{
	//MV1SetWireFrameDrawFlag(modelHandle,TRUE);
	MV1DrawModel(modelHandle);
}

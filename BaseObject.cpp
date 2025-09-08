#include "BaseObject.h"

/// <summary>
/// インスタンス
/// </summary>
BaseObject::BaseObject():
	modelHandle(-1),
	position(VGet(-1.0f, -1.0f, -1.0f)),
	pos_difference(VGet(-1.0f, -1.0f, -1.0f)),
	tag(""),
	jsonTag("")
{
}

/// <summary>
/// デストラクタ
/// </summary>
BaseObject::~BaseObject()
{
	MV1DeleteModel(modelHandle);
}

/// <summary>
/// 描画
/// </summary>
void BaseObject::Draw()
{
	MV1DrawModel(modelHandle);
}

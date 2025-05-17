#include "Include.h"


/// <summary>
/// コンストラクタ
/// </summary>
Field::Field()
{
	modelHandle = MV1LoadModel("material/mv1/field.mv1");
	position = VGet(0, 0, 0);
	//モデルの大きさ調整
	MV1SetScale(modelHandle, VGet(1.0f, 0.17f, 1.0f));
	MV1SetPosition(modelHandle, position);
}

/// <summary>
/// デストラクタ
/// </summary>
Field::~Field()
{

}

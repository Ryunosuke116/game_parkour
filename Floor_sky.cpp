#include "common.h"
#include  "nlohmann/json.hpp"
#include "Floor_sky.h"
#include "Calculation.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="handle"></param>
/// <param name="pos"></param>
/// <param name="objectTag"></param>
Floor_sky::Floor_sky(const int& handle, const VECTOR& pos,
	const float& degree, const std::string& objectTag):
	moveAmount(0.0f)
{
	modelHandle = MV1DuplicateModel(handle);
	position_Init = pos;
	moveDirection = Calculation::GetDirectionFromDegree(degree);
	tag = objectTag;
}

/// <summary>
/// デストラクタ
/// </summary>
Floor_sky::~Floor_sky()
{

}

/// <summary>
/// 初期化
/// </summary>
void Floor_sky::Initialize()
{
	isUp = true;
	pos_difference = VGet(0.0f, 0.0f, 0.0f);
	position = position_Init;

	MV1SetPosition(modelHandle, position);
	MV1SetScale(modelHandle, VGet(0.3f, 0.3f, 0.3f));

	// モデルの０番目のフレームのコリジョン情報を構築
	MV1SetupCollInfo(modelHandle, -1, 1, 1, 1);
}

/// <summary>
/// 更新
/// </summary>
void Floor_sky::Update()
{
	VECTOR newPos = position;

	if (moveAmount >= 140.0f)
	{
		isUp = false;
	}
	else if (moveAmount <= 0.0f)
	{
		isUp = true;
	}

	if (isUp)
	{
		newPos = VAdd(newPos, VScale(moveDirection, velocity));
		moveAmount += velocity;
	}
	else
	{
		newPos = VAdd(newPos, VScale(moveDirection, -velocity));
		moveAmount -= velocity;
	}

	//newPos = VAdd(newPos, addPos);

	pos_difference = VSub(newPos, position);

	position = newPos;

	MV1SetPosition(modelHandle, position);
	
	MV1RefreshCollInfo(modelHandle, -1);
}

/// <summary>
/// 描画
/// </summary>
/// <returns></returns>
void Floor_sky::Draw()
{
	MV1DrawModel(modelHandle);
}
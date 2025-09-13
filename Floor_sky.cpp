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
Floor_sky::Floor_sky(const int modelHandle, 
	const nlohmann::json& jsonData):
	moveAmount(0.0f)
{
	this->modelHandle = MV1DuplicateModel(modelHandle);
	stateData = jsonData;
}

/// <summary>
/// デストラクタ
/// </summary>
Floor_sky::~Floor_sky()
{

}

void Floor_sky::Create()
{
	Load(stateData);
}

void Floor_sky::Load(const nlohmann::json& jsonData)
{
	const float degree = jsonData[3];
	const std::string tag = jsonData[4].get<std::string>();
	const VECTOR position = VGet(jsonData[0].get<float>(), 
		jsonData[1].get<float>(),
		jsonData[2].get<float>());

	position_Init = position;
	moveDirection = Calculation::GetDirectionFromDegree(degree);
	this->tag = tag;
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

/// <summary>
/// リザルトシーン時の初期化
/// </summary>
void Floor_sky::ResultInitialize()
{
	Initialize();
}

/// <summary>
/// リザルトシーン時の更新処理
/// </summary>
void Floor_sky::ResultUpdate()
{
	//処理なし
}
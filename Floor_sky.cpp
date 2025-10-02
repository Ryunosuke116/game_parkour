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
	moveAmount(0.0f),
	isUp(false),
	moveDirection(VGet(0.0f, 0.0f, 0.0f)),
	kInitPosition(VGet(0.0f, 0.0f, 0.0f))
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

	kInitPosition = VGet(jsonData[0].get<float>(),
		jsonData[1].get<float>(),
		jsonData[2].get<float>());

	moveDirection = Calculation::GetDirectionFromDegree(degree);
	this->tag = tag;
}

/// <summary>
/// 初期化
/// </summary>
void Floor_sky::Initialize()
{
	const VECTOR kModelScale = VGet(0.3f, 0.3f, 0.3f);
	isUp = true;
	differencePosition = VGet(0.0f, 0.0f, 0.0f);
	position = kInitPosition;

	MV1SetPosition(modelHandle, position);
	MV1SetScale(modelHandle, kModelScale);

	// モデルの０番目のフレームのコリジョン情報を構築
	MV1SetupCollInfo(modelHandle, -1, 1, 1, 1);
}

/// <summary>
/// 更新
/// </summary>
void Floor_sky::Update()
{
	VECTOR newPos = position;
	const float kMaxMoveAmount = 140.0f;
	const float kMinMoveAmount = 0.0f;

	if (moveAmount >= kMaxMoveAmount)
	{
		isUp = false;
	}
	else if (moveAmount <= kMinMoveAmount)
	{
		isUp = true;
	}

	if (isUp)
	{
		newPos = VAdd(newPos, VScale(moveDirection, kVelocity));
		moveAmount += kVelocity;
	}
	else
	{
		newPos = VAdd(newPos, VScale(moveDirection, -kVelocity));
		moveAmount -= kVelocity;
	}

	//newPos = VAdd(newPos, addPos);

	differencePosition = VSub(newPos, position);

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
/// リザルトシーン時の生成
/// </summary>
/// <param name="coinCount"></param>
void Floor_sky::ResultCreate()
{
	//処理なし
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
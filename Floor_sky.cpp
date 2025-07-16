#include "common.h"
#include  "nlohmann/json.hpp"
#include "Floor_sky.h"

Floor_sky::Floor_sky(nlohmann::json jsonData)
{
	std::string path = jsonData["path"];

	modelHandle = MV1LoadModel(path.c_str());
	tag = "moveFloor_1";
}

Floor_sky::~Floor_sky()
{

}

void Floor_sky::Initialize()
{
	isUp = true;
	pos_difference = VGet(0.0f, 0.0f, 0.0f);
	position = VGet(5.0f, 10.0f, 10.0f);

	MV1SetPosition(modelHandle, position);
	MV1SetScale(modelHandle, VGet(0.5f, 0.5f, 0.5f));

	// モデルの０番目のフレームのコリジョン情報を構築
	MV1SetupCollInfo(modelHandle, -1, 1, 1, 1);
}

void Floor_sky::Update()
{
	VECTOR newPos = position;

	if (newPos.z >= 15.0f)
	{
		isUp = false;
	}
	else if (newPos.z <= 7.0f)
	{
		isUp = true;
	}

	if (isUp)
	{
		newPos.z += 0.2f;
	}
	else
	{
		newPos.z -= 0.2f;
	}

	//newPos = VAdd(newPos, addPos);

	pos_difference = VSub(newPos, position);

	position = newPos;

	MV1SetPosition(modelHandle, position);
	
	MV1RefreshCollInfo(modelHandle, -1);
}

bool Floor_sky::Draw()
{
	MV1DrawModel(modelHandle);
	return false;
}
#include "common.h"
#include  "nlohmann/json.hpp"
#include "Floor_sky.h"

Floor_sky::Floor_sky(nlohmann::json jsonData)
{
	std::string path = jsonData["path"];

	modelHandle = MV1LoadModel(path.c_str());
}

Floor_sky::~Floor_sky()
{

}

void Floor_sky::Initialize()
{
	isUp = true;
	pos_y = 10.0f;
	position = VGet(5.0f, pos_y, 0.0f);
	MV1SetPosition(modelHandle, position);
}

void Floor_sky::Update()
{
	if (pos_y >= 15.0f)
	{
		isUp = false;
	}
	else if (pos_y <= 7.0f)
	{
		isUp = true;
	}

	if (isUp)
	{
		pos_y += 0.2f;
	}
	else
	{
		pos_y -= 0.2f;
	}

	position.y = pos_y;
	MV1SetPosition(modelHandle, position);
	
}

bool Floor_sky::Draw()
{
	MV1DrawModel(modelHandle);
	return false;
}
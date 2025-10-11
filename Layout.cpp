#include "common.h"
#include <fstream>
#include <format>
#include "Layout.h"
#include "DebugDrawer.h"
#include "PadInput.h"
#include "WorldSubSystem.h"

Layout::Layout() :
	isPush(false)
{

}

Layout::~Layout()
{
	MV1DeleteModel(modelHandle);
}

void Layout::Initialize(const int& modelHandle)
{
	const VECTOR kMoedlScale = VGet(0.3f, 0.3f, 0.3f);

	this->modelHandle = MV1DuplicateModel(modelHandle);
	MV1SetScale(this->modelHandle, kMoedlScale);
}

void Layout::Update(const VECTOR& pos,
	BaseGameObjectManager& manager)
{
	MV1SetPosition(modelHandle, pos);
	DebugDrawer::GetInstance().InformationInput_string_VECTOR("position x %f  y %f  z %f\n", pos);

	if (CheckHitKey(KEY_INPUT_SPACE) ||
		PadInput::IsPushA())
	{
		if (!isPush)
		{
			manager.Add();
			isPush = true;
		}

		InputFile(WorldSubSystem::GetInstance().GetSubSystem<Camera>()->GetScreenCenterPosition());
	}
	else
	{
		isPush = false;
	}
}

void Layout::Draw()
{
	const int kMaxAlpha = 255;
	const int kMinAlpha = 0;
	// マテリアルのブレンドパラメータを 128 に変更する
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, kMaxAlpha);
	MV1DrawModel(modelHandle);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, kMinAlpha);
}

void Layout::InputFile(const VECTOR& inputPosition)
{
	std::ofstream file("coinArrangement/input.txt", std::ios::app);

	file << std::format("[ {}, {}, {} ],\n", inputPosition.x, inputPosition.y, inputPosition.z);

	file.close();
}
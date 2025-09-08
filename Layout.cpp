#include "common.h"
#include <fstream>
#include "Layout.h"
#include "DebugDrawer.h"
#include "PadInput.h"

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
	this->modelHandle = MV1DuplicateModel(modelHandle);
	MV1SetScale(this->modelHandle, VGet(0.3f, 0.3f, 0.3f));
}

void Layout::Update(const VECTOR& pos,
	BaseGameObjectManager& manager)
{
	MV1SetPosition(modelHandle, pos);
	DebugDrawer::Instance().InformationInput_string_VECTOR("position x %f  y %f  z %f\n", pos);


	if (CheckHitKey(KEY_INPUT_SPACE) ||
		PadInput::IsPush_A())
	{
		if (!isPush)
		{
			manager.Add();
			isPush = true;
		}

		InputFile();
	}
	else
	{
		isPush = false;
	}
}

void Layout::Draw()
{
	// マテリアルのブレンドパラメータを 128 に変更する
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	MV1DrawModel(modelHandle);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,0);
}

void Layout::InputFile()
{
	std::ofstream file("coinArrangement/input.txt");

	file << "a";
	file.close();
}
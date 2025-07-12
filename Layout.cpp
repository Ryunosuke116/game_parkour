#include "common.h"
#include "Layout.h"

Layout::Layout() :
	isPush(false)
{

}

Layout::~Layout()
{
	
}

void Layout::Initialize(const int& modelHandle)
{
	this->modelHandle = MV1DuplicateModel(modelHandle);
	MV1SetScale(this->modelHandle, VGet(0.3f, 0.3f, 0.3f));
}

void Layout::Update(const VECTOR& pos, CoinManager& manager)
{
	MV1SetPosition(modelHandle, pos);

	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		if (!isPush)
		{
			manager.Add();
			isPush = true;
		}
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
#include "common.h"
#include "Layout.h"
#include "CoinManager.h"

Layout::Layout() :
	isPush(false)
{

}

Layout::~Layout()
{
	
}



void Layout::Update(const VECTOR& pos, BaseManager& manager)
{
	if (CheckHitKey(KEY_INPUT_Q))
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
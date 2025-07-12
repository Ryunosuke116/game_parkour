#pragma once
#include "BaseManager.h"
#include "CoinManager.h"

class Layout
{
private:
	bool isPush;
	int modelHandle;

public:

	Layout();
	~Layout();

	void Initialize(const int& modelHandle);
	void Update(const VECTOR& pos, CoinManager& manager);

	void Draw();

};


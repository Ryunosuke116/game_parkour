#pragma once
#include "BaseManager.h"

class Layout
{
private:
	bool isPush;

public:

	Layout();
	~Layout();

	void Update(const VECTOR& pos, BaseManager& manager);

};


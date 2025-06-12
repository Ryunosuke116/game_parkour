#pragma once
#include "BaseObject.h"
class CoinObject : public BaseObject
{
public:
	CoinObject(const char* path);
	~CoinObject();

	void Initialize()override;
	void Update()override;
	void Draw()override;
};


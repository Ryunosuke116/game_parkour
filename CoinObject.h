#pragma once
#include "HitCheck.h"
#include "BaseObject.h"

class CoinObject : public BaseObject
{
public:
	CoinObject(const char* path);
	~CoinObject();

	void Initialize()override;
	void Update()override;
	void Update(const VECTOR& playerPos, const VECTOR& playerPos_bottom, const float radius);
	void Draw()override;

	HitCheck hitCheck;
};


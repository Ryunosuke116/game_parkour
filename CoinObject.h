#pragma once
#include "HitCheck.h"
#include "BaseObject.h"

class CoinObject : public BaseObject
{
public:
	CoinObject(const int& handle, const VECTOR& pos);
	~CoinObject();

	void Initialize()override;
	void Update()override;
	bool Update(const VECTOR& playerPos, const VECTOR& playerPos_bottom, const float radius);
	bool Draw()override;
	void HitPlayerAction(const VECTOR& playerPos, const float& radius);

	HitCheck hitCheck;
	bool GetHitFlag() { return hitFlag; }

private:

	bool isHitPlayer;
	bool hitFlag;
	float radian_Y;

	static constexpr float radius = 4.5f;
	static constexpr float objectScale = 0.4f;
};


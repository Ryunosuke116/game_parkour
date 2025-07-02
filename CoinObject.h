#pragma once
#include "HitCheck.h"
#include "BaseObject.h"

class CoinObject : public BaseObject
{
public:
	CoinObject(const char* path, const VECTOR& pos);
	~CoinObject();

	void Initialize()override;
	void Update()override;
	void Update(const VECTOR& playerPos, const VECTOR& playerPos_bottom, const float radius);
	void Draw()override;
	void HitPlayerAction(const VECTOR& playerPos, const float& radius);

	HitCheck hitCheck;

private:

	bool isHitPlayer;
	bool hitFlag;
	float radian_Y;

	static constexpr float radius = 4.5f;
};


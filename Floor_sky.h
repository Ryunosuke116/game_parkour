#pragma once
#include "BaseObject.h"

class Floor_sky : public BaseObject
{
public:
	Floor_sky(const int& handle, const VECTOR& pos,
		const float& degree, const std::string& objectTag);
	~Floor_sky();

	void Initialize()override;
	void Update()override;
	void Draw()override;
	void Load(const nlohmann::json& jsonData)override{}

private:
	static constexpr float velocity = 0.2f;

	bool isUp;
	float moveAmount;
	VECTOR moveDirection;
	VECTOR position_Init;
};


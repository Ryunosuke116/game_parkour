#pragma once
#include "BaseObject.h"

class Floor_sky : public BaseObject
{
public:
	Floor_sky(nlohmann::json jsonData);
	~Floor_sky();

	void Initialize()override;
	void Update()override;
	bool Draw()override;

private:
	float pos_y;
	bool isUp;
};


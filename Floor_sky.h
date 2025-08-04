#pragma once
#include "BaseObject.h"

class Floor_sky : public BaseObject
{
public:
	Floor_sky(const int& handle, const VECTOR& pos,
		const std::string& objectTag);
	~Floor_sky();

	void Initialize()override;
	void Update()override;
	bool Draw()override;

private:
	bool isUp;
};


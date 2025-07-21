#pragma once
#include "BaseObject.h"
class Wall : public BaseObject
{
public:
	Wall(nlohmann::json jsonData);
	~Wall();

	void Initialize()override;
	void Update()override;
	bool Draw()override;

private:

};


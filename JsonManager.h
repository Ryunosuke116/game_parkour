#pragma once
#include "BaseManager.h"

class JsonManager : public BaseManager
{
public:
	void Initialize()override;
	void Update()override;
	bool Draw()override;
	void Add()override;
	void Create()override;
private:
	int handlePath;
};


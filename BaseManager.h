#pragma once
#include "dxlib.h"

class BaseManager
{
public:

	virtual ~BaseManager() {};

	//ƒˆ‰¼‘zŠÖ”	
	virtual void Create() abstract;
	virtual void Initialize() abstract;
	virtual void Update() abstract;
	virtual void Draw() abstract;

};


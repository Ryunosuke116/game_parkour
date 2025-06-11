#pragma once
#include "DxLib.h"

class BaseObject
{
protected:
	int modelHandle;
	VECTOR position;
public:
	BaseObject();
	~BaseObject();

	//ƒˆ‰¼‘zŠÖ”	
	virtual void Initialize() abstract;
	virtual void Update() abstract;
	virtual void Draw() abstract;

	const int GetModelHandle() { return modelHandle; }
};


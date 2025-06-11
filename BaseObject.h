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

	//�������z�֐�	
	virtual void Initialize() abstract;
	virtual void Update() abstract;
	virtual void Draw() abstract;

	const int GetModelHandle() { return modelHandle; }
};


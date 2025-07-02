#pragma once

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
	virtual bool Draw() abstract;

	const int GetModelHandle() { return modelHandle; }
};


#pragma once
class BaseObject
{
protected:
	int modelHandle;
	VECTOR positon;
public:
	BaseObject();
	~BaseObject();

	virtual void Initialize();
	virtual void Update();
	virtual void Draw();
};


#pragma once
class BaseObject
{
protected:
	int modelHandle;
	VECTOR position;
public:
	BaseObject();
	~BaseObject();

	virtual void Initialize();
	virtual void Update();
	virtual void Draw();
};


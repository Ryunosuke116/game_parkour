#pragma once
class BaseManager
{
public:
	BaseManager() {};
	~BaseManager() {};

	//ƒˆ‰¼‘zŠÖ”	
	virtual void Create() abstract;
	virtual void Initialize() abstract;
	virtual void Update() abstract;
	virtual void Draw() abstract;

};


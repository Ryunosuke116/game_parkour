#pragma once

class IObjectManager
{
public:
	virtual ~IObjectManager(){}

	//ƒˆ‰¼‘zŠÖ”	
	virtual void Create()		abstract;
	virtual void Initialize()	abstract;
	virtual void Update()		abstract;
	virtual void Draw()			abstract;
	virtual void Add()			abstract;
};


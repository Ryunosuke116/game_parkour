#pragma once
#include <nlohmann/json.hpp>


class IObject
{
public:
	virtual ~IObject() {}

	//ƒˆ‰¼‘zŠÖ”	
	virtual void Create()								abstract;
	virtual void Initialize()							abstract;
	virtual void Update()								abstract;
	virtual void Draw()									abstract;
	virtual void ResultInitialize()						abstract;
	virtual void ResultUpdate()							abstract;
	virtual void ResultCreate()							abstract;
};
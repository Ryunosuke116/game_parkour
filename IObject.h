#pragma once
#include <nlohmann/json.hpp>
#include "ObjectMediator.h"

class IObject
{
public:
	virtual ~IObject() {}

	//ƒˆ‰¼‘zŠÖ”	
	virtual void Initialize()							abstract;
	virtual void Update(ObjectMediator& objectMediator)	abstract;
	virtual void Draw()									abstract;
	virtual void Load(const nlohmann::json& jsonData)	abstract;
	virtual void Create()								abstract;
	virtual void Add()									abstract;
};
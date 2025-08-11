#pragma once
#include <nlohmann/json.hpp>

class Object_interface
{
public:
	virtual ~Object_interface() {}

	//ƒˆ‰¼‘zŠÖ”	
	virtual void Initialize()							abstract;
	virtual void Update()								abstract;
	virtual void Draw()									abstract;
	virtual void Load(const nlohmann::json& jsonData)	abstract;
};
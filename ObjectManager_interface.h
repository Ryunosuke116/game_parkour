#pragma once
#include "common.h"
#include <nlohmann/json.hpp>


class ObjectManager_interface
{
public:

	virtual ~ObjectManager_interface(){}

	//ƒˆ‰¼‘zŠÖ”	
	virtual void Create()		abstract;
	virtual void Initialize()	abstract;
	virtual void Update()		abstract;
	virtual void Draw()			abstract;
	virtual void Add()			abstract;

};


#include <iostream>
#include "WorldSubSystem.h"
#include "IObject.h"

WorldSubSystem& WorldSubSystem::GetInstance()
{
	static WorldSubSystem instance;
	return instance;
}

void WorldSubSystem::ShutdownAll()
{
	subSystems.clear();
}
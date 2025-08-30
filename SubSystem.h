#pragma once
#include <iostream>
#include <unordered_map>
#include <memory>
#include <typeindex>
#include "ISubSystem.h"

class ObjectSubSystem;

class SubSystem
{
public:
	template <typename T, typename argument>
	T CreateSubSystem(argument)
	{
		static_assert(std::is_base_of<ISubSystem>::value, "T‚ÍIsubSystemƒNƒ‰ƒX‚Å‚Í‚ ‚è‚Ü‚¹‚ñ");

		std::shared_ptr<T> subSystem = std::make_shared<T>(std::forward<argument>());
		subSystem->Initalize();
		subSystems[typeid(T)] = subSystem;
		return subSystem;
	}

private:
	std::unordered_map<std::type_index, std::shared_ptr<SubSystem>> subSystems;
};


#pragma once
#include "CollisionObjectManager.h"
#include "Camera.h"
#include <unordered_map>
#include <memory>
#include <typeindex>

class IObject;

class WorldSubSystem
{
public:
	static WorldSubSystem& GetInstance();

	template <typename Type>
	void AddSubSystem(const std::shared_ptr<Type>& system)
	{
		static_assert(std::is_base_of<IObject, Type>::value, "Type‚ÍIObjectƒNƒ‰ƒX‚Å‚Í‚ ‚è‚Ü‚¹‚ñ");

		subSystems[typeid(Type)] = system;
	}

	template<typename Type>
	std::shared_ptr<Type> GetSubSystem() const
	{
		auto it = subSystems.find(typeid(Type));

		if (it == subSystems.end())
		{
			return nullptr;
		}

		if (auto object = it->second.lock())
		{
			return std::dynamic_pointer_cast<Type>(object);
		}

		return nullptr;
	}

	void ShutdownAll();

private:
	std::unordered_map<std::type_index, std::weak_ptr<IObject>> subSystems;
};


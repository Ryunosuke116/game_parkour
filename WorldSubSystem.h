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

	template <typename T>
	void AddSubSystem(const std::shared_ptr<T>& system)
	{
		static_assert(std::is_base_of<IObject, T>::value, "T‚ÍIObjectƒNƒ‰ƒX‚Å‚Í‚ ‚è‚Ü‚¹‚ñ");

		subSystems[typeid(T)] = system;
	}

	template<typename T>
	std::shared_ptr<T> GetSubSystem() const
	{
		auto it = subSystems.find(typeid(T));

		if (it == subSystems.end())
		{
			return nullptr;
		}

		if (auto object = it->second.lock())
		{
			return std::dynamic_pointer_cast<T>(object);
		}

		return nullptr;
	}

	void ShutdownAll();

private:
	std::unordered_map<std::type_index, std::weak_ptr<IObject>> subSystems;
};


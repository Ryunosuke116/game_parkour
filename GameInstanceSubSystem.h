#pragma once
#include <unordered_map>
#include <memory>
#include <typeindex>

class IWorldSubSystem;

class GameInstanceSubSystem
{
public:
	static GameInstanceSubSystem& GetInstance();

	template <typename Type, typename... Argument>
	std::weak_ptr<Type> AddSubSystem(Argument&&... argument)
	{
		static_assert(std::is_base_of<IWorldSubSystem, Type>::value, "Type‚ÍIsubSystemƒNƒ‰ƒX‚Å‚Í‚ ‚è‚Ü‚¹‚ñ");

		std::shared_ptr<Type> subSystem = std::make_shared<Type>(std::forward<Argument>(argument)...);
		subSystems[typeid(Type)] = subSystem;

		std::weak_ptr<Type> weakSubSystem = subSystem;
		return weakSubSystem;
	}

	template<typename Type>
	std::weak_ptr<Type> GetSubSystem() const
	{
		auto it = subSystems.find(typeid(Type));

		if (it != subSystems.end())
		{
			return std::dynamic_pointer_cast<Type>(it->second);
		}
		return std::weak_ptr<Type>();
	}

	void Create();
	void Load(const std::string& sceneName);
	void Update();
	void Draw();

	void ShutdownAll();

private:
	std::unordered_map<std::type_index, std::shared_ptr<IWorldSubSystem>> subSystems;

};


#pragma once
#include <unordered_map>
#include <memory>
#include <typeindex>

class ISubSystem;

class SubSystemManager
{
public:
	static SubSystemManager& GetInstance();

	template <typename T, typename... Argument>
	std::weak_ptr<T> AddSubSystem(Argument&&... argument)
	{
		static_assert(std::is_base_of<ISubSystem, T>::value, "T‚ÍIsubSystemƒNƒ‰ƒX‚Å‚Í‚ ‚è‚Ü‚¹‚ñ");

		std::shared_ptr<T> subSystem = std::make_shared<T>(std::forward<Argument>(argument)...);
		subSystems[typeid(T)] = subSystem;

		std::weak_ptr<T> weakSubSystem = subSystem;
		return weakSubSystem;
	}

	template<typename T>
	std::weak_ptr<T> GetSubSystem() const
	{
		auto it = subSystems.find(typeid(T));

		if (it != subSystems.end())
		{
			return std::dynamic_pointer_cast<T>(it->second);
		}
		return std::weak_ptr<T>();
	}

	void Create(const std::string& sceneName);
	void Update();
	void Draw();

	void ShutdownAll();

private:
	std::unordered_map<std::type_index, std::shared_ptr<ISubSystem>> subSystems;

};


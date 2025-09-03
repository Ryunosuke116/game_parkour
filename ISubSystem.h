#pragma once

class ISubSystem
{
public:
	virtual ~ISubSystem() = default;
	virtual void Create(const std::string& sceneName)	abstract;
	virtual void Update()								abstract;
	virtual void Draw()									abstract;
	virtual void Shutdown()								abstract;
};
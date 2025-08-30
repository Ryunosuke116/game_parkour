#pragma once
class ISubSystem
{
public:
	virtual ~ISubSystem() = default;
	virtual void Initialize()abstract;
	virtual void Shutdown()abstract;
};
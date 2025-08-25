#pragma once
#include "common.h"
#include <string>

class IEffectManager
{
public:
	//èÉêàâºëzä÷êî
	virtual void Add(const char* path, std::string& setTag,
		const float& scale)abstract;
	virtual void PlayEffectUpdate()abstract;
	virtual void SetPosition(const VECTOR setPosition,
		const std::string& tag)abstract;
	virtual void SetRotation(const VECTOR& setPosition, const std::string& tag)abstract;
	virtual void SetScale(const VECTOR& scale, const std::string& tag)abstract;
	virtual void SetSpeed(const float& speed, const std::string& tag)abstract;
	virtual void PlayEffect(const std::string& tag)abstract;
	virtual void StopEffect(const std::string& tag)abstract;

	virtual ~IEffectManager() = default;
};
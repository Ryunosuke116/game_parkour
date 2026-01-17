#pragma once
#include "common.h"
#include <string>
#include <nlohmann/json.hpp>
#include "IObject.h"


class BaseObject : public IObject
{
public:
	BaseObject();
	~BaseObject();

	void Draw()override;

	const int			GetModelHandle()		 { return modelHandle; }
	const VECTOR		GetPosition()			 { return position; }
	const std::string	GetTag()				 { return tag; }
	std::string			GetJsonTag()			 { return jsonTag; }
	const VECTOR		GetDifferencePosition()	 { return differencePosition; }

	virtual void Load(const nlohmann::json& jsonData)	abstract;

protected:
	int modelHandle;

	VECTOR position;
	VECTOR differencePosition;

	std::string tag;
	std::string jsonTag;
};


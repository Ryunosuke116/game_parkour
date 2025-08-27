#pragma once
#include "BaseObject.h"

class FieldMesh : public BaseObject
{
public:
	FieldMesh();
	~FieldMesh();

	void Initialize()							override;
	void Update(ObjectMediator& objectMediator)override;
	void Draw()									override;
	void Load(const nlohmann::json& jsonData)	override;
	void Create()								override {}

private:
	bool isPoly;
	bool isPush;
};


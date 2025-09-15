#pragma once
#include "BaseObject.h"

class FieldMesh : public BaseObject
{
public:
	FieldMesh();
	~FieldMesh();

	void Load(const nlohmann::json& jsonData)	override;
	void Create()								override;
	void Initialize()							override;
	void Update()								override;
	void Draw()									override;
	void ResultCreate()							override;
	void ResultInitialize()						override;
	void ResultUpdate()							override;

private:
	bool isPoly;
	bool isPush;
};


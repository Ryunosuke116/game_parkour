#pragma once
#include "BaseObject.h"
class FieldMesh : public BaseObject
{
public:
	FieldMesh(const char* path);
	~FieldMesh();

	void Initialize()override;
	void Update()override;
	void Draw()override;

private:
	bool isPoly;
	bool isPush;
};


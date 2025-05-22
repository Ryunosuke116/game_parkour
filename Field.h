#pragma once

class Field : public BaseObject
{
private:

public:

	Field();
	~Field();

	void Initialize()override;
	void Update()override;
	void Draw()override;


};


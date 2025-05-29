#pragma once

class Field : public BaseObject
{
private:
	bool isPush;
	bool isPoly;
public:

	Field();
	~Field();

	void Initialize()override;
	void Update()override;
	void Draw()override;


};


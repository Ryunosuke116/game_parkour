#pragma once
#include "BaseObject.h"

class BackGround : public BaseObject
{
public:
	BackGround();
	~BackGround();

	void Load(const nlohmann::json& jsonData)	override;
	void Create()								override;
	void Initialize()							override;
	void Update()								override;
	void Draw()									override;
	void ResultCreate()							override {}
	void ResultInitialize()						override {}
	void ResultUpdate()							override {}

private:
	int backGroundHandle;
	int startButtonHandle;
	int playMovieHandle;
	int startButtonX;
	int startButtonY;
	int alpha;
	int playMovieTime;
	int setTime;
	int sec;
	int addAlpha;

	bool isPlayMovie;

	static constexpr int kCanPlayMovieSec = 15;		//‰½•b‚Å“®‰æ‚ğÄ¶‚³‚¹‚é‚©
};


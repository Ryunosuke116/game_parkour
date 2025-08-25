#include "EffectManager.h"
#include "CoinObject.h"

/// <summary>
/// コンストラクタ
/// </summary>
CoinObject::CoinObject(std::shared_ptr<ISoundPlayer> sound,
	std::shared_ptr<IEffectManager> effect):
	BaseObject(),
	effectManager(effect),
	soundPlayer(sound),
	radian_Y(0.0f),
	velocity_Y(0.0f),
	isHitPlayer(false),
	deleteFlag(false),
	hitFlag(false)
{

}

/// <summary>
/// デストラクタ
/// </summary>
CoinObject::~CoinObject()
{

}

/// <summary>
/// 情報読み込み
/// </summary>
/// <param name="handle"></param>
/// <param name="pos"></param>
void CoinObject::Load(const int& handle, const VECTOR& pos)
{
	modelHandle = MV1DuplicateModel(handle);
	position = pos;
	MV1SetScale(modelHandle, VGet(objectScale, objectScale, objectScale));
	soundHandle = LoadSoundMem("material/sound/coinGet.mp3");
}

/// <summary>
/// 初期化
/// </summary>
void CoinObject::Initialize()
{
	MV1SetPosition(modelHandle, position);
	velocity_Y = 0.0f;
	radian_Y = 0.0f;
	isHitPlayer = false;
	deleteFlag = false;
	hitFlag = false;
	isSound = false;
}

/// <summary>
/// 更新
/// </summary>
/// <param name="effectManager"></param>
/// <param name="playerpos_top"></param>
/// <param name="playerPos_bottom"></param>
/// <param name="radius"></param>
/// <returns></returns>
bool CoinObject::Update(const VECTOR& playerpos_top,
	const VECTOR& playerPos_bottom,const float radius)
{
	VECTOR nearCapsulePos = HitCheck::CapsuleHitConfirmation(playerpos_top, playerPos_bottom, position, radius, 4.5f);

	if (HitCheck::HitConfirmation(position, nearCapsulePos, 4.5f, radius))
	{
		hitFlag = true;
		if (!isSound)
		{
			PlaySoundMem(soundHandle, DX_PLAYTYPE_BACK);
			soundPlayer->Play("coinGet");
			isSound = true;
		}
	}

	Rotate();

	if (hitFlag)
	{
		HitPlayerAction();
	}

	return deleteFlag;
}

/// <summary>
/// 描画
/// </summary>
void CoinObject::Draw()
{
	//printfDx("coin: %d", hitFlag);
	MV1DrawModel(modelHandle);
}

/// <summary>
/// プレイヤーと接触した時
/// </summary>
/// <param name="playerPos"></param>
void CoinObject::HitPlayerAction()
{
	if (velocity_Y <= 8.0f)
	{
		velocity_Y += addVelocity;
		position.y += addVelocity;
	}
	else
	{
		deleteFlag = true;
	}

	MV1SetPosition(modelHandle, position);
}

/// <summary>
/// コインモデルの回転制御
/// </summary>
void CoinObject::Rotate()
{
	if (!hitFlag)
	{
		radian_Y += 1.0f;
	}
	else
	{
		radian_Y += 20.0f;
	}

	//コインモデルを回転させる
	if (radian_Y >= 360.0f)
	{
		radian_Y = 0.0f;
	}

	MV1SetRotationXYZ(modelHandle, VGet(0.0f, radian_Y * DX_PI_F / 180.0f, 0.0f));
}
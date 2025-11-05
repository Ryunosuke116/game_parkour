#pragma once
#include <vector>
#include <memory>
#include "BaseObject.h"

class Camera :
	public BaseObject,
	public std::enable_shared_from_this<Camera>
{
public:
	Camera();
	~Camera();

	void Create()override;
	void Load(const nlohmann::json& jsonData)override{}
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void ResultCreate()override;
	void ResultInitialize()override;
	void ResultUpdate()override;

	void RotateUpdate();
	void AngleUpdate(const float& playerAngle);
	void ResetAngle(const float& playerAngle);
	void DistanceUpdate();
	void AdjustCameraPosition();
	void PosCalc();

	void StartUpdate(const float& timer,
		const VECTOR& playerPosition,
		const float playerAngle);
	void LayOutUpdate();
	bool CameraPosCalc(const std::vector<std::weak_ptr<BaseObject>>& collisionObjects);
	void CaluclateCameraAndTargetDistanceSize();

	///////////////////////////////////////////////////////
	// ゲッター
	///////////////////////////////////////////////////////
	VECTOR GetCameraDirection() { return cameraDirection; }
	VECTOR GetScreenCenterPosition() { return screenCenterPosition; }
	float GetCameraDistance() { return cameraDistanceSize; }
	float GetCameraAndTargetDistanceSize() { return cameraAndTargetDistanceSize; }

private:
	VECTOR cameraPosition;
	VECTOR normalCameraPosition;
	VECTOR cameraDirection;
	VECTOR centerPointSpherePos;			//カメラの中間地点にある球の中心
	VECTOR screenCenterPosition;			//画面の中心座標
	VECTOR lookPosition;					//注視点
	VECTOR rayCastStartPosition;			//rayの開始座標

	float angleRadian;
	float cameraDistanceWhenHittingObject;	//オブジェクトに当たっているときのカメラの距離
	float normalCameraDistanceSize;				//通常時のカメラ距離
	float cameraDistanceSize;					//カメラの距離
	float cameraAndTargetDistanceSize;			//カメラと目標との距離
	float nowDegree;
	float newDegree;
	float minHeight;
	float maxHeight;
	float normalDistanceProgress;			//カメラの距離の最小値から最大値までの進行度(0～1)
	float normalCameraHeightProgress;		//カメラの高さの最小値から最大値までの進行度(0～1)
	float nowDistanceProgress;
	float progressCameraPosition;
	float normalLinearProgress;

	bool isResetAngle;
	bool isHitObject;
	bool isPutBackDistance;					//通常時のカメラ距離に戻すか
	bool isPushRT;
	bool isPush;

	static constexpr float kLookRadius = 1.0f;
	static constexpr float kCameraRadius = 4.0f;
	static constexpr float kMaxDistanceSize = 70.0f;
	static constexpr float kMinDistanceSize = 15.0f;
	static constexpr float kInitializeAngle = 324.549988f; 
	static constexpr float kResultInitializeAngle = -177.55f;
	static constexpr float kInitializeDistance = 50.0f;
	static constexpr float kInitializeProgress = 0.7f;
	static constexpr float kAddCenterPosY = 15.0f;					//playerPosのY軸に加算してカメラと注視点の間の座標を求める

	const VECTOR kInitializeCameraPos = VGet(426.703827f, 238.598358f, -1275.91016f);
	const VECTOR kInitializeSpherePos = VGet(398.874207f, 222.598328f, -1238.91187f);
};


#pragma once
#include "Utils/Math/Vector3.h"
#include "Utils/Math/Matrix4x4.h"

class Transform {

public:
	Transform();

	void Initialize();

	void UpdateMatrix();

	Vector3 GetWorldPosition();

	bool IsCollisionXZ(const Transform& transform);

	/// <summary>
	/// 平行移動のみの親のセット
	/// </summary>
	/// <param name="transform">親にするTransformのポインタ</param>
	void SetWorldTranslateParent(const Transform* transform);

	/// <summary>
	/// 平行移動の親子関係の解除
	/// </summary>
	void ClearWorldTranslateParent();

public:

	Vector3 scale_;
	Vector3 rotate_;
	Vector3 translate_;

	Matrix4x4 worldMat_;

	Vector3 worldPos_;

	const Transform* parent_ = nullptr;

	const Transform* worldTranslateParent_ = nullptr;

};
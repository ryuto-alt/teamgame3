#pragma once
#include "AABB.h"
#include "Mymath.h"
#include "PlayerBullet.h"
#include <ViewProjection.h>
#include <3d/Model.h>
#include <list>
#include <Input.h>




class Enemy;

class Player {
public:
	void Initialize(Model* model,ViewProjection* camera, const Vector3& pos);
	void Update();
	void Draw();
	~Player();
	void Attack();
	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	// パリィを検出したら呼び出されるコールバック関数
	void Parry();

	Vector3 GetWorldPosition();

	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

	// AABBを取得
	AABB GetAABB();

	static inline const float kWidth = 1.0f;
	static inline const float kHeight = 1.0f;

	void SetEnemy(Enemy* enemy) { enemy_ = enemy; }

private:
	WorldTransform worldtransfrom_;

	Model* model_ = nullptr;

	Input* input_ = nullptr;

	ViewProjection* camera_ = nullptr;

	Model* modelbullet_ = nullptr;

	// 弾
	std::list<PlayerBullet*> bullets_;

	bool isDead_ = false;
	bool isParry_ = false;

	Enemy* enemy_ = nullptr;

	// 発射タイマー
	int32_t spawnTimer = 0;
};
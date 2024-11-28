#include "EnemyBullet.h"
#include <algorithm>
#include <cassert>

EnemyBullet::~EnemyBullet() { model_ = nullptr; }

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	assert(model);
	model_ = model;
	worldtransfrom_.translation_ = position;
	worldtransfrom_.Initialize();
	velocity_ = velocity; // プレイヤー方向に向かう初期速度を正しく設定
}

// ワールド座標を取得
Vector3 EnemyBullet::GetWorldPosition() {

	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldtransfrom_.matWorld_.m[3][0];
	worldPos.y = worldtransfrom_.matWorld_.m[3][1];
	worldPos.z = worldtransfrom_.matWorld_.m[3][2];

	return worldPos;
}

AABB EnemyBullet::GetAABB() {
	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

void EnemyBullet::Update() {

	if (--deathTimer_ <= 0) {
		isDead_ = true;
		return;
	}

	if (!player_)
		return;

	// 弾の現在位置とプレイヤーの位置を取得
	Vector3 bulletPosition = GetWorldPosition();
	Vector3 playerPosition = player_->GetWorldPosition();

	// プレイヤーへのベクトルを計算
	Vector3 toPlayer = Normalize(playerPosition - bulletPosition);

	// 現在の進行方向を少しずつプレイヤー方向に補正
	float adjustmentFactor = 0.05f; // 補正の割合。値を調整することで追尾の滑らかさを変更
	velocity_ = Normalize(velocity_ + toPlayer * adjustmentFactor);

	// 速度に基づいて弾の位置を更新
	worldtransfrom_.translation_.x += velocity_.x * 1.0f;
	worldtransfrom_.translation_.y += velocity_.y * 1.0f;
	worldtransfrom_.translation_.z += velocity_.z * 1.0f;

	// ワールド行列を更新
	worldtransfrom_.UpdateMatrix();
}

void EnemyBullet::OnCollision() { isDead_ = true; }

void EnemyBullet::Draw(const ViewProjection& camera) {

	if (!isDead_) {
		// モデルの描画
		model_->Draw(worldtransfrom_, camera);
	}
}
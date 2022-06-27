#pragma once

#include "GeometricObject.h"
#include "Mat3.h"
#include <memory>
#include <random>
#include "MathSH.h"
#include "GameMode.h"


class PhysicsEntity
{
public:
	enum class Type
	{
		Circle,
		Rect,
		Star
	};

	class TypeTrait
	{
	public:
		virtual ~TypeTrait() = default;
		virtual Type GetType() const = 0;
		virtual std::unique_ptr<TypeTrait> Clone() const = 0;
	};

public:
	enum class State
	{
		Normal,
		Collided,
		NeedToSplit,
		NeedToScaleUP,
		NeedToDestroy
	};

public:
	PhysicsEntity( Type type, const Vec2<int>& pos, int id );

	bool operator==( const PhysicsEntity& rhs ) const;
	bool operator!=( const PhysicsEntity& rhs ) const;

	void Update( float dt, const RECT& walls );
	void Draw( HDC hdc ) const;

	Vec2<float> GetCenter() const;
	float GetCenterX() const;
	float GetCenterY() const;
	float GetSize() const;
	float GetOuterRadius() const;
	float GetAngle() const;
	float GetScale() const;
	Type GetType() const;
	RECT GetRECT() const;
	const TypeTrait& GetEntityType() const;
	Vec2<float> GetVelocity() const;
	std::vector<Vec2<float>> GetVertices() const;
	bool WasCollided() const;

	void SetCenter( const Vec2<float>& c );
	void SetCenterX( float x );
	void SetCenterY( float y );
	void SetAngle( float angle_in );
	void SetScale( float scale_in );
	void SetVelocity( const Vec2<float>& v );
	void SetState( const State& s );
	void SetStateToCollide();
	void SetStateToSplit();
	void SetStateToScaleUP();
	void SetStateToDestroy();
	void ReboundX();
	void ReboundY();

	void DoEntityCollisionWith( PhysicsEntity& other, const GameMode& curMode, class PatternMatchingListener& listener );

private:
	void ApplyTransformation( const Mat3<float>& transformation_in );

	void MovePos( float dt )
	{
		const Vec2<float> curPos = pObj->GetCenter() + (vel * dt);
		pObj->SetCenter( curPos );
	}

	void DoWallCollision( const RECT& walls );
	bool CheckConvexOverlapWithborder( const Vec2<float>& topLeft, const Vec2<float>& bottomRight );

private:
	std::unique_ptr<GeometricObject<float>> pObj;
	std::unique_ptr<TypeTrait> pType;
	int id;
	Vec2<float> vel;
	Mat3<float> transform = Mat3<float>::Identity();
	float speed;
	float scale = 1.0f;
	float angle = 0.0f;
	float spinFreq = 0.0f;
	float time = 0.0f;
	float collideTime = 0.0f;
	State objState = State::Normal;
};
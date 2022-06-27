#pragma once

#include "PhysicsEntity.h"

class CollisionManager
{
public:
	void CircleToCircle( PhysicsEntity& circle1, PhysicsEntity& circle2 );
	void ConvexToConvex( PhysicsEntity& convex1, PhysicsEntity& convex2 );
	void ConvexToCircle( PhysicsEntity& convex, PhysicsEntity& circle );

	inline bool CheckVerticesSAT( const PhysicsEntity& refObj, const PhysicsEntity& target, Vec2<float>& minTransVec );
	inline bool CheckConvexOverlapWithConvex( PhysicsEntity& convex1, PhysicsEntity& convex2,
		Vec2<float>& minTransVec1, Vec2<float>& minTransVec2 );
	inline bool CheckCircleOverlap( const PhysicsEntity& e1, const PhysicsEntity& e2 );
	inline void CenterCorrection( PhysicsEntity& entity, const Vec2<float>& correctionVec );
	inline void SwapVelocity( PhysicsEntity& e1, PhysicsEntity& e2 );
	inline void SetCollisionStateBoth( PhysicsEntity& e1, PhysicsEntity& e2 );
	inline bool CheckConvexOverlapWitchCircle( PhysicsEntity& convex, PhysicsEntity& circle, Vec2<float>& minTransVec );
private:

};
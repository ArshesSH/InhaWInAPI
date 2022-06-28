#pragma once

#include "PhysicsEntity.h"

class CollisionManager
{
public:
	void CircleToCircle( PhysicsEntity& circle1, PhysicsEntity& circle2 )
	{
		if ( CheckCircleOverlap( circle1, circle2 ) )
		{
			// Displace this and other
			const Vec2<float> distVec = circle1.GetCenter() - circle2.GetCenter();
			const float distance = distVec.GetLength();
			const float ovelapDist = (distance - circle1.GetSize() - circle2.GetSize()) * 0.5f;
			const Vec2<float> distOverlapVec = distVec.GetNormalized() * ovelapDist;

			CenterCorrection( circle1, distOverlapVec * -1.0f );
			CenterCorrection( circle2, distOverlapVec );

			SwapVelocity( circle1, circle2 );

			circle1.SetState( PhysicsEntity::State::Collided );
			circle2.SetState( PhysicsEntity::State::Collided );
		}
	}

	void ConvexToConvex( PhysicsEntity& convex1, PhysicsEntity& convex2 )
	{
		Vec2<float> minTranslateVecConvex1;
		Vec2<float> minTranslateVecConvex2;

		if ( CheckConvexOverlapWithConvex( convex1, convex2, minTranslateVecConvex1, minTranslateVecConvex2 ) )
		{
			// Set Center Correction
			CenterCorrection( convex1, minTranslateVecConvex1 );
			CenterCorrection( convex2, minTranslateVecConvex2 );

			SwapVelocity( convex1, convex2 );

			SetCollisionStateBoth( convex1, convex2 );
		}
	}

	inline bool CheckVerticesSAT( const PhysicsEntity& refObj, const PhysicsEntity& target, Vec2<float>& minTransVec )
	{
		auto refObjVertices = refObj.GetVertices();
		auto refObjVerticesSize = refObjVertices.size();
		auto targetVertices = target.GetVertices();

		// Create Translate things
		float minTranslateScalar = INFINITY;
		Vec2<float> minTranslateNormalVec;

		// Check for each axis
		for ( int vIdx = 0; vIdx < refObjVerticesSize; ++vIdx )
		{
			const int vIdxNext = (vIdx + 1) % refObjVerticesSize;
			Vec2<float> axisProj = (refObjVertices[vIdx] - refObjVertices[vIdxNext]).GetNormalRightVec2().GetNormalized();

			float minThis = INFINITY;
			float maxThis = -INFINITY;
			for ( auto e : refObjVertices )
			{
				const float p = e * axisProj;
				minThis = (std::min)(minThis, p);
				maxThis = (std::max)(maxThis, p);
			}

			float minOther = INFINITY;
			float maxOther = -INFINITY;
			for ( auto e : targetVertices )
			{
				const float p = e * axisProj;
				minOther = (std::min)(minOther, p);
				maxOther = (std::max)(maxOther, p);
			}

			if ( !(maxOther >= minThis && maxThis >= minOther) )
			{
				return false;
			}

			// Get MinTranslate Scalr and Vector
			const float curMinTrans = maxOther - minThis;
			if ( curMinTrans < minTranslateScalar )
			{
				minTranslateScalar = curMinTrans;
				minTranslateNormalVec = axisProj;
			}
		}

		// return minimum translate Vector
		minTransVec = minTranslateNormalVec * (minTranslateScalar * 0.5);
		return true;
	}
	inline bool CheckConvexOverlapWithConvex( PhysicsEntity& convex1, PhysicsEntity& convex2,
		Vec2<float>& minTransVec1, Vec2<float>& minTransVec2 )
	{
		// First, Check Collision with Outer Circles
		if ( CheckCircleOverlap( convex1, convex2 ) )
		{
			Vec2<float> minTranslateVecConvex1;
			Vec2<float> minTranslateVecConvex2;
			// Then, Do OBB Collision detect for convex1 and convex2
			if ( CheckVerticesSAT( convex1, convex2, minTranslateVecConvex1 ) == false )
			{
				return false;
			}
			if ( CheckVerticesSAT( convex2, convex1, minTranslateVecConvex2 ) == false )
			{
				return false;
			}

			minTransVec1 = minTranslateVecConvex1;
			minTransVec2 = minTranslateVecConvex2;

			return true;
		}
		return false;
	}
	inline bool CheckCircleOverlap( const PhysicsEntity& e1, const PhysicsEntity& e2 )
	{
		Circle<float> c1( e1.GetCenter(), e1.GetOuterRadius() );
		Circle<float> c2( e2.GetCenter(), e2.GetOuterRadius() );

		const Vec2<float> distance = c1.GetCenter() - c2.GetCenter();
		const float sumOfRadius = c1.GetRadius() + c2.GetRadius();
		return fabs( distance.x * distance.x + distance.y * distance.y ) < sumOfRadius * sumOfRadius;
	}
	inline void CenterCorrection( PhysicsEntity& entity, const Vec2<float>& correctionVec )
	{
		entity.SetCenter( entity.GetCenter() + correctionVec );
	}
	inline void SwapVelocity( PhysicsEntity& e1, PhysicsEntity& e2 )
	{
		auto tmpVel = e1.GetVelocity();
		e1.SetVelocity( e2.GetVelocity() );
		e2.SetVelocity( tmpVel );
	}
	inline void SetCollisionStateBoth( PhysicsEntity& e1, PhysicsEntity& e2 )
	{
		e1.SetState( PhysicsEntity::State::Collided );
		e2.SetState( PhysicsEntity::State::Collided );
	}
	void ConvexToCircle( PhysicsEntity& convex, PhysicsEntity& circle )
	{
		Vec2<float> minTranslateVec;
		if ( CheckConvexOverlapWitchCircle( convex, circle, minTranslateVec ) )
		{
			CenterCorrection( convex, minTranslateVec );
			CenterCorrection( circle, minTranslateVec * -1.0f );

			SwapVelocity( convex, circle );

			SetCollisionStateBoth( convex, circle );
		}
	}
	inline bool CheckConvexOverlapWitchCircle( PhysicsEntity& convex, PhysicsEntity& circle, Vec2<float>& minTransVec )
	{
		if ( CheckCircleOverlap( convex, circle ) )
		{
			const auto convexVertices = convex.GetVertices();

			// Create Translate things
			float minTranslateScalar = INFINITY;
			Vec2<float> minTranslateNormalVec;

			for ( int vIdx = 0; vIdx < convexVertices.size(); ++vIdx )
			{
				const int vIdxNext = (vIdx + 1) % convexVertices.size();
				Vec2<float> axisProj = (convexVertices[vIdx] - convexVertices[vIdxNext]).GetNormalRightVec2().GetNormalized();

				float minThis = INFINITY;
				float maxThis = -INFINITY;
				for ( auto e : convexVertices )
				{
					const float p = e * axisProj;
					minThis = (std::min)(minThis, p);
					maxThis = (std::max)(maxThis, p);
				}

				float minOther = INFINITY;
				float maxOther = -INFINITY;

				const Vec2<float> normalizedAxis = axisProj.GetNormalized();
				float p = (circle.GetCenter() + (normalizedAxis * circle.GetSize())) * axisProj;
				minOther = (std::min)(minOther, p);
				maxOther = (std::max)(maxOther, p);
				p = (circle.GetCenter() - (normalizedAxis * circle.GetSize())) * axisProj;
				minOther = (std::min)(minOther, p);
				maxOther = (std::max)(maxOther, p);

				if ( !(maxOther >= minThis && maxThis >= minOther) )
				{
					return false;
				}

				// Get MinTranslate Scalr and Vector
				const float curMinTrans = maxOther - minThis;
				if ( curMinTrans < minTranslateScalar )
				{
					minTranslateScalar = curMinTrans;
					minTranslateNormalVec = axisProj;
				}
			}
			minTransVec = minTranslateNormalVec * (minTranslateScalar * 0.5f);
			return true;
		}
		return false;
	}
private:
};
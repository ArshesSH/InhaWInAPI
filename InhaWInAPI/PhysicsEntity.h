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

	class EntityType
	{
	public:
		virtual ~EntityType() = default;
		virtual Type GetType() const = 0;
		virtual std::unique_ptr<EntityType> Clone() const = 0;
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


	bool operator==( const PhysicsEntity& rhs ) const
	{
		return id == rhs.id;
	}
	bool operator!=( const PhysicsEntity& rhs ) const
	{
		return id != rhs.id;
	}

	void Update( float dt, const RECT& walls );
	void Draw( HDC hdc ) const;

	//void SetVelCollisionByTwoPointLine( const Vec2<float>& lhs, const Vec2<float>& rhs )
	//{
	//	const Vec2<float> lineDirect = (lhs - rhs).GetNormalized();
	//	const Vec2<float> changedVel = lineDirect * (vel * lineDirect) * 2.0f - vel;
	//	vel = changedVel;
	//}
	//void SetVelCollisionBy( const Vec2<float>& line )
	//{
	//	vel = line * (vel * line) * 2.0f - vel;
	//}

	//bool IsCollideWith( const PhysicsEntity& other )
	//{
	//	return pObj->IsOverlapWith( *(other.pObj) );
	//}

	Vec2<float> GetCenter() const
	{
		return pObj->GetCenter();
	}
	void SetCenter( const Vec2<float>& c )
	{
		pObj->SetCenter( c );
	}
	bool WasCollided() const
	{
		return objState == State::Collided;
	}
	int GetID() const
	{
		return id;
	}
	float GetSize() const
	{
		return pObj->GetSize();
	}
	void SetAngle( float angle_in )
	{
		angle = MathSH::WrapAngle( angle_in );
	}
	float GetAngle() const
	{
		return angle;
	}
	void SetScale( float scale_in )
	{
		scale = scale_in;
	}
	float GetScale() const
	{
		return scale;
	}
	float GetOuterRadius() const
	{
		return pObj->GetRadius();
	}
	Vec2<float> GetVelocity() const
	{
		return vel;
	}
	void SetVelocity( const Vec2<float>& v )
	{
		vel = v;
	}
	void SetState( const State& s )
	{
		objState = s;
	}
	const EntityType& GetEntityType() const
	{
		return *pType;
	}
	
	std::vector<Vec2<float>> GetVertices() const
	{
		return pObj->GetVertices();
	}

	void DoEntityCollisionWith( PhysicsEntity& other, const GameMode& curMode );
	

	void UpdateEntityByState()
	{
	}

	bool CheckCircleOverlap( const PhysicsEntity& e1, const PhysicsEntity& e2 ) const;
private:
	void MovePos( float dt )
	{
		const Vec2<float> curPos = pObj->GetCenter() + (vel * dt);
		pObj->SetCenter( curPos );
	}
	void ReboundX()
	{
		vel.x = -vel.x;
	}
	void ReboundY()
	{
		vel.y = -vel.y;
	}

	void CenterCorrection( const Vec2<float>& correctionVec )
	{
		this->SetCenter( this->GetCenter() + correctionVec );
	}

	void DoWallCollision( const RECT& walls )
	{
		if ( objType == Type::Rect )
		{
			const Vec2<float> topLeftVec{ (float)walls.left, (float)walls.top };
			const Vec2<float> bottomRightVec{ (float)walls.right, (float)walls.bottom };

			if ( CheckConvexOverlapWithborder( topLeftVec, bottomRightVec ) )
			{
			}
		}
		else
		{
			const RECT objRect = pObj->GetRECT();
			if ( objRect.left < walls.left )
			{
				pObj->SetCenterX( pObj->GetCenterX() + walls.left - objRect.left );
				ReboundX();
			}
			else if ( objRect.right > walls.right )
			{
				pObj->SetCenterX( pObj->GetCenterX() + (walls.right - objRect.right) );
				ReboundX();
			}

			if ( objRect.top < walls.top )
			{
				pObj->SetCenterY( pObj->GetCenterY() + walls.top - objRect.top );
				ReboundY();
			}
			else if ( objRect.bottom > walls.bottom )
			{
				pObj->SetCenterY( pObj->GetCenterY() + (walls.bottom - objRect.bottom) );
				ReboundY();
			}
		}
	}
	void ApplyTransformation( const Mat3<float>& transformation_in )
	{
		transform = transformation_in * transform;
	}

	// SAT Check for reference obj to target
	bool CheckVerticesSAT( const PhysicsEntity& refObj, const PhysicsEntity& target, Vec2<float>& minTransVec )
	{
		auto refObjVertices = refObj.pObj->GetVertices();
		auto refObjVerticesSize = refObjVertices.size();
		auto targetVertices = target.pObj->GetVertices();

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



	bool CheckConvexOverlapWithConvex( PhysicsEntity& convex1, PhysicsEntity& convex2 )
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

			// Set Center Correction
			convex1.CenterCorrection( minTranslateVecConvex1 );
			convex2.CenterCorrection( minTranslateVecConvex2 );

			std::swap( convex1.vel, convex2.vel );


			return true;
		}
		return false;
	}

	bool CheckConvexOverlapWithCircle( PhysicsEntity& convexEntity, PhysicsEntity& circleEntity )
	{
		if ( CheckCircleOverlap( convexEntity, circleEntity ) )
		{
			const auto convexVertices = convexEntity.pObj->GetVertices();

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
				float p = (circleEntity.GetCenter() + (normalizedAxis * circleEntity.GetSize())) * axisProj;
				minOther = (std::min)(minOther, p);
				maxOther = (std::max)(maxOther, p);
				p = (circleEntity.GetCenter() - (normalizedAxis * circleEntity.GetSize())) * axisProj;
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

			convexEntity.CenterCorrection( minTranslateNormalVec * (minTranslateScalar * 0.5f) );
			circleEntity.CenterCorrection( minTranslateNormalVec * (minTranslateScalar * -0.5f) );
			std::swap( convexEntity.vel, circleEntity.vel );

			return true;
		}
		return false;
	}

	bool CheckConvexOverlapWithCircleAlter( PhysicsEntity& convexEntity, PhysicsEntity& circleEntity )
	{
		const Vec2<float> centerAxis = circleEntity.GetCenter() - convexEntity.GetCenter();
		
		const auto convexVertices = convexEntity.pObj->GetVertices();
		
		float minConvex = INFINITY;
		float maxConvex = -INFINITY;
		for ( auto v : convexVertices )
		{
			const float p = v * centerAxis;
			minConvex = (std::min)( minConvex, p );
			maxConvex = (std::max)( maxConvex, p );
		}
		return true;
	}


	bool CheckConvexOverlapWithborder( const Vec2<float>& topLeft, const Vec2<float>& bottomRight )
	{
		// Create Normalized Horizontal and Vertical Window Sized Vectors
		const Vec2<float> NormalizedHorizontal = Vec2<float>( bottomRight.x - topLeft.x, topLeft.y ).GetNormalized();
		const Vec2<float> NormalizedVertical = Vec2<float>( topLeft.x, bottomRight.y - topLeft.y ).GetNormalized();

		// Set Projection vals
		float minHorizon = INFINITY;
		float maxHorizon = -INFINITY;
		float minVertical = INFINITY;
		float maxVertical = -INFINITY;

		std::vector<Vec2<float>> vertices = pObj->GetVertices();

		for ( auto e : vertices )
		{
			// Check Horizontal
			const float pHorizon = e * NormalizedHorizontal;
			minHorizon = (std::min)(minHorizon, pHorizon);
			maxHorizon = (std::max)(maxHorizon, pHorizon);

			// Case Left Collision
			if ( minHorizon < topLeft.x )
			{
				const Vec2<float> minimumTranslateVec = NormalizedHorizontal * (topLeft.x - e.x);
				SetCenter( GetCenter() + minimumTranslateVec );
				ReboundX();
				return true;
			}
			// Case Right Collision
			else if ( bottomRight.x < maxHorizon )
			{
				const Vec2<float> minimumTranslateVec = NormalizedHorizontal * (bottomRight.x - e.x);
				SetCenter( GetCenter() + minimumTranslateVec );
				ReboundX();
				return true;
			}

			// Check Vertical
			const float pVertical = e * NormalizedVertical;
			minVertical = (std::min)(minVertical, pVertical);
			maxVertical = (std::max)(maxVertical, pVertical);

			// Case Top Collision
			if ( minVertical < topLeft.y )
			{
				const Vec2<float> minimumTranslateVec = NormalizedVertical * (topLeft.y - e.y);
				SetCenter( GetCenter() + minimumTranslateVec );
				ReboundY();
				return true;
			}
			// Case Bottom Collision
			else if ( bottomRight.y < maxVertical )
			{
				const Vec2<float> minimumTranslateVec = NormalizedVertical * (bottomRight.y - e.y);
				SetCenter( GetCenter() + minimumTranslateVec );
				ReboundY();
				return true;
			}
		}
		return false;
	}

private:
	std::unique_ptr<GeometricObject<float>> pObj;
	std::unique_ptr<EntityType> pType;

	Type objType;

	Vec2<float> vel;
	Mat3<float> transform = Mat3<float>::Identity();
	float speed;
	float scale = 1.0f;
	float angle = 0.0f;
	float spinFreq = 0.0f;
	float time = 0.0f;
	float collideTime = 0.0f;
	int id;
	State objState = State::Normal;
};
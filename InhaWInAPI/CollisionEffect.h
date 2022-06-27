#pragma once

#include "PhysicsEntity.h"

namespace CollisionEffect
{
	class CollideType
	{
	public:
		class CircleToCircle
		{
		public:
			void operator()( PhysicsEntity& circle1, PhysicsEntity& circle2 );
		private:
		};

	public:
		class ConvexToConvex
		{
		public:
			void operator()( PhysicsEntity& convex1, PhysicsEntity& convex2 );
		private:
			inline bool CheckVerticesSAT( const PhysicsEntity& refObj, const PhysicsEntity& target, Vec2<float>& minTransVec );
			inline bool CheckConvexOverlapWithConvex( PhysicsEntity& convex1, PhysicsEntity& convex2,
				Vec2<float>& minTransVec1, Vec2<float>& minTransVec2 );
		};

	public:
		class ConvexToCircle
		{
		public:
			void operator()( PhysicsEntity& convex, PhysicsEntity& circle );
		private:
			inline bool CheckConvexOverlapWitchCircle( PhysicsEntity& convex, PhysicsEntity& circle, Vec2<float>& minTransVec );
		};

	private:
		inline static bool CheckCircleOverlap( const PhysicsEntity& e1, const PhysicsEntity& e2 );
		inline static void CenterCorrection( PhysicsEntity& entity, const Vec2<float>& correctionVec );
		inline static void SwapVelocity( PhysicsEntity& e1, PhysicsEntity& e2 );
	};


}
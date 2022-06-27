#include "PhysicsEntity.h"

#include "PhysicsEntityTypeTraits.h"
#include "PatternMatchingListener.h"
#include "CollisionEffect.h"

PhysicsEntity::PhysicsEntity( Type type, const Vec2<int>& pos, int id )
	:
	id( id )
{
	std::random_device rd;
	std::mt19937 rng( rd() );
	std::uniform_int_distribution<int> sizeGen( 20, 50 );
	std::uniform_real_distribution<float> speedGen( 100, 300 );
	std::uniform_real_distribution<float> dirXGen( -1, 1 );
	std::uniform_real_distribution<float> dirYGen( -1, 1 );
	std::uniform_real_distribution<float> rotateGen( -2, 2 );

	speed = speedGen( rng );
	vel = { dirXGen( rng ), dirYGen( rng ) };
	vel *= speed;
	spinFreq = (float)(rotateGen( rng ) * MathSH::PI);

	if ( type == Type::Rect )
	{
		const int size = sizeGen( rng ) * 2;
		pObj = std::make_unique<Rect<float>>( (float)pos.x, (float)pos.y, size, size );
		pType = std::make_unique<TypeRect>();
	}
	else if ( type == Type::Circle )
	{
		pObj = std::make_unique<Circle<float>>( (float)pos.x, (float)pos.y, sizeGen( rng ) );
		pType = std::make_unique<TypeCircle>();
	}
	else if ( type == Type::Star )
	{
		std::uniform_int_distribution<int> flareGen( 5, 9 );
		const Vec2<float> posStar{ (float)pos.x, (float)pos.y };
		pObj = std::make_unique<Star<float>>( posStar, sizeGen( rng ), flareGen( rng ) );
		pType = std::make_unique<TypeStar>();
	}
}

bool PhysicsEntity::operator==( const PhysicsEntity& rhs ) const
{
	return id == rhs.id;
}

bool PhysicsEntity::operator!=( const PhysicsEntity& rhs ) const
{
	return  id != rhs.id;
}

void PhysicsEntity::Update( float dt, const RECT& walls )
{
	{
		time += dt;
		collideTime += dt;
		MovePos( dt );
		DoWallCollision( walls );
		SetAngle( spinFreq * time );
		pObj->ApplyTransformation( Mat3<float>::Rotation( angle ) );

		if ( objState == State::Collided )
		{
			if ( collideTime >= 0.03f )
			{
				objState = State::Normal;
				collideTime = 0.0f;
			}
		}

	}
}

void PhysicsEntity::Draw( HDC hdc ) const
{
	if ( objState == State::Normal )
	{
		pObj->Draw( hdc );
	}
	else if ( objState == State::Collided )
	{
		pObj->DrawDebug( hdc );
	}
}

 Vec2<float> PhysicsEntity::GetCenter() const
{
	return pObj->GetCenter();
}

 float PhysicsEntity::GetSize() const
{
	return pObj->GetSize();
}

 float PhysicsEntity::GetOuterRadius() const
{
	return pObj->GetRadius();
}

 float PhysicsEntity::GetAngle() const
{
	return angle;
}

 float PhysicsEntity::GetScale() const
{
	return scale;
}

const PhysicsEntity::TypeTrait& PhysicsEntity::GetEntityType() const
{
	return *pType;
}

Vec2<float> PhysicsEntity::GetVelocity() const
{
	return vel;
}

std::vector<Vec2<float>> PhysicsEntity::GetVertices() const
{
	return pObj->GetVertices();
}

bool PhysicsEntity::WasCollided() const
{
	return objState == State::Collided;
}

void PhysicsEntity::SetCenter( const Vec2<float>& c )
{
	pObj->SetCenter( c );
}

 void PhysicsEntity::SetAngle( float angle_in )
{
	angle = MathSH::WrapAngle( angle_in );
}

 void PhysicsEntity::SetScale( float scale_in )
{
	scale = scale_in;
}

 void PhysicsEntity::SetVelocity( const Vec2<float>& v )
{
	vel = v;
}

 void PhysicsEntity::SetState( const State& s )
{
	objState = s;
}

void PhysicsEntity::DoEntityCollisionWith( PhysicsEntity& other, const GameMode& curMode, PatternMatchingListener& listener )
{
	//if ( !WasCollided() && !other.WasCollided() )
	{
		if ( *this != other )
		{
			Vec2<float> correctionVec;

			if ( curMode == GameMode::Collision )
			{
				listener.Switch( *this, other );


			}
		}
	}

}

inline void PhysicsEntity::ApplyTransformation( const Mat3<float>& transformation_in )
{
	transform = transformation_in * transform;
}

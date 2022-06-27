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
	std::uniform_int_distribution<int> sizeGen( minSize, maxSize );
	std::uniform_real_distribution<float> speedGen( minSpeed, maxSpeed );
	std::uniform_real_distribution<float> dirXGen( -1, 1 );
	std::uniform_real_distribution<float> dirYGen( -1, 1 );
	std::uniform_real_distribution<float> rotateGen( -roatateAmount, roatateAmount );

	vel = Vec2<float>{ dirXGen( rng ), dirYGen( rng ) } * speedGen( rng );
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

PhysicsEntity::PhysicsEntity( Type type, const Vec2<int>& pos, int id, int size_in, const Vec2<float>& vel, float angle_in, float spinFreq, int nFlares )
	:
	id( id ),
	vel( vel ),
	spinFreq( spinFreq )
{
	if ( type == Type::Rect )
	{
		pObj = std::make_unique<Rect<float>>( (float)pos.x, (float)pos.y, size_in, size_in );
		SetAngle( angle_in );
		pType = std::make_unique<TypeRect>();
	}
	else if ( type == Type::Circle )
	{
		pObj = std::make_unique<Circle<float>>( (float)pos.x, (float)pos.y, size_in );
		SetAngle( angle_in );
		pType = std::make_unique<TypeCircle>();
	}
	else if ( type == Type::Star )
	{
		const Vec2<float> posStar{ (float)pos.x, (float)pos.y };
		pObj = std::make_unique<Star<float>>( posStar, size_in, nFlares );
		SetAngle( angle_in );
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
	if ( objState == State::Collided )
	{
		pObj->DrawDebug( hdc );
	}
	else
	{
		pObj->Draw( hdc );
	}
}

 Vec2<float> PhysicsEntity::GetCenter() const
{
	return pObj->GetCenter();
}

 float PhysicsEntity::GetCenterX() const
 {
	 return pObj->GetCenterX();
 }

 float PhysicsEntity::GetCenterY() const
 {
	 return pObj->GetCenterY();
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


 PhysicsEntity::Type PhysicsEntity::GetType() const
 {
	 return pType->GetType();
 }

 RECT PhysicsEntity::GetRECT() const
 {
	 return pObj->GetRECT();
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

bool PhysicsEntity::GetStateShouldSplit() const
{
	return objState == State::ShouldSplit;
}

bool PhysicsEntity::GetStateShouldScaleUp() const
{
	return objState == State::ShouldScaleUP;
}

bool PhysicsEntity::GetStateShouldDestroy() const
{
	return objState == State::ShouldDestroy;
}




bool PhysicsEntity::WasCollided() const
{
	return objState == State::Collided;
}

float PhysicsEntity::GetSizeForAdd() const
{
	return sizeForAdd;
}

int PhysicsEntity::GetFlareCount() const
{
	return pObj->GetFlareCount();
}

float PhysicsEntity::GetSpinFreq() const
{
	return spinFreq;
}

void PhysicsEntity::AddSize( float size )
{
	pObj->AddSize( size );
}

void PhysicsEntity::SetSizeForAdd( float size )
{
	sizeForAdd = size;
}

void PhysicsEntity::SetCenter( const Vec2<float>& c )
{
	pObj->SetCenter( c );
}

void PhysicsEntity::SetCenterX( float x )
{
	pObj->SetCenterX( x );
}

void PhysicsEntity::SetCenterY( float y )
{
	pObj->SetCenterY( y );
}

 void PhysicsEntity::SetAngle( float angle_in )
{
	angle = MathSH::WrapAngle( angle_in );
}

 void PhysicsEntity::SetVelocity( const Vec2<float>& v )
{
	vel = v;
}

 void PhysicsEntity::SetState( const State& s )
{
	objState = s;
}

 void PhysicsEntity::SetStateToNormal()
 {
	 objState = State::Normal;
 }

 void PhysicsEntity::SetStateToCollide()
 {
	 objState = State::Collided;
 }

 void PhysicsEntity::SetStateShouldSplit()
 {
	 objState = State::ShouldSplit;
 }

 void PhysicsEntity::SetStateShouldScaleUP()
 {
	 objState = State::ShouldScaleUP;
 }

 void PhysicsEntity::SetStateShouldDestroy()
 {
	 objState = State::ShouldDestroy;
 }

 void PhysicsEntity::ReboundX()
 {
	 vel.x = -vel.x;
 }

 void PhysicsEntity::ReboundY()
 {
	 vel.y = -vel.y;
 }

void PhysicsEntity::DoEntityCollisionWith( PhysicsEntity& other, PatternMatchingListener& listener )
{
	//if ( !WasCollided() && !other.WasCollided() )
	{
		if ( *this != other )
		{
			Vec2<float> correctionVec;

			listener.Switch( *this, other );
		}
	}
}

inline void PhysicsEntity::ApplyTransformation( const Mat3<float>& transformation_in )
{
	transform = transformation_in * transform;
}

inline void PhysicsEntity::DoWallCollision( const RECT& walls )
{
	if ( pType->GetType() == Type::Rect )
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

inline bool PhysicsEntity::CheckConvexOverlapWithborder( const Vec2<float>& topLeft, const Vec2<float>& bottomRight )
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

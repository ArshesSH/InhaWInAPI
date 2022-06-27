#pragma once

#include <vector>
#include <memory>
#include "PhysicsEntity.h"
#include "CollisionEffect.h"

class Action
{
public:
	virtual void Do( std::vector<std::unique_ptr<PhysicsEntity>>& entities ) = 0;
};

class Collision : public Action
{
public:
	Collision( PhysicsEntity& target )
		:
		pEntity( &target )
	{}
	virtual void Do( std::vector<std::unique_ptr<PhysicsEntity>>& entities ) = 0;
private:
	PhysicsEntity* pEntity;
};

class CircleToCircleCollision : public Collision
{
	
};
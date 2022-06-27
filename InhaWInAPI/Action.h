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


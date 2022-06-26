#pragma once

#include "PhysicsEntity.h"
#include <memory>

class TypeCircle : public PhysicsEntity::EntityType
{
public:
	std::unique_ptr<EntityType> Clone() const override
	{
		return std::make_unique<TypeCircle>();
	}
	PhysicsEntity::Type GetType() const override
	{
		return PhysicsEntity::Type::Circle;
	}
};

class TypeRect : public PhysicsEntity::EntityType
{
public:
	std::unique_ptr<EntityType> Clone() const override
	{
		return std::make_unique<TypeRect>();
	}
	PhysicsEntity::Type GetType() const override
	{
		return PhysicsEntity::Type::Rect;
	}
};
class TypeStar : public PhysicsEntity::EntityType
{
public:
	std::unique_ptr<EntityType> Clone() const override
	{
		return std::make_unique<TypeStar>();
	}
	PhysicsEntity::Type GetType() const override
	{
		return PhysicsEntity::Type::Star;
	}
};
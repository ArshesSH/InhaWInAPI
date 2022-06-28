#pragma once

#include <unordered_map>
#include <functional>
#include <typeindex>
#include <type_traits>
#include "PhysicsEntity.h"

// Create TypePair
using TypePair = std::pair<const std::type_index, const std::type_index>;

// Create Hash for map
namespace std
{
	template <>
	struct hash<TypePair>
	{
		size_t operator()( const TypePair& tp ) const
		{
			const auto hash0 = tp.first.hash_code();
			return hash0 ^ (tp.second.hash_code() + 0x9e3779b9 + (hash0 << 6) + (hash0 >> 2));
		}
	};
}

class PatternMatchingListener
{
public:
	// Get TypeTrait T and U, and Functor for F, Create Both case of T,U and U,T 
	template<typename T, typename U, typename F>
	void Case( F f )
	{
		static_assert(std::is_base_of<PhysicsEntity::TypeTrait, T>::value, "Template param type T must be derived from PhysicsEntity::TypeTrait!");
		static_assert(std::is_base_of<PhysicsEntity::TypeTrait, U>::value, "Template param type U must be derived from PhysicsEntity::TypeTrait!");
		handlers[{typeid(T), typeid(U)}] = f;
		handlers[{typeid(U), typeid(T)}] = std::bind(
			f, std::placeholders::_2, std::placeholders::_1
		);
	}
	template<typename T, typename U>
	bool HasCase() const
	{
		static_assert(std::is_base_of<PhysicsEntity::TypeTrait, T>::value, "Template param type T must be derived from PhysicsEntity::TypeTrait!");
		static_assert(std::is_base_of<PhysicsEntity::TypeTrait, U>::value, "Template param type U must be derived from PhysicsEntity::TypeTrait!");
		return handlers.count( { typeid(T),typeid(U) } ) > 0;
	}
	template<typename T, typename U>
	void ClearCase()
	{
		static_assert(std::is_base_of<PhysicsEntity::TypeTrait, T>::value, "Template param type T must be derived from PhysicsEntity::TypeTrait!");
		static_assert(std::is_base_of<PhysicsEntity::TypeTrait, U>::value, "Template param type U must be derived from PhysicsEntity::TypeTrait!");
		handlers.erase( { typeid(T),typeid(U) } );
		handlers.erase( { typeid(U),typeid(T) } );
	}
	template<typename F>
	void Default( F f )
	{
		def = f;
	}
	inline void Switch( PhysicsEntity& a, PhysicsEntity& b )
	{
		auto i = handlers.find( { typeid(a.GetEntityType()),typeid(b.GetEntityType()) } );
		if ( i != handlers.end() )
		{
			i->second( a, b );
		}
		else
		{
			def( a, b );
		}
	}

private:
	std::unordered_map<TypePair, std::function<void( PhysicsEntity&, PhysicsEntity& )>> handlers;
	std::function<void( PhysicsEntity&, PhysicsEntity& )> def = []( PhysicsEntity&, PhysicsEntity& ) {};
};
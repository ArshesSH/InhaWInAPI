#include "Game.h"

void Game::ComposeFrame( const HDC& hdc )
{
	field.Draw( hdc );
}

void Game::UpdateModel()
{
	dt = ft.Mark();
	field.Update( dt, screenRect, gameMode );
}

float Game::GetDeltaTime() const
{
	return dt;
}

void Game::SetClientRECT( const RECT& r )
{
	screenRect = r;
}

void Game::SetCollisionMode()
{
	gameMode = GameMode::Collision;
}

void Game::SetCombineMode()
{
	gameMode = GameMode::Combine;
}

void Game::SetSplitMode()
{
	gameMode = GameMode::Split;
}

void Game::AddCircle( const Vec2<int>& pos )
{
	field.AddCircle( pos );
}

void Game::AddRect( const Vec2<int>& pos )
{
	field.AddRect( pos );
}

void Game::AddStar( const Vec2<int>& pos )
{
	field.AddStar( pos );
}

void Game::AddRandomGeometricObject( const Vec2<int>& pos )
{
	field.AddRandomly( pos );
}

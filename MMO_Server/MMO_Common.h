#pragma once
#include <cstdint>
#include <SFML/Graphics.hpp>
enum class GameMsg : uint32_t
{
	Server_GetStatus,
	Server_GetPing,
	Server_GetOwnTime,


	Client_Accepted,
	Client_AssignID,
	Client_RegisterWithServer,
	Client_UnregisterWithServer,
	Client_NumPlayers,
	Client_GameFull,

	Game_AddPlayer,
	Game_RemovePlayer,
	Game_UpdatePlayer,
	Game_AddBullet,
	Game_RemoveBullet,
	Game_UpdateBullet
};
//
//enum class AnimDir
//{
//	Up, NE, E, SE, S, SW, W, NW
//};


enum class AnimDir
{
	Up,
	Down,
	Left,
	Right,
	UpRight,
	UpLeft,
	DownLeft,
	DownRight,
	Invariant
};


struct sPlayerDescription
{
	uint32_t nUniqueID = 0;
	uint32_t nAvatarID = 0;

	uint32_t nHealth = 100;
	uint32_t nAmmo = 20;
	uint32_t nKills = 0;
	uint32_t nDeaths = 0;

	float fRadius = 0.5f;

	sf::Vector2f vPos = { 0.f,0.f };
	sf::Vector2f vVel = {0.f,0.f};

	
	float currSizeX{ 0.f };
	float currSizeY{ 0.f };
	uint32_t texRectLeft{ 0u };
	uint32_t texRectTop{ 0u };
	uint32_t texRectWidth{ 0u };
	uint32_t texRectHeight{ 0u };


	float currOffsetX{ 0.f };
	float currOffsetY{ 0.f };

	uint32_t texIDStr{ 0 };
	uint32_t dir = 1;
	uint32_t currFrame = 0;



};

struct BulletDescription
{
	uint32_t nUniqueID = 0;
	float fRad = 1.f;

	sf::Vector2f pos = { 0.f,0.f };
	sf::Vector2f vel = { 0.f,0.f };

	uint32_t index = 0;


};

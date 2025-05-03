#pragma once
#include <cstdint>
#include <SFML/Graphics.hpp>
enum class GameMsg : uint32_t
{
	Server_GetStatus,
	Server_GetPing,

	Client_Accepted,
	Client_AssignID,
	Client_RegisterWithServer,
	Client_UnregisterWithServer,

	Game_AddPlayer,
	Game_RemovePlayer,
	Game_UpdatePlayer,
};

enum class AnimDir
{
	N,NE,E,SE,S,SW,W,NW
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

	AnimDir dir = AnimDir::S;
	uint32_t currFrame = 0;
	

};

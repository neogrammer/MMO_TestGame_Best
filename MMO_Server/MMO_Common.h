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

	Game_AddPlayer,
	Game_RemovePlayer,
	Game_UpdatePlayer,
	Game_AddBullet,
	Game_RemoveBullet,
	Game_UpdateBullet
};

enum class AnimDir
{
	N,NE,E,SE,S,SW,W,NW
};

struct TimeSync
{
	std::chrono::system_clock::time_point timeBegin{};
	std::chrono::system_clock::time_point timeReachingServer{};
	std::chrono::system_clock::time_point timeFromServer{};

};


struct TimePlayer
{
	TimeSync ts;
	uint32_t id;

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
	
	float dt = { 0.f };


	// when creating a player ping the server, the server gets the ping, calculates the time at the server, updates the message and sends it back where client calcs time once more anduses those threee times to store time to server, and from server time displacements
	//  with this equipped, during player updates, they send their own local dt to the server plus the time to server time, and the server sends this out to each client respectively,
	//  when the other client gets it from the server, they use their from server time and add it to the latency for a total dt, which is used on that client for each client on that frame providing accurate dts, regardless of whether the time is off on any machine whatsoever
	//  im a genius
	
	// send time
	// to server time
	// receive time
	// to destination timr


};

struct BulletDescription
{
	uint32_t nUniqueID = 0;
	float fRad = 1.f;

	sf::Vector2f pos = { 0.f,0.f };
	sf::Vector2f vel = { 0.f,0.f };

	uint32_t index = 0;


};

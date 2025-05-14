#ifndef STAGEDATA_H__
#define STAGEDATA_H__

#include <string>
#include <vector>
#include <memory>
#include <resources/Cfg.h>


struct Tile
{
	enum class Type : int32_t
	{
		NOTILE = -1,
		PASS = 0,
		SOLID = 1,
		TEMP = 2,
		HORIZONTAL = 3,
		VERTICAL = 4,
		ABOVE = 5,
		MNORTH = 6,
		MEAST = 7,
		MSOUTH = 8,
		MWEST = 9
	};
	


	uint32_t index{ 0 };
	Type type{ Type::NOTILE };
};

enum class LinkType : uint8_t {               // encode however you like
	StairsUp,
	StairsDown,
	Door,
	WeakBlock,
	Unknown
};

struct Link
{
	int32_t toFloor{0};
	uint32_t x{0};
	uint32_t y{0};

	LinkType type{ LinkType::Unknown };
};

struct Floor
{
	int32_t id{ 0 };
	uint32_t width{};
	uint32_t height{};

	std::vector<uint32_t> tileIndices;

	std::vector<Link> links;
};

struct TilesetData
{
	uint32_t width{ 0 };
	uint32_t height{ 0 };
	std::vector<Tile> tiles;
};

struct MapData
{
	int32_t defaultFloor{ 0 };
	std::vector<Floor> floors;
};

struct SpawnPoint {
	uint32_t id = 0;
	uint32_t x = 0;
	uint32_t y = 0;
	int32_t  floor = 0;
};

struct Interactive {
	uint32_t tileIndex = 0;
	std::string type;                         // "WeakBlock", "Door", …
	int32_t  floor = 0;
	std::string targetStage;                  // optional
};

struct StageData {
	std::string          name;
	Cfg::Textures          textureID{ Cfg::Textures::Invariant };           // keep as string; client maps → enum
	uint32_t             tileSize = 0;
	uint32_t             revision = 0;

	MapData              map;                 // **by value** – no heap indirection
	TilesetData		 tileset;
	std::vector<SpawnPoint>  spawnPoints;
	std::vector<Interactive> interactives;
};

#endif
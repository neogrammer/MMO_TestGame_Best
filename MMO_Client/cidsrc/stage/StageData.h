#ifndef STAGEDATA_H__
#define STAGEDATA_H__

#include <string>
#include <vector>
#include <memory>


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

	std::vector<uint32_t> tiles;

	std::vector<Link> links;
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
	std::string          textureID;           // keep as string; client maps → enum
	uint32_t             tileSize = 0;
	uint32_t             revision = 0;

	MapData              map;                 // **by value** – no heap indirection
	std::vector<SpawnPoint>  spawnPoints;
	std::vector<Interactive> interactives;
};

#endif
#include "StageLoader.h"
#include <fstream>
#include <stdexcept>
#include <unordered_map>
#include <animation/Animation.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace StageLoader;

/* ───────────────── helpers ───────────────── */
static LinkType stringToLinkType(const std::string& s) {
    if (s == "StairsUp")   return LinkType::StairsUp;
    if (s == "StairsDown") return LinkType::StairsDown;
    if (s == "Door")       return LinkType::Door;
    if (s == "WeakBlock")  return LinkType::WeakBlock;
    return LinkType::Unknown;
}
static std::string linkTypeToString(LinkType t) {
    switch (t) {
    case LinkType::StairsUp:   return "StairsUp";
    case LinkType::StairsDown: return "StairsDown";
    case LinkType::Door:       return "Door";
    case LinkType::WeakBlock:  return "WeakBlock";
    default: return "Unknown";
    }
}

static Tile::Type Int32ToTileType(int32_t i) {
    if (i == -1)   return Tile::Type::NOTILE;
    if (i ==  0)   return Tile::Type::PASS;
    if (i ==  1)   return Tile::Type::SOLID;
    if (i ==  2)   return Tile::Type::TEMP;
    if (i ==  3)   return Tile::Type::HORIZONTAL;
    if (i ==  4)   return Tile::Type::VERTICAL;
    if (i ==  5)   return Tile::Type::ABOVE;
    if (i ==  6)   return Tile::Type::MNORTH;
    if (i ==  7)   return Tile::Type::MEAST;
    if (i ==  8)   return Tile::Type::MSOUTH;
    if (i ==  9)   return Tile::Type::MWEST;
    return Tile::Type::NOTILE;
}
static int32_t TileTypeToInt32(Tile::Type t) {
    switch (t) {
    case Tile::Type::NOTILE:   return -1;
    case Tile::Type::PASS:   return 0;
    case Tile::Type::SOLID:   return 1;
    case Tile::Type::TEMP:   return 2;
    case Tile::Type::HORIZONTAL:   return 3;
    case Tile::Type::VERTICAL:   return 4;
    case Tile::Type::ABOVE:   return 5;
    case Tile::Type::MNORTH:   return 6;
    case Tile::Type::MEAST:   return 7;
    case Tile::Type::MSOUTH:   return 8;
    case Tile::Type::MWEST:   return 9;
    default: return -1;
    }
}



/* ───────────────── loader ───────────────── */
StageData StageLoader::loadFromJsonFile(const std::filesystem::path& file)
{
    std::ifstream ifs(file, std::ios::binary);
    if (!ifs) throw std::runtime_error("StageLoader: cannot open " + file.string());

    json j;  ifs >> j;
    StageData sd;

    /* basic fields */
    sd.name = j.at("name").get<std::string>();
    sd.textureID = TextureIDLUT[j.at("textureID").get<std::string>()];
    sd.tileSize = j.value("tileSize", 64u);
    sd.revision = j.value("revision", 0u);

    const auto& jt = j.at("tileset");
    sd.tileset.width = jt.value("width", 1u);
    sd.tileset.height = jt.value("height", 1u);
    sd.tileset.tiles.clear();
    sd.tileset.tiles.reserve(sd.tileset.width * sd.tileset.height);
    const auto& jtypeRows = jt.at("type");

    uint32_t index{ 0 };
    for (const auto& row : jtypeRows) {
        if (row.size() != sd.tileset.width)
            throw std::runtime_error("StageLoader: row width mismatch in tileset");
        if (jtypeRows.size() != sd.tileset.height)
            throw std::runtime_error("StageLoader: col height mismatch in tileset");

        for (auto v : row)
        {
            Tile& tile = sd.tileset.tiles.emplace_back(Tile{});
            tile.index = index++;
            tile.type = Int32ToTileType(static_cast<uint32_t>(v));
        }
    }

    /* map wrapper */
    const auto& jm = j.at("map");
    sd.map.defaultFloor = jm.value("defaultFloor", 0);

    for (const auto& jf : jm.at("floors")) {
        Floor f;
        f.id = jf.at("id").get<int32_t>();
        f.width = jf.at("width").get<uint32_t>();
        f.height = jf.at("height").get<uint32_t>();

        /* tiles */
        const auto& rows = jf.at("data");
        f.tileIndices.reserve(f.width * f.height);
        for (const auto& row : rows) {
            if (row.size() != f.width)
                throw std::runtime_error("StageLoader: row width mismatch in floor " + std::to_string(f.id));
            for (auto v : row) f.tileIndices.push_back(static_cast<uint32_t>(v));
        }
        if (rows.size() != f.height)
            throw std::runtime_error("StageLoader: row count mismatch in floor " + std::to_string(f.id));

        /* links */
        if (jf.contains("links")) {
            for (const auto& jl : jf["links"]) {
                Link ln;
                ln.toFloor = jl.at("toFloor").get<int32_t>();
                ln.x = jl.at("x").get<uint32_t>();
                ln.y = jl.at("y").get<uint32_t>();
                ln.type = stringToLinkType(jl.at("type").get<std::string>());
                f.links.push_back(ln);
            }
        }
        sd.map.floors.push_back(std::move(f));
    }

    /* spawn points */
    if (j.contains("spawnPoints")) {
        for (const auto& js : j["spawnPoints"]) {
            SpawnPoint sp;
            sp.id = js.at("id").get<uint32_t>();
            sp.x = js.at("x").get<uint32_t>();
            sp.y = js.at("y").get<uint32_t>();
            sp.floor = js.at("floor").get<int32_t>();
            sd.spawnPoints.push_back(sp);
        }
    }

    /* interactive */
    if (j.contains("interactive")) {
        for (const auto& ji : j["interactive"]) {
            Interactive iv;
            iv.tileIndex = ji.at("tileIndex").get<uint32_t>();
            iv.type = ji.at("type").get<std::string>();
            iv.floor = ji.at("floor").get<int32_t>();
            if (ji.contains("targetStage"))
                iv.targetStage = ji["targetStage"].get<std::string>();
            sd.interactives.push_back(std::move(iv));
        }
    }

    return sd;
}

/* ───────────────── saver (optional) ───────────────── */
void StageLoader::saveToJsonFile(const StageData& stage, const std::filesystem::path& file)
{
    json j;
    j["name"] = stage.name;
    j["textureID"] = TexStringLUT[stage.textureID];
    j["tileSize"] = stage.tileSize;
    j["revision"] = stage.revision;


  
    json jTileset;
    jTileset["width"] = stage.tileset.width;
    jTileset["height"] = stage.tileset.height;

    json typeRows = json::array();
    for (uint32_t y = 0; y < stage.tileset.height; ++y)
    {
        json row = json::array();
        for (uint32_t x = 0; x < stage.tileset.width; ++x)
        {
            const Tile& t = stage.tileset.tiles[y * stage.tileset.width + x];
            row.push_back(TileTypeToInt32(t.type));     // encode enum → int
        }
        typeRows.push_back(std::move(row));
    }
    jTileset["type"] = std::move(typeRows);

    j["tileset"] = std::move(jTileset);
    

    json jm;
    jm["defaultFloor"] = stage.map.defaultFloor;
    json floorsJson = json::array();
    for (const auto& f : stage.map.floors) {
        json jf;
        jf["id"] = f.id;
        jf["width"] = f.width;
        jf["height"] = f.height;

        /* tiles back to rows */
        json rows = json::array();
        for (uint32_t y = 0; y < f.height; ++y) {
            json row = json::array();
            for (uint32_t x = 0; x < f.width; ++x)
                row.push_back(f.tileIndices[y * f.width + x]);
            rows.push_back(std::move(row));
        }
        jf["data"] = std::move(rows);

        /* links */
        if (!f.links.empty()) {
            json jlinks = json::array();
            for (const auto& ln : f.links) {
                jlinks.push_back({
                    {"toFloor", ln.toFloor},
                    {"x", ln.x}, {"y", ln.y},
                    {"type", linkTypeToString(ln.type)}
                    });
            }
            jf["links"] = std::move(jlinks);
        }
        floorsJson.push_back(std::move(jf));
    }
    jm["floors"] = std::move(floorsJson);
    j["map"] = std::move(jm);

    /* spawn */
    if (!stage.spawnPoints.empty()) {
        json arr = json::array();
        for (const auto& sp : stage.spawnPoints)
            arr.push_back({ {"id",sp.id},{"x",sp.x},{"y",sp.y},{"floor",sp.floor} });
        j["spawnPoints"] = std::move(arr);
    }

    /* interactive */
    if (!stage.interactives.empty()) {
        json arr = json::array();
        for (const auto& iv : stage.interactives) {
            json obj = { {"tileIndex",iv.tileIndex},{"type",iv.type},{"floor",iv.floor} };
            if (!iv.targetStage.empty()) obj["targetStage"] = iv.targetStage;
            arr.push_back(std::move(obj));
        }
        j["interactive"] = std::move(arr);
    }

    std::ofstream ofs(file, std::ios::binary);
    ofs << j.dump(2);
}

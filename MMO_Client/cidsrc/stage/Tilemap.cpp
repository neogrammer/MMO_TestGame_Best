#include "Tilemap.h"

bool TileMap::build(const StageData& sd)
{
    
    textureID = sd.textureID;
    currVerts.clear();
    currVerts.setPrimitiveType(sf::PrimitiveType::Triangles);
    currVerts.resize(sd.map.floors[sd.map.defaultFloor].width * sd.map.floors[sd.map.defaultFloor].height * 6);
    tileSize = sd.tileSize;
    const unsigned ts = sd.tileSize;
    auto mapWidth = sd.map.floors[sd.map.defaultFloor].width;
    mapPitch = mapWidth;
    auto mapHeight = sd.map.floors[sd.map.defaultFloor].height;
    this->mapHeight = mapHeight;
    sf::Texture& tex = Cfg::textures.get(textureID);
    auto defFloor = sd.map.defaultFloor;
    tilemap.clear();
    tilemap.reserve(sd.map.floors[defFloor].tileIndices.size());
    for (unsigned y = 0; y < mapHeight; ++y)
        for (unsigned x = 0; x < mapWidth; ++x)
        {
            int id = sd.map.floors[defFloor].tileIndices[y * mapWidth + x];
            tilemap.emplace_back((uint32_t)id);
            if (id < 0) continue; // -1 = empty
            unsigned tu = id % (tex.getSize().x / ts);
            unsigned tv = id / (tex.getSize().x / ts);
            sf::Vertex* quad = &currVerts[(x + y * mapWidth) * 6];
            sf::Vector2f pos((float)x * (float)ts, (float)y * (float)ts);
            sf::Vector2f uv((float)tu * (float)ts, (float)tv * (float)ts);
            // two triangles ↙↗
            quad[0].position = pos;
            quad[1].position = pos + sf::Vector2f((float)ts, 0.f);
            quad[2].position = pos + sf::Vector2f((float)ts, (float)ts);
            quad[3].position = pos;
            quad[4].position = pos + sf::Vector2f((float)ts, (float)ts);
            quad[5].position = pos + sf::Vector2f(0.f, (float)ts);
            quad[0].texCoords = uv;
            quad[1].texCoords = uv + sf::Vector2f((float)ts, 0.f);
            quad[2].texCoords = uv + sf::Vector2f((float)ts, (float)ts);
            quad[3].texCoords = uv;
            quad[4].texCoords = uv + sf::Vector2f((float)ts, (float)ts);
            quad[5].texCoords = uv + sf::Vector2f(0.f, (float)ts);
        }


    tileset.width = sd.tileset.width;
    tileset.height = sd.tileset.height;
    tileset.tiles.clear();
    tileset.tiles.reserve(tileset.width * tileset.height);

    uint32_t index{ 0 };
    for (uint32_t y = 0; y < tileset.height; ++y)
        for (uint32_t x = 0; x < tileset.width; ++x)
        {
            Tile& tile = tileset.tiles.emplace_back(Tile{});
            tile.index = index++;
            tile.type = sd.tileset.tiles[y * tileset.width + x].type;
        }

    return true;
    
}

void TileMap::update(sf::Vector2f pos_, sf::RenderWindow& wnd_)
{
    // assume pos_ is the (0,0) pixel on the screen, representing (mapoffX, mapoffY) of the view the player sees currently
    auto size = wnd_.getView().getSize();   // or window.getSize() if you don’t use a view

    // number of *fully covered* tiles  → add an extra 2‑tile cushion
    unsigned w = unsigned((((unsigned)wnd_.getView().getSize().x + (unsigned)(tileSize - 1)) / (unsigned)tileSize) + 1u);   // ceil + )1
    if (w > mapPitch) w = mapPitch;

    unsigned h = unsigned((((unsigned)wnd_.getView().getSize().y + (unsigned)(tileSize - 1)) / (unsigned)tileSize) + 1u);
    if (h > mapHeight) h = mapHeight;

    topLeftTile.x = std::clamp<int>((int)(pos_.x / tileSize), 0, (int)mapPitch - int(w));
    topLeftTile.y = std::clamp<int>((int)(pos_.y / tileSize), 0, (int)mapHeight - int(h));

    bottomRightTile = { topLeftTile.x + (int)w, topLeftTile.y + (int)h };  // no “‑1” anymore
    if (bottomRightTile.x >= (int)mapPitch)
    {
        bottomRightTile.x = (int)mapPitch;
    }
    if (bottomRightTile.y >= (int)mapHeight)
    {
        bottomRightTile.y = (int)mapHeight;
    }

    //bottomRightTile = {(int)( std::min(topLeftTile.x + (w-1), mapPitch - 1)), (int)(std::min(topLeftTile.y + (h-1), mapHeight - 1) )};
    auto ts = tileSize;
    sf::Texture& tex = Cfg::textures.get(textureID);
    if (currVerts.getVertexCount() != (bottomRightTile.x - topLeftTile.x) * (bottomRightTile.y - topLeftTile.y) * 6)
    {
        std::cout << "Vertices drawn to screen each frame before optimization: " << currVerts.getVertexCount() << "\nAnd vertex count now: ";
        currVerts.clear();
        currVerts.resize((bottomRightTile.x - topLeftTile.x) * (bottomRightTile.y - topLeftTile.y) * 6);
        std::cout << currVerts.getVertexCount() << std::endl;
    }
    for (int y = topLeftTile.y; y < bottomRightTile.y; ++y)
        for (int x = topLeftTile.x; x < bottomRightTile.x; ++x)
        {
            int id = tilemap[y * mapPitch + x];
            if (id == -1 || Tile::Type::ABOVE == tileset.tiles[tilemap[y*mapPitch+x]].type) continue; // -1 = empty
            unsigned tu = id % (tex.getSize().x / ts);
            unsigned tv = id / (tex.getSize().x / ts);
            sf::Vertex* quad = &currVerts[((x - topLeftTile.x) + (y - topLeftTile.y) * (bottomRightTile.x - topLeftTile.x)) * 6];
            sf::Vector2f pos((float)x * (float)ts, (float)y * (float)ts);
            sf::Vector2f uv((float)tu * (float)ts, (float)tv * (float)ts);
            // two triangles ↙↗
            quad[0].position = pos;
            quad[1].position = pos + sf::Vector2f((float)ts, 0.f);
            quad[2].position = pos + sf::Vector2f((float)ts, (float)ts);
            quad[3].position = pos;
            quad[4].position = pos + sf::Vector2f((float)ts, (float)ts);
            quad[5].position = pos + sf::Vector2f(0.f, (float)ts);
            quad[0].texCoords = uv;
            quad[1].texCoords = uv + sf::Vector2f((float)ts, 0.f);
            quad[2].texCoords = uv + sf::Vector2f((float)ts, (float)ts);
            quad[3].texCoords = uv;
            quad[4].texCoords = uv + sf::Vector2f((float)ts, (float)ts);
            quad[5].texCoords = uv + sf::Vector2f(0.f, (float)ts);
        }
}

uint32_t TileMap::getTileSize()
{
    return tileSize;
}

uint32_t TileMap::getMapWidth()
{
    return mapPitch;
}

uint32_t TileMap::getMapHeight()
{
    return mapHeight;
}

Tile::Type TileMap::getTileType(uint32_t index)
{
    return tileset.tiles[tilemap[index]].type;
}

void TileMap::draw(sf::RenderTarget& tgt, sf::RenderStates s) const
{
    s.texture = &Cfg::textures.get(textureID);
    tgt.draw(currVerts, s);
}

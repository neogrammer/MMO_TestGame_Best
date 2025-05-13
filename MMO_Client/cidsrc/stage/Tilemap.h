#ifndef TILEMAP_H__
#define TILEMAP_H__
#include "StageData.h"
#include <game_objects/GameObject.h>
#include <resources/Cfg.h>
#include <iostream>
class TileMap final :  public sf::Drawable
{
public:
    bool build(const StageData& sd)
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
        tilemap.reserve(sd.map.floors[defFloor].tiles.size());
        for (unsigned y = 0; y < mapHeight; ++y)
            for (unsigned x = 0; x < mapWidth; ++x)
            {
                int id = sd.map.floors[defFloor].tiles[y * mapWidth + x];
                tilemap.emplace_back((uint16_t)id);
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
        return true;
    }

    /// <summary>
    ///  Call after the game map is in the final spot for the frame, or it will be rendering at the wrong spot in the game world
    /// </summary>
    void update(sf::Vector2f pos_)
    {
        // assume pos_ is the (0,0) pixel on the screen, representing (mapoffX, mapoffY) of the view the player sees currently
        auto w = sf::VideoMode::getDesktopMode().size.x / tileSize;
        auto h = sf::VideoMode::getDesktopMode().size.y / tileSize;
        topLeftTile = sf::Vector2i((int)(pos_.x / tileSize), (int)(pos_.y / tileSize));
        if (topLeftTile.x < 0)
        {
            topLeftTile.x = 0;
        }
        if (topLeftTile.y < 0)
        {
            topLeftTile.y = 0;
        }
        if (topLeftTile.x >= (int)(mapPitch - w))
        {
            topLeftTile.x = (int)(mapPitch - w);
        }
        if (topLeftTile.y >= (int)(mapHeight - h))
        {
            topLeftTile.y = (int)(mapPitch - h);
        }
        bottomRightTile = {(int)( std::min(topLeftTile.x + (w-1), mapPitch)), (int)(std::min(topLeftTile.y + (h-1), mapHeight) )};
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
                if (id < 0) continue; // -1 = empty
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
private:
    sf::VertexArray currVerts;
    void draw(sf::RenderTarget& tgt, sf::RenderStates s) const override
    {
        s.texture = &Cfg::textures.get(textureID);
        tgt.draw(currVerts, s);
    }
    Cfg::Textures textureID; // awesome way to handle textures
    uint32_t tileSize;
    uint32_t mapPitch;
    uint32_t mapHeight;
    sf::Vector2i topLeftTile{ 0,0 };
    sf::Vector2i bottomRightTile{ 0,0 };
    std::vector<uint16_t> tilemap;
};

#endif
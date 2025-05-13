#ifndef TILEMAP_H__
#define TILEMAP_H__
#include "StageData.h"
#include <game_objects/GameObject.h>
#include <resources/Cfg.h>
class TileMap final :  public sf::Drawable
{
public:
    bool build(const StageData& sd)
    {
        textureID = sd.textureID;
        vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
        vertices.resize(sd.map.floors[sd.map.defaultFloor].width * sd.map.floors[sd.map.defaultFloor].height * 6);

        const unsigned ts = sd.tileSize;
        auto mapWidth = sd.map.floors[sd.map.defaultFloor].width;
        auto mapHeight = sd.map.floors[sd.map.defaultFloor].height;
        sf::Texture& tex = Cfg::textures.get(textureID);
        auto defFloor = sd.map.defaultFloor;
        for (unsigned y = 0; y < mapHeight; ++y)
            for (unsigned x = 0; x < mapWidth; ++x)
            {
                int id = sd.map.floors[defFloor].tiles[y * mapWidth + x];
                if (id < 0) continue; // -1 = empty

                unsigned tu = id % (tex.getSize().x / ts);
                unsigned tv = id / (tex.getSize().x / ts);

                sf::Vertex* quad = &vertices[(x + y * mapWidth) * 6];

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
  
private:
    sf::VertexArray vertices;
    void draw(sf::RenderTarget& tgt, sf::RenderStates s) const override
    {
        s.texture = &Cfg::textures.get(textureID);
        tgt.draw(vertices, s);
    }
    Cfg::Textures textureID; // awesome way to handle textures
};

#endif
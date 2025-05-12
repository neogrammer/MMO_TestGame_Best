#ifndef TILEMAP_H__
#define TILEMAP_H__
#include "StageData.h"
//
//class TileMap final : public GameObject
//{
//public:
//    bool build(const StageData& sd)
//    {
//        if (!_texture.loadFromFile(sd.tilesetPath)) return false;
//        _vertices.setPrimitiveType(sf::Triangles);
//        _vertices.resize(sd.mapSize.x * sd.mapSize.y * 6);
//
//        const unsigned ts = sd.tileSize;
//        for (unsigned y = 0; y < sd.mapSize.y; ++y)
//            for (unsigned x = 0; x < sd.mapSize.x; ++x)
//            {
//                int id = sd.tileIDs[y * sd.mapSize.x + x];
//                if (id < 0) continue; // -1 = empty
//
//                unsigned tu = id % (_texture.getSize().x / ts);
//                unsigned tv = id / (_texture.getSize().x / ts);
//
//                sf::Vertex* quad = &_vertices[(x + y * sd.mapSize.x) * 6];
//
//                sf::Vector2f pos(x * ts, y * ts);
//                sf::Vector2f uv(tu * ts, tv * ts);
//
//                // two triangles ↙↗
//                quad[0].position = pos;
//                quad[1].position = pos + sf::Vector2f(ts, 0);
//                quad[2].position = pos + sf::Vector2f(ts, ts);
//
//                quad[3].position = pos;
//                quad[4].position = pos + sf::Vector2f(ts, ts);
//                quad[5].position = pos + sf::Vector2f(0, ts);
//
//                quad[0].texCoords = uv;
//                quad[1].texCoords = uv + sf::Vector2f(ts, 0);
//                quad[2].texCoords = uv + sf::Vector2f(ts, ts);
//                quad[3].texCoords = uv;
//                quad[4].texCoords = uv + sf::Vector2f(ts, ts);
//                quad[5].texCoords = uv + sf::Vector2f(0, ts);
//            }
//        return true;
//    }
//
//private:
//    sf::VertexArray _vertices;
//    virtual void draw(sf::RenderTarget& tgt, sf::RenderStates s) const override
//    {
//        s.texture = &_texture;
//        tgt.draw(_vertices, s);
//    }
//    sf::Texture _texture; // NOTE: lightweight in SFML3 but no default ctor
//};

#endif
#ifndef TILEMAP_H__
#define TILEMAP_H__
#include "StageData.h"
#include <game_objects/GameObject.h>
#include <resources/Cfg.h>
#include <iostream>
class TileMap final :  public sf::Drawable
{
public:
    bool build(const StageData& sd);
  

    /// <summary>
    ///  Call after the game map is in the final spot for the frame, or it will be rendering at the wrong spot in the game world
    /// </summary>
    void update(sf::Vector2f pos_, sf::RenderWindow& wnd_);
    Tile::Type getTileType(uint32_t index);
    uint32_t getTileSize();
    uint32_t getMapWidth();
    uint32_t getMapHeight();

private:
    sf::VertexArray currVerts;
    void draw(sf::RenderTarget& tgt, sf::RenderStates s) const override;
  
    Cfg::Textures textureID; // awesome way to handle textures
    uint32_t tileSize;
    uint32_t mapPitch;
    uint32_t mapHeight;
    sf::Vector2i topLeftTile{ 0,0 };
    sf::Vector2i bottomRightTile{ 0,0 };
    std::vector<uint32_t> tilemap;

    TilesetData tileset;
};

#endif
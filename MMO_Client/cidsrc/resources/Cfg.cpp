#include "Cfg.h"


ResourceManager<sf::Texture, Cfg::Textures> Cfg::textures = {};
ResourceManager<sf::Font, Cfg::Fonts> Cfg::fonts = {};
ResourceManager<sf::Music, Cfg::Music> Cfg::music = {};
ResourceManager<sf::SoundBuffer, Cfg::Sounds> Cfg::sounds = {};

//ActionMap<int> Cfg::playerInputs = {};

void Cfg::Initialize()
{
    initTextures();
    initFonts();
	initMusic();
    initSounds();
}

void Cfg::Uninitialize()
{
	Cfg::textures.unloadAll();
}

void Cfg::initMusic()
{
}

void Cfg::initSounds()
{
}

void Cfg::initTextures()
{
	textures.load(Textures::PlayerAtlas, "assets/textures/players/playerSheet2.png");
	textures.load(Textures::Stage_1_Tileset, "assets/textures/tilesets/stage_1_tileset.png");
	textures.load(Textures::Invariant, "assets/textures/misc/Invariant.png");


}

void Cfg::destroyTextures()
{
	
}


void Cfg::initFonts()
{
	fonts.load(Fonts::Font1, "assets/fonts/font1.ttf");
}
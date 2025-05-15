#ifndef ANIMATION_H__
#define ANIMATION_H__
#include <map>
#include <animation/AnimTypes.h>
#include <vector>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>
#include <resources/Cfg.h>
#include <MMO_Common.h>


extern std::unordered_map<std::string, AnimName> AnimNameLUT;
extern std::unordered_map < std::string, AnimDir> DirectionLUT;
extern std::unordered_map < std::string, Cfg::Textures> TextureIDLUT;
extern std::unordered_map < Cfg::Textures, std::string> TexStringLUT;
extern std::unordered_map < std::string, AnimSheetType> AnimSheetTypeLUT;
extern std::unordered_map < AnimName, std::string > FSMStateNameLUT;
extern std::unordered_map < std::string, AnimName > FSMAnimNameLUT;


struct Animation
{
public:
	int numFrames{ 0 };
	
	std::vector<sf::IntRect> leftFrames{};
	std::vector<sf::IntRect> rightFrames{};
	std::vector<sf::IntRect> downFrames{};
	std::vector<sf::IntRect> upFrames{};
	std::vector<sf::IntRect> upLeftFrames{};
	std::vector<sf::IntRect> upRightFrames{};
	std::vector<sf::IntRect> downLeftFrames{};
	std::vector<sf::IntRect> downRightFrames{};

	std::vector<sf::Vector2f> leftOffsets{};
	std::vector<sf::Vector2f> rightOffsets{};
	std::vector<sf::Vector2f> downOffsets{};
	std::vector<sf::Vector2f> upOffsets{};
	std::vector<sf::Vector2f> upLeftOffsets{};
	std::vector<sf::Vector2f> upRightOffsets{};
	std::vector<sf::Vector2f> downLeftOffsets{};
	std::vector<sf::Vector2f> downRightOffsets{};

	std::vector<sf::Vector2f> leftSizes{};
	std::vector<sf::Vector2f> rightSizes{};
	std::vector<sf::Vector2f> downSizes{};
	std::vector<sf::Vector2f> upSizes{};
	std::vector<sf::Vector2f> upLeftSizes{};
	std::vector<sf::Vector2f> upRightSizes{};
	std::vector<sf::Vector2f> downLeftSizes{};
	std::vector<sf::Vector2f> downRightSizes{};

	std::vector<sf::Vector2f> leftBulletAnchors{};
	std::vector<sf::Vector2f> rightBulletAnchors{};
	std::vector<sf::Vector2f> downBulletAnchors{};
	std::vector<sf::Vector2f> upBulletAnchors{};
	std::vector<sf::Vector2f> upLeftBulletAnchors{};
	std::vector<sf::Vector2f> upRightBulletAnchors{};
	std::vector<sf::Vector2f> downLeftBulletAnchors{};
	std::vector<sf::Vector2f> downRightBulletAnchors{};

	AnimState previousState{ AnimState::Invariant };
	AnimState transientState{ AnimState::Invariant };
	AnimState currState{ AnimState::Invariant };
	AnimName name{ AnimName::Invariant };
	Cfg::Textures texID{ Cfg::Textures::Invariant };

	float frameDelay{ 0.f };
	float loopWaitDelay{ 0.f };
	bool loopWaits{ false };
	bool loops{true};
	bool playing{ true };

	sf::IntRect getFrame(AnimDir dir_, int index);
	sf::Vector2f getOffset(AnimDir dir_, int index);
	sf::Vector2f getWorldSize(AnimDir dir_, int index_);
	sf::Vector2f getBulletPoint(AnimDir dir_, int index_);
	int getNumFrames();

};


#endif
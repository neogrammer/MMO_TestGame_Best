#include "Animation.h"


std::unordered_map<std::string, AnimName> AnimNameLUT = {
    {"Idle",AnimName::Idle},
    {"Invariant",AnimName::Invariant},
    {"Running", AnimName::Running},
    {"Shooting", AnimName::Shooting},
    {"RunningAndShooting", AnimName::RunningAndShooting},
};
std::unordered_map < std::string, AnimDir> DirectionLUT = {
    {"Up",AnimDir::Up},
    {"Down",AnimDir::Down},
    {"DownLeft",AnimDir::DownLeft},
    {"DownRight",AnimDir::DownRight},
    {"Left",AnimDir::Left},
    {"Right",AnimDir::Right},
    {"UpLeft",AnimDir::UpLeft},
    {"UpRight",AnimDir::UpRight},
    {"Invariant",AnimDir::Invariant}
};
std::unordered_map < std::string, Cfg::Textures> TextureIDLUT = {
    {"PlayerAtlas",Cfg::Textures::PlayerAtlas},
    {"Stage_1_Tileset", Cfg::Textures::Stage_1_Tileset},
    {"Invariant", Cfg::Textures::Invariant}
};

std::unordered_map < Cfg::Textures, std::string> TexStringLUT = {
    {Cfg::Textures::PlayerAtlas,"PlayerAtlas"},
    { Cfg::Textures::Stage_1_Tileset,"Stage_1_Tileset"},
    {Cfg::Textures::Invariant, "Invariant" }
};

std::unordered_map < std::string, AnimSheetType> AnimSheetTypeLUT = {
    {"Blocks", AnimSheetType::Blocks},
    {"Normal", AnimSheetType::Normal},
    {"Padded", AnimSheetType::Padded},
    {"Vertical", AnimSheetType::Vertical}
};


std::unordered_map < AnimName, std::string > FSMStateNameLUT =
{
    {AnimName::Idle                       , "Idle"                          },
    {AnimName::Invariant                  , "Invariant"                     },
    {AnimName::Running                    , "Running"                       },
    {AnimName::Shooting                   , "Shooting"                      },
    {AnimName::RunningAndShooting                   , "RunningAndShooting"            },
};

std::unordered_map < std::string, AnimName > FSMAnimNameLUT =
{
   {"Idle",AnimName::Idle},
    {"Invariant",AnimName::Invariant},
    {"Running", AnimName::Running},
    {"Shooting", AnimName::Shooting},
    {"RunningAndShooting", AnimName::RunningAndShooting},
};



sf::IntRect Animation::getFrame(AnimDir dir_, int index)
{
    using a = AnimDir;
    switch (dir_)
    {
    case a::Right:
        return rightFrames.at(index);
        break;
    case a::Left:
        return leftFrames.at(index);
        break;
    case a::Up:
        return upFrames.at(index);
        break;
    case a::Down:
        return downFrames.at(index);
        break;
    case a::UpLeft:
        return upLeftFrames.at(index);
        break;
    case a::UpRight:
        return upRightFrames.at(index);
        break;
    case a::DownLeft:
        return downLeftFrames.at(index);
        break;
    case a::DownRight:
        return downRightFrames.at(index);
        break;
    default:
        break;
    }
    return sf::IntRect{ sf::Vector2i{ 0,0 }, sf::Vector2i{ 0, 0 } };
}

sf::Vector2f Animation::getOffset(AnimDir dir_, int index)
{
    using a = AnimDir;
    switch (dir_)
    {
    case a::Right:
        return rightOffsets.at(index);
        break;
    case a::Left:
        return leftOffsets.at(index);
        break;
    case a::Up:
        return upOffsets.at(index);
        break;
    case a::Down:
        return downOffsets.at(index);
        break;
    case a::UpLeft:
        return upLeftOffsets.at(index);
        break;
    case a::UpRight:
        return upRightOffsets.at(index);
        break;
    case a::DownLeft:
        return downLeftOffsets.at(index);
        break;
    case a::DownRight:
        return downRightOffsets.at(index);
        break;
    default:
        break;
    }
    return { 0.f,0.f };
}

sf::Vector2f Animation::getWorldSize(AnimDir dir_, int index_)
{
    using a = AnimDir;
    switch (dir_)
    {
    case a::Right:
        return rightSizes.at(index_);
        break;
    case a::Left:
        return leftSizes.at(index_);
        break;
    case a::Up:
        return upSizes.at(index_);
        break;
    case a::Down:
        return downSizes.at(index_);
        break;
    case a::UpRight:
        return upRightSizes.at(index_);
        break;
    case a::UpLeft:
        return upLeftSizes.at(index_);
        break;
    case a::DownLeft:
        return downLeftSizes.at(index_);
        break;
    case a::DownRight:
        return downRightSizes.at(index_);
        break;
    default:
        break;
    }
    return { 0.f,0.f };
}

sf::Vector2f Animation::getBulletPoint(AnimDir dir_, int index_)
{
    using a = AnimDir;
    switch (dir_)
    {
    case a::Right:
        return rightBulletAnchors.at(index_);
        break;
    case a::Left:
        return leftBulletAnchors.at(index_);
        break;
    case a::Down:
        return downBulletAnchors.at(index_);
        break;
    case a::Up:
        return upBulletAnchors.at(index_);
        break;
    case a::UpRight:
        return upRightBulletAnchors.at(index_);
        break;
    case a::UpLeft:
        return upLeftBulletAnchors.at(index_);
        break;
    case a::DownLeft:
        return downLeftBulletAnchors.at(index_);
        break;
    case a::DownRight:
        return downRightBulletAnchors.at(index_);
        break;
    default:
        break;
    }
    return { 0.f,0.f };
}

int Animation::getNumFrames()
{
    if (leftFrames.size() != 0)
    {
        return (int)leftFrames.size();
    }
    else if (rightFrames.size() != 0)
    {
        return (int)rightFrames.size();
    }
    else if (upFrames.size() != 0)
    {
        return (int)upFrames.size();
    }
    else if (downFrames.size() != 0)
    {
        return (int)downFrames.size();
    }
    else if (upLeftFrames.size() != 0)
    {
        return (int)upLeftFrames.size();
    }
    else if (upRightFrames.size() != 0)
    {
        return (int)upRightFrames.size();
    }
    else if (downLeftFrames.size() != 0)
    {
        return (int)downLeftFrames.size();
    }
    else if (downRightFrames.size() != 0)
    {
        return (int)downRightFrames.size();
    }
    else
        return 0;
}

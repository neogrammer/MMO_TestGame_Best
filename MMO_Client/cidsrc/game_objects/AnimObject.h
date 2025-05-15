#pragma once
#include "GameObject.h"
#include <handlers/AnimMgr.h>
class AnimObject : public GameObject
{
public:
	AnimMgr animMgr{};
public:
	using GameObject::GameObject;


	virtual ~AnimObject() override {}
	sf::Vector2f getWorldSize() override final;
	virtual void handleInput() override {}
	virtual void executeScript() override {}
	virtual void setTexRect(sf::IntRect tRect_);
	virtual void update(float dt_) override = 0;
	virtual void finalize(float dt_, sf::RenderWindow& wnd_) override = 0;
	virtual void getHit(int power) override {}
	std::string loadInFile(std::string animFile_);
	
};
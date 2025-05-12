#include "GameObject.h"
#include <iostream>
#include <game_objects/AnimObject.h>
GameObject::GameObject()
{
	texID = Cfg::Textures::Invariant;
	currTexRect = { {0,0},{64,64} };

	// define the 6 corners of the two triangles
	quad[0].position = sf::Vector2f(0.f, 0.f);
	quad[1].position = sf::Vector2f({ 64.f,0.f });
	quad[2].position = sf::Vector2f({ 0.f, 64.f });
	quad[3].position = sf::Vector2f({ 0.f,64.f });
	quad[4].position = sf::Vector2f({ 64.f, 0.f });
	quad[5].position = sf::Vector2f({ 64.f, 64.f });

	// define the 6 matching texture coordinates
	quad[0].texCoords = sf::Vector2f({ 0.f, 0.f });
	quad[1].texCoords = sf::Vector2f({ 64.f, 0.f });
	quad[2].texCoords = sf::Vector2f({0.f, 64.f });
	quad[3].texCoords = sf::Vector2f({ 0.f,64.f });
	quad[4].texCoords = sf::Vector2f({ 64.f, 0.f });
	quad[5].texCoords = sf::Vector2f({ 64.f, 64.f });

	for (int i = 0; i < 6; ++i)
	{
		quad[i].color = sf::Color::White;
	}

	worldPos = { 0.f,0.f };

}

GameObject::GameObject(Cfg::Textures texID_, sf::IntRect rect_, sf::Vector2f texOff_, sf::Vector2f size_, sf::Vector2f pos_)
{
	if (rect_ == sf::IntRect{{0, 0}, { 0,0 }})
	{
		rect_.size = (sf::Vector2i)Cfg::textures.get(texID_).getSize();
	}

	currTexRect = rect_;
	updateTexCoords();

	currOffset = texOff_;
	worldPos = pos_;

	if (size_ == sf::Vector2f{0.f, 0.f})
		currWorldSize = (sf::Vector2f)rect_.size;
	else
		currWorldSize = size_;

	// define the 6 corners of the two triangles
	quad[0].position = sf::Vector2f(worldPos - currOffset);
	quad[1].position = sf::Vector2f({ worldPos.x - currOffset.x + rect_.size.x, worldPos.y - currOffset.y });
	quad[2].position = sf::Vector2f({ worldPos.x - currOffset.x,worldPos.y - currOffset.y + rect_.size.y });
	quad[3].position = sf::Vector2f({ worldPos.x - currOffset.x,worldPos.y - currOffset.y + rect_.size.y });
	quad[4].position = sf::Vector2f({ worldPos.x - currOffset.x + rect_.size.x, worldPos.y - currOffset.y });
	quad[5].position = sf::Vector2f({ worldPos.x - currOffset.x + rect_.size.x, worldPos.y - currOffset.y + rect_.size.y });

	for (int i = 0; i < 6; ++i)
	{
		quad[i].color = sf::Color::White;
	}

	texID = texID_;
	this->setPosition(pos_ - texOff_);
}

GameObject::~GameObject()
{
}

GameObject::GameObject(const GameObject& o)
	: quad{ o.quad }
	, currOffset{ o.currOffset }, currTexRect{ o.currTexRect }, currWorldSize{ o.currWorldSize }
	, worldPos{ o.worldPos }, velocity{ o.velocity }, acceleration{o.acceleration}
	, alive{o.alive}, texID{o.texID}
{
	for (int i = 0; i < 6; ++i)
	{
		quad[i].color = sf::Color::White;
	}
}

GameObject& GameObject::operator=(const GameObject& o)
{
	this->quad = o.quad;

	for (int i = 0; i < 6; ++i)
	{
		quad[i].color = sf::Color::White;
	}

	currOffset = (o.currOffset);
	currTexRect =(o.currTexRect);
	currWorldSize = (o.currWorldSize);
	worldPos = (o.worldPos);
	velocity = (o.velocity);
	acceleration = (o.acceleration);
	alive = (o.alive);
	texID = (o.texID);

	return *this;
	
}

GameObject::GameObject(GameObject&& o)
	: quad{ std::move(o.quad) }
	, currOffset{ std::move(o.currOffset) }, currTexRect{ std::move(o.currTexRect) }, currWorldSize{ std::move(o.currWorldSize) }
	, worldPos{ std::move(o.worldPos) }, velocity{ std::move(o.velocity) }, acceleration{ std::move(o.acceleration) }
	, alive{ std::move(o.alive) }, texID{ std::move(o.texID) }
{
	for (int i = 0; i < 6; ++i)
	{
		quad[i].color = sf::Color::White;
	}
}

GameObject& GameObject::operator=(GameObject&& o)
{
	this->quad = std::move(o.quad);
	for (int i = 0; i < 6; ++i)
	{
		quad[i].color = sf::Color::White;
	}

	
	currOffset = std::move(o.currOffset);
	currTexRect = std::move(o.currTexRect);
	currWorldSize = std::move(o.currWorldSize);
	worldPos = std::move(o.worldPos);
	velocity = std::move(o.velocity);
	acceleration = std::move(o.acceleration);
	alive = std::move(o.alive);
	texID = std::move(o.texID);
	return *this;
}

void GameObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// apply the entity's transform -- combine it with the one that was passed by the caller
	states.transform *= getTransform(); // getTransform() is defined by sf::Transformable

	// apply the texture
	states.texture = &Cfg::textures.get(texID);
	if (states.texture->getSize().x == 0 || states.texture->getSize().y == 0) {
		std::cerr << "[ERROR] Texture for texID " << TexStringLUT[texID] << " is not loaded.\n";
	}

	// draw the vertex array
	target.draw(this->quad, states);
}

void GameObject::move(sf::Vector2f amt_)
{
	worldPos += amt_;

	updatePosition();
	updateTexCoords();
}

void GameObject::setPosition(sf::Vector2f pos_)
{
	worldPos = pos_;

	updatePosition();
	updateTexCoords();
}

sf::Vector2f GameObject::getPosition()
{
	return worldPos;
}

sf::Vector2f GameObject::getWorldSize()
{
	auto anO = dynamic_cast<AnimObject*>(this);
	if (anO != nullptr)
	{
		return anO->getWorldSize();
	}
	return currWorldSize;
}

void GameObject::setWorldSize(sf::Vector2f size_)
{
	currWorldSize = size_;
}

Cfg::Textures GameObject::getTexID()
{
	return texID;
}

void GameObject::setTexID(Cfg::Textures texID_)
{
	texID = texID_;
}

sf::Vector2f GameObject::getCurrOffset()
{
	return currOffset;
}

void GameObject::setTexRect(sf::IntRect rect_)
{
	currTexRect = rect_;
	updateTexCoords();
	updatePosition();
}

sf::Vector2f GameObject::getVelocity()
{
	return velocity;
}

void GameObject::setVelocity(sf::Vector2f vel_)
{
	velocity = vel_;
}

sf::FloatRect GameObject::getTestArea(float dt_)
{
	sf::FloatRect area;

	area.position.x = getPosition().x - (std::fabsf(velocity.x) * dt_);
	area.position.y = getPosition().y - (std::fabsf(velocity.y) * dt_);
	area.size.x = getWorldSize().x + (2 * std::fabsf(velocity.x) * dt_);
	area.size.y = getWorldSize().y + (2 * std::fabsf(velocity.y) * dt_);

	return area;
}

sf::IntRect GameObject::getTexRect()
{
	return currTexRect;
}

void GameObject::updateTexCoords()
{


	// define the 6 matching texture coordinates
	quad[0].texCoords = sf::Vector2f(currTexRect.position);
	quad[1].texCoords = sf::Vector2f({ (float)currTexRect.position.x + (float)currTexRect.size.x, (float)currTexRect.position.y });
	quad[2].texCoords = sf::Vector2f({(float)currTexRect.position.x, (float)currTexRect.position.y + (float)currTexRect.size.y });
	quad[3].texCoords = sf::Vector2f({ (float)currTexRect.position.x, (float)currTexRect.position.y + (float)currTexRect.size.y});
	quad[4].texCoords = sf::Vector2f({ (float)currTexRect.position.x + (float)currTexRect.size.x,(float)currTexRect.position.y });
	quad[5].texCoords = sf::Vector2f({ (float)currTexRect.position.x + (float)currTexRect.size.x,(float)currTexRect.position.y + (float)currTexRect.size.y });


}

void GameObject::updatePosition() 
{


	// define the 6 corners of the two triangles
	quad[0].position = sf::Vector2f(worldPos - currOffset);
	quad[1].position = sf::Vector2f({ (float)worldPos.x - (float)currOffset.x + (float)currTexRect.size.x, (float)worldPos.y - (float)currOffset.y });
	quad[2].position = sf::Vector2f({ (float)worldPos.x - (float)currOffset.x,(float)worldPos.y - (float)currOffset.y + (float)currTexRect.size.y });
	quad[3].position = sf::Vector2f({ (float)worldPos.x - (float)currOffset.x,(float)worldPos.y - (float)currOffset.y + (float)currTexRect.size.y });
	quad[4].position = sf::Vector2f({ (float)worldPos.x - (float)currOffset.x + (float)currTexRect.size.x, (float)worldPos.y - (float)currOffset.y });
	quad[5].position = sf::Vector2f({ (float)worldPos.x - (float)currOffset.x + (float)currTexRect.size.x, (float)worldPos.y - (float)currOffset.y + (float)currTexRect.size.y });


}

bool operator==(GameObject& o, const GameObject& b)
{
	if (&o.quad == &b.quad)
		return true;
	return false;
}

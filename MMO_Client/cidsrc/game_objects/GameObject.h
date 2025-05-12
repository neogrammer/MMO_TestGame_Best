#ifndef GAME_OBJECT_H__
#define GAME_OBJECT_H__
#include <SFML/Graphics.hpp>
#include <resources/Cfg.h>
#include <map>
#include <vector>

class AnimObject;
/// <summary>
/// GameObjects all have a representation in the game world.  First vertex in quad is the objects position in the world minus the tex offset
/// </summary>
class GameObject : public sf::Transformable, public sf::Drawable
{

protected:
	sf::VertexArray quad{sf::PrimitiveType::Triangles, 6};
	sf::Vector2f currWorldSize{ 64.f,64.f };
	sf::IntRect currTexRect{};
	sf::Vector2f currOffset{ };
	sf::Vector2f worldPos{ 0.f,0.f };
	sf::Vector2f velocity{ 0.f,0.f };
	sf::Vector2f acceleration{ 0.f,0.f };
	


	bool alive{ true };

	Cfg::Textures texID{ Cfg::Textures::Invariant };

	void updateTexCoords();
	void updatePosition();
public:
	GameObject();
	GameObject(Cfg::Textures texID_, sf::IntRect rect_ = { {0,0},{0,0} }, sf::Vector2f texOff_ = { 0.f,0.f }, sf::Vector2f size_ = {0.f,0.f}, sf::Vector2f pos_ = {0.f,0.f});

	virtual ~GameObject() = 0;
	GameObject(const GameObject& o);
	GameObject& operator=(const GameObject& o);
	GameObject(GameObject&& o);
	GameObject& operator=(GameObject&& o);

	friend bool operator==(GameObject& o, const GameObject& b);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	
	
	virtual void handleInput() {}
	virtual void executeScript() {}
	virtual void update(float dt_) = 0;
	virtual void finalize(float dt_, sf::RenderWindow& wnd_) = 0;

	virtual void getHit(int power) {}

	void move(sf::Vector2f amt_);
	void setPosition(sf::Vector2f pos_);
	sf::Vector2f getPosition();
	virtual sf::Vector2f getWorldSize();

	void setWorldSize(sf::Vector2f size_);
	Cfg::Textures getTexID();
	void setTexID(Cfg::Textures texID_);
	sf::Vector2f getCurrOffset();
	sf::IntRect getTexRect();
	void setTexRect(sf::IntRect rect_);
	sf::Vector2f getVelocity();
	void setVelocity(sf::Vector2f vel_);

	sf::FloatRect getTestArea(float dt_);
};

#endif
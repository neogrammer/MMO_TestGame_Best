#ifndef PROJECTILE_H__
#define PROJECTILE_H__
#include <game_objects/AnimObject.h>
//class Projectile : public AnimObject
//{
//	AnimObject* owner{ nullptr };
//	bool collided{ false };
//	GameObject* collider{ nullptr };
//	int power{ 1 };
//public:
//	using AnimObject::AnimObject;
//	Projectile() : AnimObject{} {}
//
//	Projectile(GameObject* owner_, Cfg::Textures texID_, sf::IntRect rect_ = { {0,0},{0,0} }, sf::Vector2f texOff_ = { 0.f,0.f }, sf::Vector2f size_ = { 0.f,0.f }, sf::Vector2f pos_ = { 0.f,0.f });
//	virtual ~Projectile() override;
//	virtual void update(float dt_) = 0;
//	virtual void render(sf::RenderWindow& wnd_) = 0;
//
//	bool hasCollided();
//	GameObject* getCollider();
//	void collide(GameObject* obj_);
//	int getPower();
//};

#endif
#ifndef INVARIANTOBJECT_H__
#define INVARIANTOBJECT_H__
#include "GameObject.h"
class InvariantObject : public GameObject
{
public:
	using GameObject::GameObject;
	~InvariantObject() override final;

	virtual void update(float dt_) override final;
	virtual void finalize(float dt_, sf::RenderWindow& wnd_) override final;

};

#endif
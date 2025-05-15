#ifndef PLAYER_H__
#define PLAYER_H__

#include "AnimObject.h"
//#include <animation/PlayerAnimFSM.h>
//#include <game_objects/BusterShot.h>
//#include <stack>
//#include <memory>
//#include <utility>
//
class Player : public AnimObject
{
//	friend class Physics;
//	friend class PlayState;
//	float bgLowBoundX{ 800.f };
//	float bgHighBoundX{ 7200.f };
//
//	float bgLowBoundY{ 450.f };
//	float bgHighBoundY{ 450.f };
//	bool waitingForJumpButton{ false };
//	bool rightPressed{ false };
//	bool leftPressed{ false };
//	bool landingJumpButtonHeld{ false };
//	bool jumpPressed{ false };
//	bool shootPressed{ false };
//	bool rightReleased{ false };
//	bool leftReleased{ false };
//	bool jumpReleased{ false };
//	bool shootReleased{ false };
//	const float MOVESPEED{ 300.f };
//
//	bool canJump{ false };
//	bool justJumped{ false };
//	bool falling{ true };
//	bool rising{ false };
//
//	bool reachedJumpPeak{ false };
//	float topOfJumpHoldTime{ 0.1f };
//	float topOfJumpHoldTimeElapsed{ 0.f };
//	const float JUMPSTRENGTH{ 800.f };
//	float currJumpHeight{ 0.f };
//	const float MAXJUMPHEIGHT{800.f};
//	float jumpHeightComparator{ 0.f };
//	float shootStopElapsed{ 0.f };
//	float shootStopDelay{ 0.35f };
//	const int MAXBULLETS{ 6 };
//	int currBullets{ 0 };
//
//	std::vector<std::shared_ptr<Projectile>> projectiles{ nullptr };
//
//	float shootCooldownTime{ 0.1f };
//	float shootCooldownElapsed{ 0.f };
//
//	bool shootCoolingDown{ false };
//
//	bool continueShooting{ false };
//	int maxBullets{ 5 };
//	bool makeBulletFixed{ false };
//public:
//
//	FSM_Player fsm;
//	
public:
	using AnimObject::AnimObject;
	Player(sf::Vector2f pos_ = { 300.f,300.f });
	virtual ~Player() override final;
//	void renderBullets(sf::RenderWindow& wnd_);
//	void shoot();
	void handleInput() override final;
	void update(float dt_) override final;
	void finalize(float dt_, sf::RenderWindow& wnd_) override final;
//
//	void setBGSize(float lx_, float hx_, float ly_, float hy_);
//
//	void jump();
//	void updateJump(float dt_);
//	virtual void getHit(int power) override;
//
//
//public:
//
};

#endif

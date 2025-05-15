#include "Player.h"
//#include <animation/DuckFold.h>
#include <iostream>
Player::Player(sf::Vector2f pos_)
	: AnimObject{ Cfg::Textures::PlayerAtlas,{{0,0}, { 148, 140 }},{0.f,0.f}, {148.f,140.f}, pos_ }
//	, projectiles{}
{
	setPosition(pos_);
	setTexRect({ {0,0},{148,140} });
	currWorldSize = {148.f,140.f};
//
	std::string pass = loadInFile("player.anim");

	if (pass != "OK")
	{
		std::cout << "\nPlayer Animation file not loaded in properly" << std::endl;
	}
	animMgr.switchAnim(AnimName::Idle, AnimDir::Down);
	setWorldSize({ 148.f,140.f });
//
//	projectiles.clear();
}
//
Player::~Player()
{
}
//
//void Player::renderBullets(sf::RenderWindow& wnd_)
//{
//	for (auto& b : projectiles)
//	{
//		wnd_.draw(*b);
//	}
//
//	std::vector<std::vector<std::shared_ptr<Projectile>>::iterator> itt{};
//
//	for (int i = 0; i < projectiles.size(); i++)
//	{
//		auto vw = wnd_.getView().getCenter().x;
//		if (projectiles[i]->getPosition().x < vw - wnd_.getSize().x / 2.f || projectiles[i]->getPosition().x > vw + wnd_.getSize().x / 2.f)
//		{
//
//			itt.push_back(projectiles.begin() + i);
//
//		}
//
//	}
//
//	for (auto start = itt.rbegin(); start != itt.rend(); start++)
//	{
//		projectiles.erase(*start);
//	}
//	projectiles.shrink_to_fit();
//
//
//	std::vector<std::vector<std::shared_ptr<Projectile>>::iterator>  ers;
//	for (int i = 0; i < projectiles.size(); i++)
//	{
//		if (projectiles[i]->hasCollided())
//		{
//			auto* collider = projectiles[i]->getCollider();
//
//			if (collider)
//			{
//				collider->getHit(projectiles[i]->getPower());
//
//			}
//
//			ers.push_back(projectiles.begin() + i);
//		}
//	}
//
//	for (auto start = ers.rbegin(); start != ers.rend(); start++)
//	{
//		projectiles.erase(*start);
//	}
//
//	projectiles.shrink_to_fit();
//
//}
//
void Player::handleInput()
{
//
//	// running logic
//
//	rightReleased = false;
//	leftReleased = false;
//	jumpReleased = false;
//	shootReleased = false;
//
//	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && rightPressed)
//	{
//		rightReleased = true;
//		if (!leftPressed)
//			dispatch(fsm, EventStoppedMoving{});
//	}
//
//
//	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && leftPressed)
//	{
//		leftReleased = true;
//		if (!rightPressed)
//			dispatch(fsm, EventStoppedMoving{});
//	}
//
//
//	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && jumpPressed)
//	{
//		jumpReleased = true;
//		dispatch(fsm, EventJumpReleased{});
//	}
//
//
//	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter) && shootPressed)
//	{
//		shootReleased = true;
//	}
//
//	jumpPressed = false;
//	shootPressed = false;
//	rightPressed = false;
//	leftPressed = false;
//
//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
//	{
//		rightPressed = true;
//		velocity.x = 300.f;
//		dispatch(fsm, EventMoved{});
//		animMgr.setCurrDir(AnimDir::Right);
//	}
//
//
//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
//	{
//		leftPressed = true;
//		velocity.x = -300.f;
//		dispatch(fsm, EventMoved{});
//		animMgr.setCurrDir(AnimDir::Left);
//
//
//	}
//
//
//	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
//	{
//		if (jumpPressed)
//		{
//
//			if (velocity.y < 0.f)
//			{
//
//				velocity.y *= 0.997f;
//			}
//			dispatch(fsm, EventJumpReleased{});
//			jumpReleased = true;
//		}
//
//	}
//
//	if (!jumpPressed)
//	{
//		if (velocity.y < 0.f)
//		{
//			velocity.y *= 0.997f;
//		}
//	}
//
//
//	// Jump logic
//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
//	{
//		jumpPressed = true;
//
//		if (canJump && animMgr.getCurrAnimName() != AnimName::Landing && animMgr.getCurrAnimName() != AnimName::Falling && animMgr.getCurrAnimName() != AnimName::LandingAndShooting && animMgr.getCurrAnimName() != AnimName::FallingAndShooting)
//		{
//			justJumped = true;
//			dispatch(fsm, EventJumped{});
//		}
//
//	}
//
//	if (animMgr.getCurrAnimName() == AnimName::LiftOff || animMgr.getCurrAnimName() == AnimName::LiftOffAndShooting)
//	{
//		if (animMgr.getCurrIndex() == 2)
//		{
//			dispatch(fsm, EventLiftOffTimedOut{});
//		}
//	}
//
//	if (animMgr.getCurrAnimName() == AnimName::Rising || animMgr.getCurrAnimName() == AnimName::RisingAndShooting)
//	{
//		if (std::fabsf(velocity.y) < 50.f)
//		{
//			dispatch(fsm, EventEnteredJumpPeak{});
//		}
//	}
//
//	if (animMgr.getCurrAnimName() == AnimName::JumpPeakRising || animMgr.getCurrAnimName() == AnimName::JumpPeakRisingAndShooting)
//	{
//		if (std::fabsf(velocity.y) < 0.5f)
//		{
//			dispatch(fsm, EventReachedJumpMax{});
//		}
//	}
//
//	if (animMgr.getCurrAnimName() == AnimName::JumpPeakFalling || animMgr.getCurrAnimName() == AnimName::JumpPeakFallingAndShooting)
//	{
//		if (std::fabsf(velocity.y) > 50.f)
//		{
//			dispatch(fsm, EventExitingJumpPeak{});
//		}
//	}
//	if (animMgr.getCurrAnimName() == AnimName::Landing || animMgr.getCurrAnimName() == AnimName::LandingAndShooting)
//	{
//		if (animMgr.getCurrIndex() == 2)
//		{
//			dispatch(fsm, EventFullyLanded{});
//		}
//	}
//
//
//	
//	shootPressed = false;
//	// Shoot logic
//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
//	{
//
//		shootPressed = true;
//
//		if (animMgr.getCurrAnimName() == AnimName::ShootSetup)
//		{
//			if (animMgr.getCurrIndex() == 1)
//			{
//				dispatch(fsm, EventShootSetupTimedOut{});
//				shoot();
//				shootCoolingDown = true;
//			}
//		}
//		else
//		{
//			dispatch(fsm, EventShoot{});
//			if (!shootCoolingDown)
//			{
//				shoot();
//				shootCoolingDown = true;
//			}
//		}
//
//
//
//	}
//
}
//
//void Player::shoot()
//{
//	 //for shooting at the correct location
//	if (projectiles.size() < maxBullets)
//	{
//		if (!shootCoolingDown)
//		{
//
//			makeBulletFixed = true;
//
//			//projectiles.push_back(std::make_shared<BusterShot>(this, Cfg::Textures::BusterShot, sf::IntRect{ sf::Vector2i{0,0},sf::Vector2i{24,14} }, sf::Vector2f{ 0.f,0.f }, sf::Vector2f{ 24.f,14.f }, sf::Vector2f{ getPosition().x + animMgr.getBulletPoint(AnimName::Shooting, animMgr.getCurrDir(), (int)animMgr.getCurrIndex()).x, getPosition().y + animMgr.getBulletPoint(AnimName::Shooting, animMgr.getCurrDir(), (int)animMgr.getCurrIndex()).y}));
//			////projectiles.back()->setPosition({ getPosition().x + (animMgr.getBulletPoint(AnimName::Shooting, animMgr.getCurrDir(), 0).x - getPosition().x),getPosition().y + (animMgr.getBulletPoint(AnimName::Shooting, animMgr.getCurrDir(), 0).y - getPosition().y) });
//			//if (animMgr.getCurrDir() == AnimDir::Right)
//			//{
//			//	projectiles.back()->setVelocity({ 700.f, 0.f });
//			//}
//			//else
//			//{
//			//	projectiles.back()->setVelocity({ -700.f,0.f });
//			//}
//
//			shootCoolingDown = true;
//			shootCooldownElapsed = 0.f;
//
//
//		}
//	}
//
//}
//
void Player::update(float dt_)
{
//	
//
//	if (shootCoolingDown)
//	{
//		shootCooldownElapsed += dt_;
//
//		if (shootCooldownElapsed >= shootCooldownTime)
//		{
//			shootCooldownElapsed = 0;
//
//			shootCoolingDown = false;
//		}
//	}
//
//	if (justJumped)
//	{
//		velocity.y = -1150.f;
//		worldPos.y -= 1.f;
//		justJumped = false;
//		canJump = false;
//		dispatch(fsm, EventJumped{});
//	}
//	
//	if (velocity.y > 0.f)
//	{
//		if (animMgr.getCurrAnimName() == AnimName::JumpPeakRising ||
//			animMgr.getCurrAnimName() == AnimName::JumpPeakRisingAndShooting ||
//			animMgr.getCurrAnimName() == AnimName::JumpPeakFalling ||
//			animMgr.getCurrAnimName() == AnimName::JumpPeakFallingAndShooting)
//		{
//			dispatch(fsm, EventFell{});
//		}
//	}
//
//	static bool flag{ false };
//	if (shootReleased)
//	{
//		flag = true;
//		shootStopElapsed = 0.f;
//
//		if (animMgr.getCurrAnimName() == AnimName::ShootSetup)
//		{
//			flag = true;
//			dispatch(fsm, EventShootSetupTimedOut{});
//
//		}
//
//	}
//
//	if (flag)
//	{
//		shootStopElapsed += dt_;
//		if (shootStopElapsed >= shootStopDelay)
//		{
//			flag = false;
//			dispatch(fsm, EventStoppedShooting{});
//		}
//	}
//
//	/*else
//	{
//		if (!shootPressed)
//		{
//			shootStopElapsed += dt_;
//				if (shootStopElapsed >= shootStopDelay)
//				{
//					flag = true;
//					shootStopElapsed = 0.f;
//				}
//			}
//		}
//		
//	}*/
//
//
////	auto r = animMgr.currFrame();
//
//	//}
//
}
//
void Player::finalize(float dt_, sf::RenderWindow& wnd_)
{
	//
	//	if (fsm.getStateName() == "RisingAndShooting" && velocity.y > 0.f)
	//	{
	//		dispatch(fsm, EventFell{});
	//
	//
	//	}
	//
	//	// checks if the direction or animation needs to change then update the animation for either one changing, changing the direction first and using that in the anim switch
	//
	//	bool playerFacingRight{};
	//	bool playerFacingRightAfter{};
	//auto nameBefore = animMgr.getCurrAnimName();
	//auto dirBefore = animMgr.getCurrDir();
	//	if (dirBefore == AnimDir::Right)
	//		playerFacingRight = true;
	//	else if (dirBefore == AnimDir::Left)
	//		playerFacingRight = false;
	//AnimDir dirAfter{ dirBefore };
	//
	//	if (dirAfter == AnimDir::Right)
	//		playerFacingRightAfter = true;
	//	else if (dirAfter == AnimDir::Left)
	//		playerFacingRightAfter = false;
	//
	//auto nameAfter = animMgr.getCurrAnimName();
	//
	//
	//	if (playerFacingRight != playerFacingRightAfter)
	//	{
	//		if (playerFacingRightAfter == true)
	//		{
	//			animMgr.setCurrDir(AnimDir::Right);
	//		}
	//		else
	//		{
	//			animMgr.setCurrDir(AnimDir::Left);
	//		}
	//	}
	//
	//auto finalDir = animMgr.getCurrDir();

	//if (FSMStateNameLUT.find(nameBefore) != FSMStateNameLUT.end() && FSMStateNameLUT.find(nameAfter) != FSMStateNameLUT.end())
	//{

		//if (nameBefore != nameAfter || finalDir != dirBefore)
	//	{
	//		animMgr.switchAnim(nameAfter, finalDir);
	//		setTexID(animMgr.getTexID());
	//		animMgr.reset();
	//	}
	animMgr.animate(dt_);
	setTexID(Cfg::Textures::PlayerAtlas);
	setTexRect(animMgr.getTexRect());
	
}

void Player::render(sf::RenderWindow& wnd_)
{

	auto tmp = getPosition();
	currOffset = { 36.f, 30.f };
	setPosition(tmp - currOffset);
	wnd_.draw(*this);
	setPosition(tmp);

}

	//
	//
	//
	//
	//
	//	// now check position and velocity of player and move the screen instead when in middle of it moving in a direction until the end of the level then move the player
	//	auto vw = wnd_.getView();
	//	if (rightPressed)
	//	{
	//		//velocity.x = 300.f;
	//		// if player is going to be still on the left side of screen after moving, then move the player
	//		if (getPosition().x + velocity.x * dt_ < bgLowBoundX || vw.getCenter().x >= bgHighBoundX)
	//		{
	//			move({ velocity.x * dt_, 0.f });
	//		}
	//		else // move the player to the center, move the view instead by (playerPosition if moved - view.getCenter) * dt_
	//		{
	//			vw.move({ (getPosition().x + velocity.x * dt_) - vw.getCenter().x, 0.f });
	//			wnd_.setView(vw);
	//			setPosition({ wnd_.mapPixelToCoords({(int)bgLowBoundX,(int)bgLowBoundY}).x, getPosition().y});
	//		}
	//	}
	//	else if (leftPressed)
	//	{
	//		//velocity.x = -300.f;
	//		// if player is going to be still on the left side of screen after moving, then move the player
	//		if (getPosition().x + velocity.x * dt_ > bgHighBoundX || vw.getCenter().x <= bgLowBoundX)
	//		{
	//			move({ velocity.x * dt_, 0.f });
	//		}
	//		else // move the player to the center, move the view instead by (playerPosition if moved - view.getCenter) * dt_
	//		{
	//			vw.move({ (getPosition().x + velocity.x * dt_) - (vw.getCenter().x) , 0.f });
	//			wnd_.setView(vw);
	//			setPosition({ wnd_.mapPixelToCoords({(int)bgLowBoundX,(int)bgLowBoundY}).x, getPosition().y });
	//		}
	//	}
	//
	//	// we set the x above with the screen movement, not adjust the player for the y value
	//	// apply y velocity
	//	setPosition({ getPosition().x, getPosition().y + (getVelocity().y * dt_) });
	//
	//	// Now make a nbullet if needed
	//	if (makeBulletFixed)
	//	{
	//		makeBulletFixed = false;
	//		projectiles.push_back(std::make_shared<BusterShot>(this, Cfg::Textures::BusterShot, sf::IntRect{ sf::Vector2i{0,0},sf::Vector2i{24,14} }, sf::Vector2f{ 0.f,0.f }, sf::Vector2f{ 24.f,14.f }, sf::Vector2f{ getPosition().x + animMgr.getBulletPoint(AnimName::Shooting, animMgr.getCurrDir(), (int)animMgr.getCurrIndex()).x, getPosition().y + animMgr.getBulletPoint(AnimName::Shooting, animMgr.getCurrDir(), (int)animMgr.getCurrIndex()).y }));
	//		//projectiles.back()->setPosition({ getPosition().x + (animMgr.getBulletPoint(AnimName::Shooting, animMgr.getCurrDir(), 0).x - getPosition().x),getPosition().y + (animMgr.getBulletPoint(AnimName::Shooting, animMgr.getCurrDir(), 0).y - getPosition().y) });
	//		if (animMgr.getCurrDir() == AnimDir::Right)
	//		{
	//			projectiles.back()->setVelocity({ 700.f, 0.f });
	//		}
	//		else
	//		{
	//			projectiles.back()->setVelocity({ -700.f,0.f });
	//		}
	//		std::cout << "PlayerPosition: x= " << getPosition().x << ", y= " << getPosition().y << "\n>>>  TexOffset: x= " << getCurrOffset().x << ", y= " << getCurrOffset().y << "\n>>> AnchorPointRelative: x= " << animMgr.getBulletPoint(AnimName::Shooting, animMgr.getCurrDir(), 0).x << ", y= " << animMgr.getBulletPoint(AnimName::Shooting, animMgr.getCurrDir(), 0).y << " \nFinalPositionBullet: x= " << projectiles.back()->getPosition().x << ", y= " << projectiles.back()->getPosition().y << std::endl;
	//
	//
	//	}
	//
	//
	// 

	//	// now that our position is finalized, lets animate the frame then update the texture rect
	//const auto& r = animMgr.currFrame();
	//animMgr.animate(dt_);
	//if (r != animMgr.currFrame())
	//{
	//	// frame changed
	//	setTexRect(animMgr.currFrame());
	//}
//}


//void Player::setBGSize(float lx_, float hx_, float ly_, float hy_)
//{
//	bgLowBoundX = lx_;
//	bgLowBoundY = ly_;
//	bgHighBoundX = hx_;
//	bgHighBoundY = hy_;
//
//
//}
//
//void Player::jump()
//{
//	if (canJump)
//	{
//		justJumped = true;
//		canJump = false;
//		velocity.y -= JUMPSTRENGTH;  // up is down the y axis, so subtract the jump power to send play upward
//		falling = false;
//		rising = true;
//
//		jumpHeightComparator = getPosition().y;
//		currJumpHeight = jumpHeightComparator;
//	}
//
//}
//
//void Player::updateJump(float dt_)
//{
//	if (rising)
//	{
//		currJumpHeight += velocity.y * dt_;
//		if (jumpHeightComparator - currJumpHeight > MAXJUMPHEIGHT) // beacuse up goes down the y axis, and you jump upward
//		{
//			reachedJumpPeak = true;
//			
//			topOfJumpHoldTimeElapsed = 0.f;
//			rising = false;
//			falling = false;
//			setVelocity({ getVelocity().x, 0.f });
//		}
//	}
//	else if (reachedJumpPeak)
//	{
//		topOfJumpHoldTimeElapsed += dt_;
//		setVelocity({ getVelocity().x, 0.f });
//
//		if (topOfJumpHoldTimeElapsed >= topOfJumpHoldTime)
//		{
//			topOfJumpHoldTimeElapsed = 0.f;
//			reachedJumpPeak = false;
//			falling = true;
//		}
//	}
//	else if (falling)
//	{
//		// if the collision detection changed the canJump flag, we are no longer falling
//		if (canJump)
//		{
//			falling = false;
//		}
//	}
//}
//
//void Player::getHit(int power)
//{
//}
//

#include "../MMO_Server/MMO_Common.h"

#include <SFML/Graphics.hpp>

#include <map>
#include <net_client.h>
#include <net_message.h>

bool isMyWindowInFocus(sf::RenderWindow& wnd_)
{
	HWND tvhandle{};

	if (wnd_.isOpen())
	{
		tvhandle = (HWND)wnd_.getNativeHandle();
	}

	bool one = tvhandle == GetFocus();
	bool two = tvhandle == GetForegroundWindow();

	if (one && two)
	{
		// our window is on top and in focus
		return true;
	}
	else
	{
		return false;
	}
}

class MMOGame : cnet::client_interface<GameMsg>
{
	bool isFocused = true;
	sf::RenderWindow tv;
	sf::Vector2i initialLoc = { 0,0 };
	sf::Vector2i panStart = { 0,0 };
	sf::Font fnt{ "assets/font1.ttf" };
	sf::Texture playerTex{ "assets/textures/players/playerSheet.png" };
	const float BASE_SPEED = 2.f; // 120 pixels per second at normal zoom (1.f)
	sf::Vector2f zoomFactor{ 1.f,1.f };
	int tileSize{ 64 };
	bool hasDropped{ true };

	std::map<uint32_t, sPlayerDescription> mapObjects;
	std::map<AnimDir, std::vector<sf::IntRect>> walkAnim
	{
		{AnimDir::N, std::vector<sf::IntRect>{ {{0*74,0*70},{74,70}}, {{1*74,0*70},{74,70}}, {{2*74,0*70},{74,70}}, {{3*74,0*70},{74,70} } }},
		{AnimDir::NE, std::vector<sf::IntRect>{ {{0 * 74,1 * 70},{74,70}}, {{1 * 74,1 * 70},{74,70}}, {{2 * 74,1 * 70},{74,70}}, {{3 * 74,1 * 70},{74,70} } }},
		{AnimDir::E, std::vector<sf::IntRect>{ {{0 * 74,2 * 70},{74,70}}, {{1 * 74,2 * 70},{74,70}}, {{2 * 74,2 * 70},{74,70}}, {{3 * 74,2 * 70},{74,70} } }},
		{AnimDir::SE, std::vector<sf::IntRect>{ {{0 * 74,3 * 70},{74,70}}, {{1 * 74,3 * 70},{74,70}}, {{2 * 74,3 * 70},{74,70}}, {{3 * 74,3 * 70},{74,70} } }},
		{AnimDir::S, std::vector<sf::IntRect>{ {{0 * 74,4 * 70},{74,70}}, {{1 * 74,4 * 70},{74,70}}, {{2 * 74,4 * 70},{74,70}}, {{3 * 74,4 * 70},{74,70} } }},
		{AnimDir::SW, std::vector<sf::IntRect>{ {{0 * 74,5 * 70},{74,70}}, {{1 * 74,5 * 70},{74,70}}, {{2 * 74,5 * 70},{74,70}}, {{3 * 74,5 * 70},{74,70} } }},
		{AnimDir::W, std::vector<sf::IntRect>{ {{0 * 74,6 * 70},{74,70}}, {{1 * 74,6 * 70},{74,70}}, {{2 * 74,6 * 70},{74,70}}, {{3 * 74,6 * 70},{74,70} } }},
		{AnimDir::NW, std::vector<sf::IntRect>{ {{0 * 74,7 * 70},{74,70}}, {{1 * 74,7 * 70},{74,70}}, {{2 * 74,7 * 70},{74,70}}, {{3 * 74,7 * 70},{74,70} } }}
	};
	float animDelay{ 0.13f };
	bool playing{ false };
	uint32_t currentFrame = 0;
	AnimDir currentDir = AnimDir::S;

	void startPan(sf::RenderWindow& wnd_, sf::Vector2i startPos_)
	{
		if (hasDropped)
		{
			tv.setMouseCursorVisible(false);
			sf::Mouse::setPosition({ (sf::Vector2i)(sf::Vector2f{(float)tv.getSize().x / 2.f, (float)tv.getSize().y / 2.f}) }, wnd_);
			

			initialLoc = startPos_;
			panStart = (sf::Vector2i)(sf::Vector2f{ (float)tv.getSize().x / 2.f, (float)tv.getSize().y / 2.f });
			hasDropped = false;
			isPanning = true;
		}
		else
		{
			// set mouse to invisible and store the location
			tv.setMouseCursorVisible(false);
			initialLoc = startPos_;

		}
	}

	void updatePan(sf::RenderWindow& wnd_, sf::Vector2i mousePos_, float dt_)
	{
		float dispX = 0.f;
		float dispY = 0.f;
		
		sf::Vector2f mpos = (sf::Vector2f)sf::Mouse::getPosition(wnd_);
		auto disp = (sf::Vector2f)panStart - mpos;
		if (disp != sf::Vector2f{ 0.f,0.f })
		{
			disp = {disp.normalized().x * 1600.f, disp.normalized().y * 900.f};
		}
		sf::View vw = wnd_.getView();
		vw.move({disp.x * dt_, disp.y * dt_});
		wnd_.setView(vw);

		sf::Mouse::setPosition(panStart, wnd_);
		isPanning = true;
	}

	void dropPan(sf::RenderWindow& wnd_, sf::Vector2i mousePos_)
	{
		if (hasDropped == false)
			sf::Mouse::setPosition(initialLoc, tv);
		tv.setMouseCursorVisible(true);
		hasDropped = true;

	}

	
	int minimum(int a, int b)
	{
		if (a < b)
		{
			return a;
		}
		else
		{
			if (a > b)
			{
				return b;
			}
			else
			{
				return a;
			}
		}
	}


	int maximum(int a, int b)
	{
		if (a > b)
		{
			return a;
		}
		else
		{
			if (a < b)
			{
				return b;
			}
			else
			{
				return a;
			}
		}
	}

	int getTopTile()
	{
		auto vw = tv.getView();
		return (int)(maximum(0, ((int)vw.getCenter().y - (int)tv.getSize().y / 2) / (int)tileSize));

	}

	int getLeftTile()
	{
		auto vw = tv.getView();
		return (int)(maximum(0, ((int)vw.getCenter().x - (int)tv.getSize().x / 2) / (int)tileSize));
	}

	int getBottomTile()
	{
		auto vw = tv.getView();
		return (int)(minimum(14, ((int)vw.getCenter().y + (int)tv.getSize().y / 2) / (int)tileSize));
	}

	int getRightTile()
	{
		auto vw = tv.getView();
		return (int)(minimum(24, ((int)vw.getCenter().x + (int)tv.getSize().x / 2) / (int)tileSize));
	}

	struct WorldObject
	{
		uint32_t ownerID{ 10000 };
		sf::Vector2f pos{ 0.f,0.f };
		sf::Vector2f vel{ 0.f,0.f };
		float fRad{ 0.5f };
	};

	enum class Direction
	{
		N, NE, E, SE, S, SW, W, NW
	};

	WorldObject object;
	Direction currDir;

	std::map<uint32_t, std::vector<WorldObject>> projectiles;

public:
	MMOGame()
		: tv{ sf::VideoMode({1600U, 900U},32U), "MMO CLIENT", sf::State::Windowed }
		, sAppName{ "MMO Client" }
	{
		projectiles.clear();

		object.ownerID = nPlayerID;

		currDir = Direction::S;
	}

private:
	
	std::string sWorldMap =
		"################################"
		"#..............................#"
		"#..............................#"
		"#..............................#"
		"#..............................#"
		"#..............................#"
		"#..............................#"
		"#..............................#"
		"#..............................#"
		"#..............................#"
		"#..............................#"
		"#..............................#"
		"#..........####...####.........#"
		"#..........#.........#.........#"
		"#..........#.........#.........#"
		"#..........#.........#.........#"
		"#..........##############......#"
		"#..............................#"
		"#..................#.#.#.#.....#"
		"#..............................#"
		"#..................#.#.#.#.....#"
		"#..............................#"
		"#..............................#"
		"#..............................#"
		"#..............................#"
		"#..............................#"
		"#..............................#"
		"#..............................#"
		"#..............................#"
		"#..............................#"
		"#..............................#"
		"################################";

	sf::Vector2i vWorldSize = { 32, 32 };

private:
	uint32_t nPlayerID = 0;
	sPlayerDescription descPlayer;
	const char* sAppName;
	bool bWaitingForConnection = true;

	bool isPanning{ false };

	bool leftMouseButtonReleased{ false };
	bool leftMouseButtonPressed{ false };
	bool leftMouseButtonHeld{ false };

	const float speed = BASE_SPEED * (1.f / zoomFactor.x);
	bool isMoving = false;


	int vAreaTopThis{};
	int vAreaLeftThis{};
	int vAreaBttmThis{};
	int vAreaRightThis{};

public:

	bool Init()
	{
		sf::ContextSettings settings;
		settings.antiAliasingLevel = 8;
		settings.attributeFlags = sf::ContextSettings::Attribute::Core;

		tv.create( sf::VideoMode({1600U, 900U},32U), "MMO CLIENT", sf::State::Windowed , settings);
		if (Connect("192.168.0.6", 60000))
		{
			object.pos = { 3.f,3.f };
			return true;
		}

		return false;
	}

	bool handleNewClientMessages()
	{
		bool waiting = true;

		while (waiting)
		{


			while (!Incoming().empty())
			{
				auto msg = Incoming().pop_front().msg;

				switch (msg.header.id)
				{
				case GameMsg::Server_GetPing:
				{

				}
				break;
				case(GameMsg::Client_Accepted):
				{
					std::cout << "Server accepted client - you're in!\n";
					cnet::message<GameMsg> msg;
					msg.header.id = GameMsg::Client_RegisterWithServer;
					descPlayer.vPos = { 3.0f,3.0f };
					msg << descPlayer;
					Send(msg);
					break;
				}

				case(GameMsg::Client_AssignID):
				{
					// Server is assigning us OUR id
					msg >> nPlayerID;
					std::cout << "Assigned Client ID = " << nPlayerID << "\n";
					mapObjects[nPlayerID] = sPlayerDescription{};
					mapObjects[nPlayerID].nUniqueID = nPlayerID;
					mapObjects[nPlayerID].fRadius = 32.f;
					mapObjects[nPlayerID].vPos = { 3.0f,3.0f };

					// CRUCIAL STEP: update the WorldObject's ownerID here
					object.ownerID = nPlayerID;


					projectiles.emplace(nPlayerID, std::vector<WorldObject>{});
					projectiles[nPlayerID].clear();


					break;
				}

				case GameMsg::Server_GetOwnTime:
				{

				}
				break;
				case(GameMsg::Game_AddPlayer):
				{
					sPlayerDescription desc;
					msg >> desc;

					mapObjects.insert_or_assign(desc.nUniqueID, desc);
					projectiles.insert_or_assign(desc.nUniqueID, std::vector<WorldObject>{});
					projectiles[desc.nUniqueID].clear();
				

					// Special handling for your own player:
					if (desc.nUniqueID == nPlayerID)
					{
						object.ownerID = nPlayerID; // Just ensure it's correct again
						object.pos = desc.vPos;
						auto vw = tv.getView();
						vw.setCenter(object.pos * (float)tileSize);
						tv.setView(vw);
						bWaitingForConnection = false;
					}

					break;
				}

				}
			}


			if (bWaitingForConnection)
			{
				tv.clear(sf::Color(47, 147, 247, 255));
				std::cout << "\n" << "Waiting To Connect..." << std::endl;
				waiting = true;
			}
			else
			{
				waiting = false;
			}


		}

		return waiting;

	}

	void handleCoreLoopMessages()
	{
		while (!Incoming().empty())
		{
			auto msg = Incoming().pop_front().msg;

			switch (msg.header.id)
			{

			case(GameMsg::Game_AddBullet):
			{
				BulletDescription desc;
				msg >> desc;
				if (projectiles.find(desc.nUniqueID) == projectiles.end())
				{
					projectiles[desc.nUniqueID] = std::vector<WorldObject>{};
					projectiles[desc.nUniqueID].clear();
				}
				projectiles[desc.nUniqueID].emplace_back(WorldObject{});
				projectiles[desc.nUniqueID][projectiles[desc.nUniqueID].size() - 1].pos = desc.pos;
				projectiles[desc.nUniqueID][projectiles[desc.nUniqueID].size() - 1].vel = desc.vel;
				projectiles[desc.nUniqueID][projectiles[desc.nUniqueID].size() - 1].fRad = desc.fRad;
				projectiles[desc.nUniqueID][projectiles[desc.nUniqueID].size() - 1].ownerID = desc.nUniqueID;

				break;
			}

			case(GameMsg::Game_RemoveBullet):
			{
				BulletDescription desc;
				msg >> desc;
				projectiles[desc.nUniqueID].erase(projectiles[desc.nUniqueID].begin() + (desc.index));
				break;
			}

			case(GameMsg::Game_UpdateBullet):
			{

				break;
			}




			case(GameMsg::Game_AddPlayer):
			{
				sPlayerDescription desc;
				msg >> desc;

				mapObjects.insert_or_assign(desc.nUniqueID, desc);
				projectiles.insert_or_assign(desc.nUniqueID, std::vector<WorldObject>{});
				projectiles[desc.nUniqueID].clear();
				
				// Special handling for your own player:
				if (desc.nUniqueID == nPlayerID)
				{
					object.ownerID = nPlayerID; // Just ensure it's correct again
					object.pos = desc.vPos;
					bWaitingForConnection = false;
				}

				break;
			}

			case(GameMsg::Game_RemovePlayer):
			{
				uint32_t nRemovalID = 0;
				msg >> nRemovalID;
				mapObjects.erase(nRemovalID);
				projectiles.erase(nRemovalID);
				break;
			}

			case(GameMsg::Game_UpdatePlayer):
			{
				sPlayerDescription desc;
				msg >> desc;
				mapObjects.insert_or_assign(desc.nUniqueID, desc);
				break;
			}
			}
		}
	}

	void handleMyStaticInput(float dt)
	{
		object.vel = { 0.f,0.f };

		isMoving = false;

		if (isFocused)
		{
			if (!isPanning && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
			{
				isMoving = true;
				currDir = Direction::NW;
				object.vel = { -speed * 0.7071f * dt, -speed * 0.7071f * dt };
			}
			else if (!isPanning && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
			{
				isMoving = true;
				currDir = Direction::NE;
				object.vel = { speed * 0.7071f * dt, -speed * 0.7071f * dt };
			}
			else if (!isPanning && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
			{
				isMoving = true;
				currDir = Direction::SW;
				object.vel = { -speed * 0.7071f * dt, speed * 0.7071f * dt };
			}
			else if (!isPanning && sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D))
			{
				isMoving = true;
				currDir = Direction::SE;
				object.vel = { speed * 0.7071f * dt, speed * 0.7071f * dt };
			}
			else
			{
				if (!isPanning && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
				{
					object.vel.y = -speed * dt;
					isMoving = true;
					currDir = Direction::N;
				}
				else if (!isPanning && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
				{
					isMoving = true;
					object.vel.y = speed * dt;
					currDir = Direction::S;
				}
				if (!isPanning && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
				{
					isMoving = true;
					object.vel.x = -speed * dt;
					currDir = Direction::W;
				}
				else if (!isPanning && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
				{
					isMoving = true;
					object.vel.x = speed * dt;
					currDir = Direction::E;
				}
			}



			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Space))
			{
				const float BSPD{ 20.f };
				auto& p = projectiles[nPlayerID];
				p.emplace_back(WorldObject{});
				p[p.size() - 1].ownerID = nPlayerID;
				p[p.size() - 1].pos = object.pos;
				p[p.size() - 1].vel = { 0.f,0.f };
				p[p.size() - 1].fRad = 1.f;

				switch (currDir) {
				case Direction::N:
				{
					p[p.size() - 1].vel = sf::Vector2f{ 0.f,-1.f * BSPD };
				}
				break;
				case Direction::NE:
				{
					p[p.size() - 1].vel = sf::Vector2f{ .7071f * BSPD,-.7071f * BSPD };
				}
				break;
				case Direction::E:
				{
					p[p.size() - 1].vel = sf::Vector2f{ 1.f * BSPD,0.f };
				}
				break;
				case Direction::SE:
				{
					p[p.size() - 1].vel = sf::Vector2f{ .7071f * BSPD,.7071f * BSPD };
				}
				break;
				case Direction::S:
				{
					p[p.size() - 1].vel = sf::Vector2f{ 0.f,1.f * BSPD };
				}
				break;
				case Direction::SW:
				{
					p[p.size() - 1].vel = sf::Vector2f{ -.7071f * BSPD,.7071f * BSPD };
				}
				break;
				case Direction::W:
				{
					p[p.size() - 1].vel = sf::Vector2f{ -1.f * BSPD,0.f };
				}
				break;
				case Direction::NW:
				{
					p[p.size() - 1].vel = sf::Vector2f{ -.7071f * BSPD,-.7071f * BSPD };
				}
				break;
				default:
				{
				}
				break;
				}

				cnet::message<GameMsg> msgAddBullet;
				msgAddBullet.header.id = GameMsg::Game_AddBullet;
				BulletDescription bd{};
				bd.fRad = p[p.size() - 1].fRad;
				bd.nUniqueID = p[p.size() - 1].ownerID;
				bd.pos = p[p.size() - 1].pos;
				bd.vel = p[p.size() - 1].vel;
				bd.index = (uint32_t)(p.size() - 1);
				msgAddBullet << bd;

				Send(msgAddBullet);

			}
		}
	}


	void updateAndCollidePlayers(float dt)
	{
		for (auto& player : mapObjects)
		{
			// update the player on this client to match the updated velocity vals from static input handler
			if (player.first == object.ownerID)
			{
				player.second.vPos = object.pos;
				player.second.vVel = object.vel;
			}
			sf::Vector2f vPotentialPosition = player.second.vPos + (player.second.vVel * ((player.first == object.ownerID) ? 1 : dt));
			sf::Vector2i vCurrentCell = { (int)(floorf(player.second.vPos.x)), (int)(floorf(player.second.vPos.y)) };
			sf::Vector2i vTargetCell = sf::Vector2i({ (int)vPotentialPosition.x, (int)vPotentialPosition.y });
			int vAreaTop = maximum(((int)(minimum(vCurrentCell.y, vTargetCell.y) - 1)), (int)0);
			int vAreaLeft = maximum(((int)(minimum(vCurrentCell.x, vTargetCell.x) - 1)), (int)0);
			int vAreaBttm = minimum((maximum(vCurrentCell.y, vTargetCell.y) + 3), (vWorldSize.y - 1));
			int vAreaRight = minimum((maximum(vCurrentCell.x, vTargetCell.x) + 3), (vWorldSize.x - 1));
			//	// Iterate through each cell in test area
			sf::Vector2i vCell;
			for (vCell.y = vAreaTop; vCell.y <= vAreaBttm; vCell.y++)
			{
				for (vCell.x = vAreaLeft; vCell.x <= vAreaRight; vCell.x++)
				{
					// Check if the cell is actually solid...
					if (sWorldMap[vCell.y * vWorldSize.x + vCell.x] == '#')
					{
						sf::Vector2f vNearestPoint;
						vNearestPoint.x = (float)std::max(float(vCell.x), std::min((vPotentialPosition.x), float(vCell.x + 1)));
						vNearestPoint.y = (float)std::max(float(vCell.y), std::min((vPotentialPosition.y), float(vCell.y + 1)));
						sf::Vector2f vRayToNearest = { vNearestPoint.x - (vPotentialPosition.x), vNearestPoint.y - (vPotentialPosition.y) };
						float fOverlap = (player.second.fRadius - vRayToNearest.length());
						if (std::isnan(fOverlap)) fOverlap = 0;
						if (fOverlap > 0)
						{
							// Statically resolve the collision
							sf::Vector2f tmp = (vRayToNearest == sf::Vector2f{ 0.f, 0.f }) ? sf::Vector2f{ 0.f, 0.f } : vRayToNearest.normalized();
							vPotentialPosition = vPotentialPosition - (tmp * fOverlap);
						}
					}
				}
			}
			// now set to correct position after resolution
			player.second.vPos = vPotentialPosition;

			// set new thisClient positioning to the object struct depicting this client player object
			if (player.first == nPlayerID)
			{
				object.pos = player.second.vPos;
				object.vel = player.second.vVel;

				// only fix the pan if the user is no longer panning and starts moving
				if (!isPanning && isMoving)
				{
					auto vw = tv.getView();
					vw.setCenter({ (object.pos * (float)tileSize) });// /*  - off.x */, vw.getCenter().y
					tv.setView(vw);
				}
			}
		}
	}
	
	void updateAndCollideProjectiles(float dt)
	{

		// now update the bullets locally and check for collision, then send this players updated bullets to the server in a loop which will update all the players with all those bullets, and the other players will do the same
		for (auto& playerBullets : projectiles)
		{
			for (auto& bullet : playerBullets.second)
			{
				sf::Vector2f bulletPoss = bullet.pos + (bullet.vel * dt);

				sf::Vector2i vCurrentCell = { (int)(floorf(bullet.pos.x)), (int)(floorf(bullet.pos.y)) };
				sf::Vector2i vTargetCell = sf::Vector2i({ (int)bulletPoss.x, (int)bulletPoss.y });
				int vAreaTop = maximum(((int)(minimum(vCurrentCell.y, vTargetCell.y) - 1)), (int)0);
				int vAreaLeft = maximum(((int)(minimum(vCurrentCell.x, vTargetCell.x) - 1)), (int)0);
				int vAreaBttm = minimum((maximum(vCurrentCell.y, vTargetCell.y) + 3), (vWorldSize.y - 1));
				int vAreaRight = minimum((maximum(vCurrentCell.x, vTargetCell.x) + 3), (vWorldSize.x - 1));

				if (playerBullets.first == nPlayerID)
				{
					vAreaTopThis = vAreaTop;
					vAreaLeftThis = vAreaLeft;
					vAreaRightThis = vAreaRight;
					vAreaBttmThis = vAreaBttm;

				}

				//	// Iterate through each cell in test area
				sf::Vector2i vCell;
				for (vCell.y = vAreaTop; vCell.y <= vAreaBttm; vCell.y++)
				{
					for (vCell.x = vAreaLeft; vCell.x <= vAreaRight; vCell.x++)
					{
						// Check if the cell is actually solid...
					//	sf::Vector2f vCellMiddle = vCell.floor();
						if (sWorldMap[vCell.y * vWorldSize.x + vCell.x] == '#')
						{
							// ...it is! So work out nearest point to future player position, around perimeter
							// of cell rectangle. We can test the distance to this point to see if we have
							// collided.

							sf::Vector2f vNearestPoint;
							//				// Inspired by this (very clever btw) 
							//				// https://stackoverflow.com/questions/45370692/circle-rectangle-collision-response
							vNearestPoint.x = (float)std::max(float(vCell.x), std::min((bulletPoss.x), float(vCell.x + 1)));
							vNearestPoint.y = (float)std::max(float(vCell.y), std::min((bulletPoss.y), float(vCell.y + 1)));

							//				// But modified to work :P
							sf::Vector2f vRayToNearest = { vNearestPoint.x - (bulletPoss.x), vNearestPoint.y - (bulletPoss.y) };

							float fOverlap = (mapObjects[playerBullets.first].fRadius - vRayToNearest.length());
							if (std::isnan(fOverlap)) fOverlap = 0;// Thanks Dandistine!

							//				// If overlap is positive, then a collision has occurred, so we displace backwards by the 
							//				// overlap amount. The potential position is then tested against other tiles in the area
							//				// therefore "statically" resolving the collision
							if (fOverlap > 0)
							{
								//					// Statically resolve the collision


								// change to hit target instead of static collide			
								sf::Vector2f tmp = (vRayToNearest == sf::Vector2f{ 0.f, 0.f }) ? sf::Vector2f{ 0.f, 0.f } : vRayToNearest.normalized();
								bulletPoss = bulletPoss - (tmp * fOverlap);

							}
						}
					}
				}
				bullet.pos = bulletPoss;
			}
		}

	}

	void handlePanAndZooming(float dt)
	{


		if ((leftMouseButtonHeld || leftMouseButtonPressed) && !sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		{
			leftMouseButtonReleased = true;
			leftMouseButtonHeld = false;
			leftMouseButtonPressed = false;
		}
		else
		{
			leftMouseButtonReleased = false;
		}

		// Handle Pan & Zoom
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && isFocused)
		{
			if (leftMouseButtonPressed || leftMouseButtonHeld)
			{
				leftMouseButtonHeld = true;
				leftMouseButtonPressed = false;
			}
			else
			{
				leftMouseButtonPressed = true;
				leftMouseButtonHeld = false;
			}
		}
		else
		{
			leftMouseButtonPressed = false;
			leftMouseButtonHeld = false;
		}



		if (leftMouseButtonPressed && !isMoving)
		{
			if (isFocused)
				startPan(tv, sf::Mouse::getPosition(tv));
		}
		else if (leftMouseButtonHeld)
		{

			if (isFocused && object.vel == sf::Vector2f{ 0.f,0.f } && !isMoving)
				updatePan(tv, sf::Mouse::getPosition(tv), dt);
			else
			{
				if (hasDropped)
				{
					initialLoc = sf::Mouse::getPosition(tv);
					dropPan(tv, sf::Mouse::getPosition(tv));
				}
				else
				{
					dropPan(tv, sf::Mouse::getPosition(tv));
				}
			}

		}
		else if (leftMouseButtonReleased)
		{
			if (isFocused)
			{
				dropPan(tv, sf::Mouse::getPosition(tv));
			}
			else
			{
				if (!hasDropped)
				{
					dropPan(tv, sf::Mouse::getPosition(tv));
				}
			}
		}

		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) && isPanning && !isMoving && !leftMouseButtonPressed && !leftMouseButtonHeld)
			isPanning = false;
	}

	void renderScene()
	{
		// Clear World
		tv.clear();

		//sf::Vector2f topLeftPixel = tv.mapPixelToCoords({ 0,0 });

		// Draw World
		sf::Vector2i vStartCell = { maximum((int)((tv.getView().getCenter().x / tileSize) - (((tv.getSize().x / 2.f) / zoomFactor.x) / tileSize)) ,0), maximum((int)((tv.getView().getCenter().y / tileSize) - (((tv.getSize().y / 2.f) / zoomFactor.y) / tileSize)), 0) };
		int vRight = minimum((vStartCell.x + ((int)floorf(32.f / (zoomFactor.x)))), vWorldSize.x - 1);//minimum((int)((tv.getView().getCenter().x + tv.getSize().x / 2.f) / tileSize) + 1, vWorldSize.x);
		int vBttm = minimum((vStartCell.y + ((int)floorf(15.f / (zoomFactor.y)))), vWorldSize.y - 1);// ((int)((tv.getView().getCenter().y + tv.getSize().y / 2.f) / tileSize) + 1, vWorldSize.y);
		for (auto y = vStartCell.y; y <= vBttm; y++)
		{
			for (auto x = vStartCell.x; x <= vRight; x++)
			{
				if (sWorldMap[y * vWorldSize.x + x] == '#')
				{
					sf::RectangleShape rect{ {64.f,64.f} };
					rect.setFillColor(sf::Color::Transparent);
					rect.setOutlineColor(sf::Color::White);
					rect.setOutlineThickness(1);
					rect.setPosition({ (float)x * tileSize, (float)y * tileSize });
					sf::RectangleShape rect2{ {50.f,50.f} };
					rect2.setFillColor(sf::Color::Transparent);
					rect2.setOutlineColor(sf::Color::White);
					rect2.setOutlineThickness(1);
					rect2.setPosition({ (float)(x * tileSize + 7), (float)(y * tileSize + 7) });
					tv.draw(rect);
					tv.draw(rect2);
				}
			}
		}




		// Draw World Objects
		for (auto& object : mapObjects)
		{
			// Draw Boundary
			sf::CircleShape shp{ 32.f };
			shp.setFillColor(sf::Color::White);
			shp.setOutlineColor(sf::Color::Transparent);
			shp.setOutlineThickness(0);
			shp.setPosition({ object.second.vPos.x * tileSize - 32.f, object.second.vPos.y * tileSize - 32.f });




			tv.draw(shp);
			if (object.first == this->nPlayerID)
			{

			}
			else
			{
				sf::Text txt{ fnt };
				txt.setString(std::to_string(object.first));
				txt.setPosition({ object.second.vPos.x * tileSize, object.second.vPos.y * tileSize + (object.second.fRadius * tileSize) + 32.f });
				txt.setFillColor(sf::Color::Blue);
				txt.setCharacterSize(24U);
				tv.draw(txt);
			}

		}





		sf::Text nameText{ fnt };

		if (object.vel != sf::Vector2f{ 0.f, 0.f })
		{

			sf::VertexArray line(sf::PrimitiveType::Lines, 2Ui64);
			line[0].color = sf::Color::Magenta;
			line[1].color = sf::Color::Magenta;
			line[0].position = sf::Vector2f{ object.pos.x * tileSize, object.pos.y * tileSize };


			sf::Vector2f velLine = (object.vel.lengthSquared() == 0) ? sf::Vector2f{ 0.f,0.f } : sf::Vector2f{ object.vel.normalized() * 32.f }; //{ (object.vel.x * ((object.fRad * tileSize) / 2.f)) * tileSize,  (object.vel.y * ((object.fRad * tileSize) / 2.f)) * tileSize };

			line[1].position = line[0].position + velLine;

			tv.draw(line);
		}
		nameText.setString("ID " + std::to_string(nPlayerID));


		sf::RectangleShape bounds{ { float((vAreaRightThis - vAreaLeftThis) * tileSize), float((vAreaBttmThis - vAreaTopThis) * tileSize) } };
		bounds.setPosition({ float(vAreaLeftThis * tileSize),float(vAreaTopThis * tileSize) });
		bounds.setFillColor(sf::Color(47, 147, 247, 53));
		tv.draw(bounds);

		for (auto& playerBullets : projectiles)
		{
			for (auto& bullet : playerBullets.second)
			{
				sf::CircleShape shp{ 3.f };
				shp.setFillColor(sf::Color::Yellow);
				shp.setPosition({ bullet.pos.x * tileSize, bullet.pos.y * tileSize });
				tv.draw(shp);
			}
		}


		// Draw Name
		sf::RectangleShape plaque{ {180.f,60.f} };
		plaque.setFillColor(sf::Color::Black);
		plaque.setOutlineThickness(4);
		plaque.setOutlineColor(sf::Color::White);
		plaque.setPosition({ 30.f,40.f });



		nameText.setFillColor(sf::Color::Green);
		nameText.setCharacterSize(32U);
		nameText.setPosition({ 50.f,50.f });
		auto vw = tv.getView();
		tv.setView(tv.getDefaultView());
		tv.draw(plaque);
		tv.draw(nameText);
		tv.setView(vw);

		tv.display();

	}

	void updateOtherClients()
	{
		// Send player description
		cnet::message<GameMsg> msg;
		msg.header.id = GameMsg::Game_UpdatePlayer;
		mapObjects[nPlayerID].vPos = object.pos;
		mapObjects[nPlayerID].vVel = object.vel;
		msg << mapObjects[nPlayerID];
		Send(msg);
	}

	void pollEvents(float dt)
	{
		while (const std::optional event = tv.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				if (isFocused)
					tv.close();
			}
			else if (event->is<sf::Event::FocusGained>())
			{
				isFocused = true;
				break;
			}
			else if (event->is<sf::Event::FocusLost>())
			{
				isFocused = false;
				break;
			}
			else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
			{
				if (isFocused)
				{
					if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
					{
						tv.close();
						break;
					}


				}
			}
			else if (event->is<sf::Event::MouseWheelScrolled>())
			{
				static float currZoom{ 1.f };
				if (isFocused)
				{
					if (event->getIf<sf::Event::MouseWheelScrolled>()->delta > 0)
					{
						// scrolled up, zoom un
						auto vw = tv.getView();
						currZoom = -10.f * dt;
						vw.zoom(1.f + currZoom);

						tv.setView(vw);

						zoomFactor = { 1600.f / tv.getView().getSize().x , 900.f / tv.getView().getSize().y };

					}
					else if (event->getIf<sf::Event::MouseWheelScrolled>()->delta < 0)
					{
						// scrolled down, zoom out
						auto vw = tv.getView();
						currZoom = 10.f * dt;

						vw.zoom(1.f + currZoom);


						tv.setView(vw);
						zoomFactor = { 1600.f / tv.getView().getSize().x , 900.f / tv.getView().getSize().y };

					}
					else
					{

					}
				}
			}

		}
	}



	bool run()
	{
		sf::Clock timer{};
		// Check for incoming network messages
		if (IsConnected())
		{
			while (bool stillWaiting = handleNewClientMessages()) {};

			while (tv.isOpen())
			{
				handleCoreLoopMessages();

				float dt = timer.restart().asSeconds();
				isFocused = isMyWindowInFocus(tv);

				pollEvents(dt);

				handleMyStaticInput(dt);

				updateAndCollidePlayers(dt);
				updateAndCollideProjectiles(dt);
				
				handlePanAndZooming(dt);
				renderScene();

				updateOtherClients();
					
			}
		}
		return true;
	}
};

int main()
{
	MMOGame demo;
	demo.Init();

	demo.run();
	return 0;
}
#include "../MMO_Server/MMO_Common.h"

#include <SFML/Graphics.hpp>

#include <map>
#include <net_client.h>
#include <net_message.h>

class MMOGame : cnet::client_interface<GameMsg>
{
	bool isFocused = true;
	sf::RenderWindow tv;
	sf::Vector2i initialLoc = { 0,0 };
	sf::Vector2i panStart = { 0,0 };
	sf::Font fnt{ "assets/font1.ttf" };
	sf::Texture playerTex{ "assets/textures/players/playerSheet.png" };
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
			sf::Mouse::setPosition({ (sf::Vector2i)(sf::Vector2f{(float)tv.getSize().x / 2.f, (float)tv.getSize().y / 2.f}) }, tv);
			panStart = sf::Mouse::getPosition(tv);

			initialLoc = startPos_;
			hasDropped = false;
		}
	}

	void updatePan(sf::RenderWindow& wnd_, sf::Vector2i mousePos_, float dt_)
	{
		float dispX = 0.f;
		float dispY = 0.f;
		//panStart = tv.getView().getCenter();
		
		if (mousePos_.x > panStart.x)
		{
			dispX = -0.777f;
		}
		else if (mousePos_.x < panStart.x)
		{
			dispX = 0.777f;
		}
		else
		{
			dispX = 0.f;
		}

		if (mousePos_.y > panStart.y)
		{
			dispY = -0.777f;
		}
		else if (mousePos_.y < panStart.y)
		{
			dispY = 0.777f;
		}
		else
		{
			dispY = 0.f;
		}
		sf::View vw = wnd_.getView();
		vw.move({ dispX * dt_, dispY * dt_ });
	
		//panStart = mousePos_;
		wnd_.setView(vw);
		


		sf::Mouse::setPosition(tv.mapCoordsToPixel({(float)tv.getView().getCenter().x, (float)tv.getView().getCenter().y }), tv);
		panStart = sf::Mouse::getPosition(tv);
	}

	void dropPan(sf::RenderWindow& wnd_, sf::Vector2i mousePos_)
	{

		std::cout << "\nNormal Size: 1600 900, zoom is 1, 1" << std::endl;
		std::cout << "\nCurrent Size: " << std::to_string(tv.getView().getSize().x) << ", " << std::to_string(tv.getView().getSize().y) << std::endl;

		std::cout << "\nRatio normal to current: " << std::to_string(1600.f /  (tv.getView().getSize().x)) << ", " << std::to_string(900.f /  (tv.getView().getSize().y)) << std::endl;

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

public:
	bool Init()
	{
		
		//mapObjects[0].nUniqueID = 0;
		//mapObjects[0].vPos = { 3.0f, 3.0f };

		sf::ContextSettings settings;
		settings.antiAliasingLevel = 8;
		settings.attributeFlags = sf::ContextSettings::Attribute::Core;

		tv.create( sf::VideoMode({1600U, 900U},32U), "MMO CLIENT", sf::State::Windowed , settings);
		if (Connect("192.168.0.6", 60000))
		{
			object.pos = { 3.f,3.f };
			return true;
		}

		mapObjects.emplace(std::pair<uint32_t, sPlayerDescription> { 0, sPlayerDescription{} });

		
		


		return false;
	}

	bool run()
	{
		float dt = 0.f;
		sf::Clock timer{};
		// Check for incoming network messages
		if (IsConnected())
		{
			static bool stillWaiting = true;
			while (stillWaiting)
			{


				while (!Incoming().empty())
				{
					auto msg = Incoming().pop_front().msg;

					switch (msg.header.id)
					{
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

						projectiles.emplace(nPlayerID, std::vector<WorldObject>{});
						projectiles[nPlayerID].clear();


						break;
					}

					case(GameMsg::Game_AddPlayer):
					{
						sPlayerDescription desc;
						msg >> desc;
						mapObjects.insert_or_assign(desc.nUniqueID, desc);
						projectiles.insert_or_assign(desc.nUniqueID, std::vector<WorldObject>{});
						projectiles[desc.nUniqueID].clear();
						if (desc.nUniqueID == nPlayerID)
						{
							// Now we exist in game world
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


				if (bWaitingForConnection)
				{
					tv.clear(sf::Color(47, 147, 247, 255));
					std::cout << "\n" << "Waiting To Connect..." << std::endl;
					stillWaiting = true;
				}
				else
				{
					stillWaiting = false;
				}

		
			}
			while (tv.isOpen())
			{



				while (!Incoming().empty())
				{
					auto msg = Incoming().pop_front().msg;

					switch (msg.header.id)
					{
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
						break;
					}

					case(GameMsg::Game_AddPlayer):
					{
						sPlayerDescription desc;
						msg >> desc;
						mapObjects.insert_or_assign(desc.nUniqueID, desc);

						if (desc.nUniqueID == nPlayerID)
						{
							// Now we exist in game world
							bWaitingForConnection = false;
						}
						break;
					}

					case(GameMsg::Game_RemovePlayer):
					{
						uint32_t nRemovalID = 0;
						msg >> nRemovalID;
						mapObjects.erase(nRemovalID);
						break;
					}

					case(GameMsg::Game_UpdatePlayer):
					{
						sPlayerDescription desc;
						msg >> desc;
						mapObjects.insert_or_assign(desc.nUniqueID, desc);
						break;
					}

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


					}
				}



				dt = timer.getElapsedTime().asSeconds();
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
						if (isFocused)
						{
							if (event->getIf<sf::Event::MouseWheelScrolled>()->delta > 0)
							{
								// scrolled up, zoom un
								auto vw = tv.getView();
								vw.zoom(1.f - 0.02f * dt);

								//zoomFactor *= (1.f - 0.2f * dt);
								tv.setView(vw);

								zoomFactor = { 1600.f / tv.getView().getSize().x , 900.f / tv.getView().getSize().y };

							}
							else if (event->getIf<sf::Event::MouseWheelScrolled>()->delta < 0)
							{
								// scrolled down, zoom out
								auto vw = tv.getView();
								vw.zoom(1.f + 0.02f * dt);


								tv.setView(vw);
								zoomFactor = { 1600.f / tv.getView().getSize().x , 900.f / tv.getView().getSize().y };

							}
							else
							{

							}
						}
					}
				}
				// Control of Player Object
				//mapObjects[nPlayerID].vVel = { 0.f,0.f };
				object.vel = { 0.f,0.f };

				if (isFocused)
				{
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
					{
						currDir = Direction::NW;
						object.vel = { -0.0177f, -0.0177f };
					}
					else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
					{
						currDir = Direction::NE;
						object.vel = { 0.0177f, -0.0177f };


					}
					else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
					{
						currDir = Direction::SW;
						object.vel = { -0.0177f, 0.0177f };

					}
					else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::D))
					{
						currDir = Direction::SE;
						object.vel = { 0.0177f, 0.0177f };

					}
					//mapObjects[nPlayerID].vVel = { 0.0f, 0.0f };
					else
					{

						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
						{
							object.vel.y = -0.02f;
							//mapObjects[nPlayerID].vVel.y = -0.02f;
							currDir = Direction::N;


						}
						else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
							object.vel.y = 0.02f;
							currDir = Direction::S;
						}// mapObjects[nPlayerID].vVel.y = 0.02f; }


						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
							object.vel.x = -0.02f;
							currDir = Direction::W;

						} //mapObjects[nPlayerID].vVel.x = -0.02f; }
						else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
							object.vel.x = 0.02f;
							currDir = Direction::E;

						}//mapObjects[nPlayerID].vVel.x = 0.02f; }

					}



					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Space))
					{
						auto& p = projectiles[nPlayerID];
						p.emplace_back(WorldObject{});
						p[p.size() - 1].ownerID = nPlayerID;
						p[p.size() - 1].pos = object.pos;
						p[p.size() - 1].vel = { 0.f,0.f };
						p[p.size() - 1].fRad = 1.f;

						switch (currDir) {
						case Direction::N:
						{
							p[p.size() - 1].vel = sf::Vector2f{ 0.f,-.002f };
						}
						break;
						case Direction::NE:
						{
							p[p.size() - 1].vel = sf::Vector2f{ .00177f,-.00177f };
						}
						break;
						case Direction::E:
						{
							p[p.size() - 1].vel = sf::Vector2f{ .002f,0.f };
						}
						break;
						case Direction::SE:
						{
							p[p.size() - 1].vel = sf::Vector2f{ .00177f,.00177f };
						}
						break;
						case Direction::S:
						{
							p[p.size() - 1].vel = sf::Vector2f{ 0.f,.002f };
						}
						break;
						case Direction::SW:
						{
							p[p.size() - 1].vel = sf::Vector2f{ -.00177f,.00177f };
						}
						break;
						case Direction::W:
						{
							p[p.size() - 1].vel = sf::Vector2f{ -0.002f,0.f };
						}
						break;
						case Direction::NW:
						{
							p[p.size() - 1].vel = sf::Vector2f{ -.00177f,-.00177f };
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

				if (object.vel.lengthSquared() > 0)
				{
					object.vel = object.vel.normalized() * 0.001f;
				}

				sf::Vector2f vPotentialPosition = object.pos + (object.vel * dt);

				/*	if (mapObjects[nPlayerID].vVel.length() > 0)
					{
						if (mapObjects[nPlayerID].vVel.x > 3.f)
						{
							mapObjects[nPlayerID].vVel.x = 3.f;
						}
						else if (mapObjects[nPlayerID].vVel.x < -3.f)
						{
							mapObjects[nPlayerID].vVel.x = -3.f;
						}
						else if (mapObjects[nPlayerID].vVel.y > 3.f)
						{
							mapObjects[nPlayerID].vVel.y = 3.f;
						}
						else if (mapObjects[nPlayerID].vVel.y < -3.f)
						{
							mapObjects[nPlayerID].vVel.y = -3.f;
						}

					}*/

				sf::Vector2i vCurrentCell = { (int)(floorf(object.pos.x)), (int)(floorf(object.pos.y)) };
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
					//	sf::Vector2f vCellMiddle = vCell.floor();
						if (sWorldMap[vCell.y * vWorldSize.x + vCell.x] == '#')
						{
							// ...it is! So work out nearest point to future player position, around perimeter
							// of cell rectangle. We can test the distance to this point to see if we have
							// collided.

							sf::Vector2f vNearestPoint;
							//				// Inspired by this (very clever btw) 
							//				// https://stackoverflow.com/questions/45370692/circle-rectangle-collision-response
							vNearestPoint.x = (float)std::max(float(vCell.x), std::min((vPotentialPosition.x), float(vCell.x + 1)));
							vNearestPoint.y = (float)std::max(float(vCell.y), std::min((vPotentialPosition.y), float(vCell.y + 1)));

							//				// But modified to work :P
							sf::Vector2f vRayToNearest = { vNearestPoint.x - (vPotentialPosition.x), vNearestPoint.y - (vPotentialPosition.y) };

							float fOverlap = (object.fRad - vRayToNearest.length());
							if (std::isnan(fOverlap)) fOverlap = 0;// Thanks Dandistine!

							//				// If overlap is positive, then a collision has occurred, so we displace backwards by the 
							//				// overlap amount. The potential position is then tested against other tiles in the area
							//				// therefore "statically" resolving the collision
							if (fOverlap > 0)
							{
								//					// Statically resolve the collision
								//			
								sf::Vector2f tmp = (vRayToNearest == sf::Vector2f{ 0.f, 0.f }) ? sf::Vector2f{ 0.f, 0.f } : vRayToNearest.normalized();
								vPotentialPosition = vPotentialPosition - (tmp * fOverlap);
							}
						}
					}
				}

				object.pos = vPotentialPosition;


				// now update the bullets locally and check for collision, then send this players updated bullets to the server in a loop which will update all the players with all those bullets, and the other players will do the same
				for (auto& playerBullets : projectiles)
				{
					for (auto& bullet : playerBullets.second)
					{
						float dt = mapObjects[bullet.ownerID].dt;

						sf::Vector2f bulletPoss = bullet.pos + (bullet.vel * dt);

						sf::Vector2i vCurrentCell = { (int)(floorf(bullet.pos.x)), (int)(floorf(bullet.pos.y)) };
						sf::Vector2i vTargetCell = sf::Vector2i({ (int)bulletPoss.x, (int)bulletPoss.y });
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

									float fOverlap = (object.fRad - vRayToNearest.length());
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

				// Update objects locally
				//auto& obj = object;
				//for (auto& object : mapObjects)
				//{
				////	// Where will object be worst case?
				//	sf::Vector2f vPotentialPosition = (object.second.vPos + (object.second.vVel * dt));

				////	// Extract region of world cells that could have collision this frame
				//	sf::Vector2i vCurrentCell = { (int)(floorf(object.second.vPos.x)), (int)(floorf(object.second.vPos.y))};
				//	sf::Vector2i vTargetCell = sf::Vector2i({ (int)vPotentialPosition.x, (int)vPotentialPosition.y });
				//	int vAreaTop = maximum(((int)(minimum(vCurrentCell.y, vTargetCell.y))), (int)0);
				//	int vAreaLeft = maximum(((int)(minimum(vCurrentCell.x, vTargetCell.x))), (int)0);
				//	int vAreaBttm = minimum((maximum(vCurrentCell.y, vTargetCell.y) + 1), (vWorldSize.y - 1));
				//	int vAreaRight = minimum((maximum(vCurrentCell.x, vTargetCell.x) + 1), (vWorldSize.x - 1));


				////	// Iterate through each cell in test area
				//	sf::Vector2i vCell;
				//	for (vCell.y = vAreaTop; vCell.y <= vAreaBttm; vCell.y++)
				//	{
				//		for (vCell.x = vAreaLeft; vCell.x <= vAreaRight; vCell.x++)
				//		{
				//			// Check if the cell is actually solid...
				//		//	sf::Vector2f vCellMiddle = vCell.floor();
				//			if (sWorldMap[vCell.y * vWorldSize.x + vCell.x] == '#')
				//			{
				//				// ...it is! So work out nearest point to future player position, around perimeter
				//				// of cell rectangle. We can test the distance to this point to see if we have
				//				// collided.

				//				sf::Vector2f vNearestPoint;
				////				// Inspired by this (very clever btw) 
				////				// https://stackoverflow.com/questions/45370692/circle-rectangle-collision-response
				//				vNearestPoint.x = (float)maximum(int(vCell.x), minimum((int)vPotentialPosition.x, int(vCell.x + 1)));
				//				vNearestPoint.y = (float)maximum(int(vCell.y), minimum((int)vPotentialPosition.y, int(vCell.y + 1)));

				////				// But modified to work :P
				//				sf::Vector2f vRayToNearest = { vNearestPoint.x - vPotentialPosition.x, vNearestPoint.y - vPotentialPosition.y };
				//				vRayToNearest.x += object.second.vVel.x;
				//				vRayToNearest.y += object.second.vVel.y;
				//				float mag2 = vRayToNearest.lengthSquared();
				//				float fOverlap = (object.second.fRadius - mag2);
				//				if (std::isnan(fOverlap)) fOverlap = 0;// Thanks Dandistine!

				////				// If overlap is positive, then a collision has occurred, so we displace backwards by the 
				////				// overlap amount. The potential position is then tested against other tiles in the area
				////				// therefore "statically" resolving the collision
				//				if (fOverlap > 0)
				//				{
				////					// Statically resolve the collision
				////			
				//					vPotentialPosition = { vPotentialPosition - (vRayToNearest.normalized() * fOverlap) * dt };
				//				}
				//			}
				//		}
				//	}

				////	// Set the objects new position to the allowed potential position
				//	object.second.vPos = vPotentialPosition;
				//}

				//object.pos = mapObjects[nPlayerID].vPos;
				//object.vel = mapObjects[nPlayerID].vVel;





				static bool leftMouseButtonReleased{ false };
				static bool leftMouseButtonPressed{ false };
				static bool leftMouseButtonHeld{ false };
				if (isFocused)
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
					if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
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

					if (leftMouseButtonPressed)
					{
						startPan(tv, sf::Mouse::getPosition(tv));
					}
					else if (leftMouseButtonHeld)
					{
						updatePan(tv, sf::Mouse::getPosition(tv), dt);
					}
					else if (leftMouseButtonReleased)
					{
						dropPan(tv, sf::Mouse::getPosition(tv));
					}
				}
			
				//if (GetMouseWheel() > 0) tv.ZoomAtScreenPos(1.5f, GetMousePos());
				//if (GetMouseWheel() < 0) tv.ZoomAtScreenPos(0.75f, GetMousePos());


				//// FINALIZE POSITIONS
				//for (auto& o : mapObjects)
				//{
				//	o.second.vPos += o.second.vVel * dt;
				//}


				// Clear World
				tv.clear();

				sf::Vector2f topLeftPixel = tv.mapPixelToCoords({ 0,0 });

				// Draw World
				sf::Vector2i vStartCell = { maximum((int)((tv.getView().getCenter().x / tileSize) - (((tv.getSize().x / 2.f) / zoomFactor.x) / tileSize)) ,0), maximum((int)((tv.getView().getCenter().y / tileSize) - (((tv.getSize().y / 2.f) / zoomFactor.y) / tileSize)), 0)};
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
					


					//sf::Sprite p1{ playerTex };
					//p1.setTextureRect(walkAnim[currentDir].at(currentFrame));
					//p1.setPosition({ object.second.vPos });
					tv.draw(shp);
					if (object.first == this->nPlayerID)
					{
					
					}
					else
					{
						sf::Text txt{ fnt };
						txt.setString(std::to_string(object.first));
						txt.setPosition({object.second.vPos.x * tileSize, object.second.vPos.y * tileSize + (object.second.fRadius * tileSize) + 32.f});
						txt.setFillColor(sf::Color::Blue);
						txt.setCharacterSize(24U);
						tv.draw(txt);
					}



					// Draw Velocity
				
					
					//if (object.second.vVel.mag2() > 0)
						//tv.DrawLine(object.second.vPos, object.second.vPos + object.second.vVel.norm() * object.second.fRadius, olc::MAGENTA);

					//sf::Vector2i vNameSize = GetTextSizeProp("ID: " + std::to_string(object.first));
					//tv.DrawStringPropDecal(object.second.vPos - sf::Vector2f{ vNameSize.x * 0.5f * 0.25f * 0.125f, -object.second.fRadius * 1.25f }, "ID: " + std::to_string(object.first), olc::BLUE, { 0.25f, 0.25f });
				}


				


				sf::Text nameText{ fnt };

				if (object.vel != sf::Vector2f{ 0.f, 0.f })
				{

					sf::VertexArray line(sf::PrimitiveType::Lines, 2Ui64);
					line[0].color = sf::Color::Magenta;
					line[1].color = sf::Color::Magenta;
					line[0].position = sf::Vector2f{ object.pos.x * tileSize, object.pos.y * tileSize };


					sf::Vector2f velLine = (object.vel.lengthSquared() == 0) ? sf::Vector2f{ 0.f,0.f } : sf::Vector2f{ object.vel.normalized() * 32.f }; //{ (object.vel.x * ((object.fRad * tileSize) / 2.f)) * tileSize,  (object.vel.y * ((object.fRad * tileSize) / 2.f)) * tileSize };
					/*if (object.vel.x != 0.f && object.vel.y != 0.f)
					{
						velLine.x *= 0.70f;
						velLine.y *= 0.70f;
					}*/
					line[1].position = line[0].position + velLine;

					tv.draw(line);
				}
				nameText.setString("ID " + std::to_string(nPlayerID));


				sf::RectangleShape bounds{ { float((vAreaRight - vAreaLeft) * tileSize), float((vAreaBttm - vAreaTop) * tileSize) } };
				bounds.setPosition({float(vAreaLeft * tileSize),float(vAreaTop * tileSize)});
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

				// Send player description
				cnet::message<GameMsg> msg;
				msg.header.id = GameMsg::Game_UpdatePlayer;
				mapObjects[nPlayerID].vPos = object.pos;
				mapObjects[nPlayerID].vVel = object.vel;
				mapObjects[nPlayerID].dt = dt;
				msg << mapObjects[nPlayerID];
				Send(msg);
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
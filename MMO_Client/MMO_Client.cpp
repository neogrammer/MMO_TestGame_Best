#include "../MMO_Server/MMO_Common.h"

#include <SFML/Graphics.hpp>

#include <map>
#include <net_client.h>
#include <net_message.h>
#include <stage/StageLoader.h>
#include <stage/Tilemap.h>
#include <nlohmann/json.hpp>
#include <game_objects/Player.h>

using json = nlohmann::json;
using namespace StageLoader;


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

	Player player{};
	std::unique_ptr<StageData> currStage;
	std::unique_ptr<sf::Texture> currTileset;
	std::vector<sf::IntRect> tileRects;
	bool isFocused = true;
	sf::RenderWindow tv;
	sf::Vector2i initialLoc = { 0,0 };
	sf::Vector2i panStart = { 0,0 };
	sf::Font fnt{ "assets/fonts/font1.ttf" };
	sf::Texture playerTex{ "assets/textures/players/playerSheet.png" };
	const float BASE_SPEED = 2.f; // 120 pixels per second at normal zoom (1.f)
	sf::Vector2f zoomFactor{ 1.f,1.f };
	int tileSize{ 64 };
	bool hasDropped{ true };
	TileMap tilemap1;
	
	std::map<uint32_t, sPlayerDescription> mapObjects;


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
		, currStage{}
		, currTileset{}
		, tileRects{}
		, tilemap1{}
	{
		projectiles.clear();

		object.ownerID = nPlayerID;

		currDir = Direction::S;
		
		currStage = std::make_unique<StageData>();
		*currStage = StageLoader::loadFromJsonFile("assets/data/stage_1.json");

		std::cout << currStage->map.floors[0].tileIndices[2] << std::endl;


		currTileset = std::make_unique<sf::Texture>("assets/textures/tilesets/stage_1_tileset.png");

		tileRects.clear();
		tileRects.reserve(17 * 19);



		for (int y = 0; y < 17; y++)
		{
			for (int x = 0; x < 19; x++)
			{
				tileRects.emplace_back(sf::IntRect{ {x*(int)currStage->tileSize,y*(int)currStage->tileSize},{(int)currStage->tileSize,(int)currStage->tileSize} });
			}
		}
		tilemap1.build(*currStage);
	}

private:
	uint32_t nPlayerID = 0;
	sPlayerDescription descPlayer;
	const char* sAppName;
	bool bWaitingForConnection = true;
	bool gameFull{ false };

	bool isPanning{ false };

	bool leftMouseButtonReleased{ false };
	bool leftMouseButtonPressed{ false };
	bool leftMouseButtonHeld{ false };

	const float speed = BASE_SPEED * (1.f / zoomFactor.x);
	bool isMoving = false;
	bool gameOver = false;

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
		if (Connect("127.0.0.1", 60000))
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

				
				case(GameMsg::Client_GameFull):
				{
						std::cout << "\nGame is full.  Try again later" << std::endl;
						// user has not been assigned id so just let the program end
						gameOver = true;
				}
				break;
				
				
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


			if (bWaitingForConnection && !gameOver)
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
		if (!gameOver)
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

	inline float length(const sf::Vector2f& v)
	{
		return std::sqrt(v.x * v.x + v.y * v.y);
	}

	inline sf::Vector2f normalize(const sf::Vector2f& v)
	{
		float l = length(v);
		return (l > 0.f) ? v / l : sf::Vector2f{ 0.f, 0.f };
	}

	//--------------------------------------------------
// Utilities that work with the new TileMap
//--------------------------------------------------
	static inline bool isBlocking(Tile::Type t)
	{
		switch (t)
		{
		case Tile::Type::NOTILE:
		case Tile::Type::PASS:
		case Tile::Type::TEMP:   // walk‑through & single‑use
		case Tile::Type::ABOVE:  // purely decorative overlay
			return false;
		default:                 // SOLID, HORIZ, VERT, etc.
			return true;
		}
	}

//--------------------------------------------------
// PLAYER HANDLING  (replace your old function body)
//--------------------------------------------------
void updateAndCollidePlayers(float dt, TileMap& tm)
{
	const int w = (int)tm.getMapWidth();
	const int h = (int)tm.getMapHeight();

	auto cellIsSolid = [&](int cx, int cy) -> bool
		{
			if (cx < 0 || cy < 0 || cx >= w || cy >= h)
				return true;                              // treat off‑map as walls

			Tile::Type t = tm.getTileType(cy * w + cx);   // safe – handles “‑1”
			return isBlocking(t);
		};

	for (auto& player : mapObjects)
	{
		//--------------------------------------------------
		// 1. integrate velocity
		//--------------------------------------------------
		if (player.first == object.ownerID)
		{
			player.second.vPos = object.pos;
			player.second.vVel = object.vel;
		}
		sf::Vector2f nextPos =
			player.second.vPos +
			player.second.vVel * ((player.first == object.ownerID) ? 1.f : dt);

		//--------------------------------------------------
		// 2. build sweep area in tile space
		//--------------------------------------------------
		sf::Vector2i curCell((int)std::floor(player.second.vPos.x),
			(int)std::floor(player.second.vPos.y));
		sf::Vector2i tgtCell((int)nextPos.x, (int)nextPos.y);

		int areaTop = std::max(std::min(curCell.y, tgtCell.y) - 1, 0);
		int areaLeft = std::max(std::min(curCell.x, tgtCell.x) - 1, 0);
		int areaBottom = std::min(std::max(curCell.y, tgtCell.y) + 3, h - 1);
		int areaRight = std::min(std::max(curCell.x, tgtCell.x) + 3, w - 1);

		//--------------------------------------------------
		// 3. SAT‑style circle‑vs‑tile resolve
		//--------------------------------------------------
		for (int ty = areaTop; ty <= areaBottom; ++ty)
			for (int tx = areaLeft; tx <= areaRight; ++tx)
			{
				if (!cellIsSolid(tx, ty)) continue;

				// nearest point on the block (unit square) to the circle centre
				sf::Vector2f nearest(
					std::clamp(nextPos.x, (float)tx, (float)(tx + 1)),
					std::clamp(nextPos.y, (float)ty, (float)(ty + 1)));

				sf::Vector2f delta = nearest - nextPos;
				float overlap = player.second.fRadius - length(delta);
				if (overlap > 0.f && !std::isnan(overlap))
				{
					sf::Vector2f n = (delta == sf::Vector2f{ 0,0 })
						? sf::Vector2f{ 0,0 }
					: normalize(delta);
					nextPos -= n * overlap;               // push out of wall
				}
			}

		//--------------------------------------------------
		// 4. commit position + camera follow
		//--------------------------------------------------
		player.second.vPos = nextPos;

		if (player.first == nPlayerID)
		{
			object.pos = player.second.vPos;
			object.vel = player.second.vVel;

			if (!isPanning && isMoving)               // your existing rule
			{
				auto vw = tv.getView();
				vw.setCenter(object.pos * (float)tm.getTileSize());
				tv.setView(vw);
			}
		}
	}
}

//--------------------------------------------------
// PROJECTILE HANDLING  (replace your old function body)
//--------------------------------------------------
void updateAndCollideProjectiles(float dt, TileMap& tm)
{
	const int w = (int)tm.getMapWidth();
	const int h = (int)tm.getMapHeight();

	auto cellIsSolid = [&](int cx, int cy) -> bool
		{
			if (cx < 0 || cy < 0 || cx >= w || cy >= h)
				return true;
			return isBlocking(tm.getTileType(cy * w + cx));
		};

	for (auto& playerBullets : projectiles)
	{
		for (auto& bullet : playerBullets.second)
		{
			sf::Vector2f nextPos = bullet.pos + bullet.vel * dt;

			sf::Vector2i curCell((int)std::floor(bullet.pos.x),
				(int)std::floor(bullet.pos.y));
			sf::Vector2i tgtCell((int)nextPos.x, (int)nextPos.y);

			int areaTop = std::max(std::min(curCell.y, tgtCell.y) - 1, 0);
			int areaLeft = std::max(std::min(curCell.x, tgtCell.x) - 1, 0);
			int areaBottom = std::min(std::max(curCell.y, tgtCell.y) + 3, h - 1);
			int areaRight = std::min(std::max(curCell.x, tgtCell.x) + 3, w - 1);

			for (int ty = areaTop; ty <= areaBottom; ++ty)
				for (int tx = areaLeft; tx <= areaRight; ++tx)
				{
					if (!cellIsSolid(tx, ty)) continue;

					sf::Vector2f nearest(
						std::clamp(nextPos.x, (float)tx, (float)(tx + 1)),
						std::clamp(nextPos.y, (float)ty, (float)(ty + 1)));

					sf::Vector2f delta = nearest - nextPos;
					float overlap = mapObjects[playerBullets.first].fRadius  // same
					- length(delta);
					if (overlap > 0.f && !std::isnan(overlap))
					{
						sf::Vector2f n = (delta == sf::Vector2f{ 0,0 })
							? sf::Vector2f{ 0,0 }
						: normalize(delta);
						nextPos -= n * overlap;
					}
				}
			bullet.pos = nextPos;
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

		tv.draw(tilemap1);



		// Draw World Objects
		for (auto& object : mapObjects)
		{
			// Draw Boundary
			//sf::CircleShape shp{ 32.f };
			//shp.setFillColor(sf::Color::White);
			//shp.setOutlineColor(sf::Color::Transparent);
			//shp.setOutlineThickness(0);
			//shp.setPosition({ object.second.vPos.x * tileSize - 32.f, object.second.vPos.y * tileSize - 32.f });




			//tv.draw(shp);
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


	}

	void updateOtherClients()
	{
		// Send player description
		cnet::message<GameMsg> msg;
		msg.header.id = GameMsg::Game_UpdatePlayer;
		mapObjects[nPlayerID].vPos = object.pos;
		mapObjects[nPlayerID].vVel = object.vel;
		mapObjects[nPlayerID].currSizeX = player.getWorldSize().x;
		mapObjects[nPlayerID].currSizeY = player.getWorldSize().y;

		mapObjects[nPlayerID].dir = (uint32_t)player.animMgr.getCurrDir();
		mapObjects[nPlayerID].currOffsetX = player.getCurrOffset().x;
		mapObjects[nPlayerID].currOffsetY = player.getCurrOffset().y;

		mapObjects[nPlayerID].currFrame = (uint32_t)player.animMgr.getCurrIndex();
		mapObjects[nPlayerID].texIDStr = (uint32_t)player.getTexID();
		mapObjects[nPlayerID].texRectLeft = player.getTexRect().position.x;
		mapObjects[nPlayerID].texRectTop = player.getTexRect().position.y;
		mapObjects[nPlayerID].texRectWidth = player.getTexRect().size.x;
		mapObjects[nPlayerID].texRectHeight = player.getTexRect().size.y;

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
			player.setPosition(object.pos);
			player.setVelocity(object.vel);


			while (bool stillWaiting = handleNewClientMessages()) {};

			while (tv.isOpen())
			{
				if (gameOver)
				{
					tv.close();
				}

				handleCoreLoopMessages();

				float dt = timer.restart().asSeconds();

				if (tv.isOpen())
				{
					isFocused = isMyWindowInFocus(tv);
					pollEvents(dt);
				}

				if (tv.isOpen())
				{

					handleMyStaticInput(dt);

					updateAndCollidePlayers(dt, tilemap1);
					player.setPosition({ object.pos.x * tileSize,object.pos.y * tileSize });
					player.setVelocity(object.vel);

					updateAndCollideProjectiles(dt, tilemap1);

					handlePanAndZooming(dt);

					tilemap1.update(tv.mapPixelToCoords({ 0,0 }), tv);

					player.finalize(dt, tv);

					renderScene();

					player.render(tv);

					// draw other players
					for (auto& user : mapObjects)
					{
						if (user.first == object.ownerID) continue;
						sf::Sprite tmp{ Cfg::textures.get((Cfg::Textures)user.second.texIDStr) };
						tmp.setTextureRect(sf::IntRect{ sf::Vector2i{(int)user.second.texRectLeft,(int)user.second.texRectTop},sf::Vector2i{(int)user.second.texRectWidth,(int)user.second.texRectHeight}  });
						//currOffset = { 30.f, 10.f };
						tmp.setPosition({ user.second.vPos.x * tileSize - 36.f, user.second.vPos.y * tileSize - 30.f });
						tv.draw(tmp);
					}


					tv.display();

					updateOtherClients();
				}
			}
		}
		return true;
	}
};

int main()
{
	Cfg::Initialize();

	MMOGame demo;
	demo.Init();

	demo.run();

	return 0;
}
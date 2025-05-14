#include <iostream>
#include <unordered_map>

#include "MMO_Common.h"
#include <net_server.h>

class GameServer : public cnet::server_interface<GameMsg>
{
public:
	GameServer(uint16_t nPort) : cnet::server_interface<GameMsg>(nPort)
	{
		m_mapPlayerBullets.clear();
	}

	std::unordered_map<uint32_t, sPlayerDescription> m_mapPlayerRoster;
	std::unordered_map<uint32_t, std::vector<BulletDescription>> m_mapPlayerBullets;

	std::vector<uint32_t> m_vGarbageIDs;

protected:
	bool OnClientConnect(std::shared_ptr<cnet::connection<GameMsg>> client) override
	{
		// For now we will allow all 
		return true;
	}

	void OnClientValidated(std::shared_ptr<cnet::connection<GameMsg>> client) override
	{
		// Client passed validation check, so send them a message informing
		// them they can continue to communicate
		m_mapPlayerBullets[client->GetID()] = std::vector<BulletDescription>{};
		m_mapPlayerBullets[client->GetID()].clear();


		 if (m_mapPlayerRoster.size() >= 4)
		 {
			   cnet::message<GameMsg> msg;
			   msg.header.id = GameMsg::Client_GameFull;
			   client->Send(msg);
		 }
		 else 
		 {
				cnet::message<GameMsg> msg;
				msg.header.id = GameMsg::Client_Accepted;
				client->Send(msg);
		 }
	}

	void OnClientDisconnect(std::shared_ptr<cnet::connection<GameMsg>> client) override
	{
		if (client)
		{
			if (m_mapPlayerRoster.find(client->GetID()) == m_mapPlayerRoster.end())
			{
				// client never added to roster, so just let it disappear
				std::cout << "A Client that needs to be removed apparently is not even on the roster, so fuck it" << std::endl;
			}
			else
			{
				auto& pd = m_mapPlayerRoster[client->GetID()];
				std::cout << "[UNGRACEFUL REMOVAL]:" + std::to_string(pd.nUniqueID) + "\n";
				m_mapPlayerRoster.erase(client->GetID());
				m_vGarbageIDs.push_back(client->GetID());
			}
		}

	}

	void OnMessage(std::shared_ptr<cnet::connection<GameMsg>> client, cnet::message<GameMsg>& msg) override
	{
		if (!m_vGarbageIDs.empty())
		{
			for (auto pid : m_vGarbageIDs)
			{
				cnet::message<GameMsg> m;
				m.header.id = GameMsg::Game_RemovePlayer;
				m << pid;
				std::cout << "Removing " << pid << "\n";
				MessageAllClients(m);
			}
			m_vGarbageIDs.clear();
		}

		switch (msg.header.id)
		{
		case GameMsg::Client_RegisterWithServer:
		{
			sPlayerDescription desc;
			msg >> desc;
			desc.nUniqueID = client->GetID();
			m_mapPlayerRoster.insert_or_assign(desc.nUniqueID, desc);
			cnet::message<GameMsg> msgSendID;
			msgSendID.header.id = GameMsg::Client_AssignID;
			msgSendID << desc.nUniqueID;
			MessageClient(client, msgSendID);
			cnet::message<GameMsg> msgAddPlayer;
			msgAddPlayer.header.id = GameMsg::Game_AddPlayer;
			msgAddPlayer << desc;
			MessageAllClients(msgAddPlayer);
			for (const auto& player : m_mapPlayerRoster)
			{
				cnet::message<GameMsg> msgAddOtherPlayers;
				msgAddOtherPlayers.header.id = GameMsg::Game_AddPlayer;
				msgAddOtherPlayers << player.second;
				MessageClient(client, msgAddOtherPlayers);
			}
		}
		break;
		case GameMsg::Client_UnregisterWithServer:
		{
			break;
		}

		case GameMsg::Game_UpdatePlayer:
		{
			MessageAllClients(msg, client);
			break;
		}


		case GameMsg::Game_AddBullet:
		{
			BulletDescription desc;
			msg >> desc;
			m_mapPlayerBullets[client->GetID()].push_back(desc);
			uint32_t idx = (uint32_t)(m_mapPlayerBullets[client->GetID()].size() - 1);
			desc.index = idx;
			cnet::message<GameMsg> msgAddBullet;
			msgAddBullet.header.id = GameMsg::Game_AddBullet;
			msgAddBullet << desc;

			MessageAllClients(msgAddBullet, client);
			break;
		}

		case GameMsg::Game_RemoveBullet:
		{
			BulletDescription desc;
			msg >> desc;
			
			uint32_t idx = desc.index;
		
			m_mapPlayerBullets[desc.nUniqueID].erase(m_mapPlayerBullets[desc.nUniqueID].begin() + idx);
			cnet::message<GameMsg> msgRemoveBullet;
			msgRemoveBullet.header.id = GameMsg::Game_RemoveBullet;
			msgRemoveBullet << desc;

			MessageAllClients(msgRemoveBullet, client);
			break;
		}

		case GameMsg::Game_UpdateBullet:
		{
			MessageAllClients(msg, client);
			break;
		}

		case GameMsg::Server_GetPing:
		{
		}
		break;
		}

	}

};



int main()
{
	GameServer server(60000);
	server.Start();

	while (1)
	{
		server.Update(-1, true);
	}
	return 0;
}
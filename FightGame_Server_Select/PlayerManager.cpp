#include "PlayerManager.h"
#include "NetworkManager.h"
#include "CreatePacket.h"
#include "PacketDefine.h"

PlayerManager::PlayerManager()
{

}

PlayerManager::~PlayerManager()
{

}

void PlayerManager::DestroyAllPlayers()
{
	Player* player;
	while (!_playerList.empty())
	{
		player = _playerList.back();
		_playerList.pop_back();
		//player->GetSession()->SetSessionDead();
		delete(player);
	}
}

void PlayerManager::DestroyDeadPlayers()
{
	for (std::list<Player*>::iterator i = _playerList.begin(); i != _playerList.end();)
	{
		// Destroy Dead Player
		if (!(*i)->GetStateAlive())
		{
			Player* deletedPlayer = (*i);

			stPACKET_SC_DELETE_CHARACTER packetDelete;
			Create_PACKET_SC_DELETE_CHARACTER(packetDelete, deletedPlayer->GetID());
			HeaderANDMsgBroadcast(dfPACKET_SC_DELETE_CHARACTER, (char*)&packetDelete,
				sizeof(stPACKET_SC_DELETE_CHARACTER));

			deletedPlayer->GetSession()->SetSessionDead();
			i = _playerList.erase(i);
			delete(deletedPlayer);
			printf("죽은 플레이어가 생겼습니다!\n");
		}
		else
			i++;
	}
}

void PlayerManager::PlayerActionProc()
{
	DestroyDeadPlayers();

	for (std::list<Player*>::iterator i = _playerList.begin(); i != _playerList.end(); i++)
	{
		// recvbuf에서 행동 정보 가져옴
		(*i)->DeqFromRecvbufANDHandlePacket();
		// 정보에 맞게 행동으로 옮김
		(*i)->Move();

		// 옮긴 행동을 다른 이들에게 알림 -> SC PACKETS을 broadcasting함
		if ((*i)->GetPacketMoveStart())
		{
			stPACKET_SC_MOVE_START packetSCMoveStart;
			Create_PACKET_SC_MOVE_START(packetSCMoveStart,
				(*i)->GetID(), (*i)->GetMoveDirection(), (*i)->GetX(), (*i)->GetY());
			HeaderANDMsgBroadcast(dfPACKET_SC_MOVE_START, (char*)&packetSCMoveStart,
				sizeof(stPACKET_SC_MOVE_START), (*i));
			(*i)->ResetPacketMoveStart();
		}

		if ((*i)->GetPacketMoveStop())
		{
			stPACKET_SC_MOVE_STOP packetSCMoveStop;
			Create_PACKET_SC_MOVE_STOP(packetSCMoveStop,
				(*i)->GetID(), (*i)->GetHeadDirection(), (*i)->GetX(), (*i)->GetY());
			HeaderANDMsgBroadcast(dfPACKET_SC_MOVE_STOP, (char*)&packetSCMoveStop,
				sizeof(stPACKET_SC_MOVE_STOP), (*i));
			(*i)->ResetPacketMoveStop();
		}

		if ((*i)->GetPacketAttack1())
		{
			stPACKET_SC_ATTACK1 packetSCAttack1;
			Create_PACKET_SC_ATTACK1(packetSCAttack1,
				(*i)->GetID(), (*i)->GetHeadDirection(), (*i)->GetX(), (*i)->GetY());
			ChkHitANDBroadcast(dfATTACK_TYPE_ATTACK1,
				(*i)->GetID(), (*i)->GetHeadDirection(), (*i)->GetX(), (*i)->GetY());
			HeaderANDMsgBroadcast(dfPACKET_SC_ATTACK1, (char*)&packetSCAttack1,
				sizeof(stPACKET_SC_ATTACK1), (*i));
			(*i)->ResetPacketAttack1();
		}

		if ((*i)->GetPacketAttack2())
		{
			stPACKET_SC_ATTACK2 packetSCAttack2;
			Create_PACKET_SC_ATTACK2(packetSCAttack2,
				(*i)->GetID(), (*i)->GetHeadDirection(), (*i)->GetX(), (*i)->GetY());
			ChkHitANDBroadcast(dfATTACK_TYPE_ATTACK2,
				(*i)->GetID(), (*i)->GetHeadDirection(), (*i)->GetX(), (*i)->GetY());
			HeaderANDMsgBroadcast(dfPACKET_SC_ATTACK2, (char*)&packetSCAttack2,
				sizeof(stPACKET_SC_ATTACK2), (*i));
			(*i)->ResetPacketAttack2();
		}

		if ((*i)->GetPacketAttack3())
		{
			stPACKET_SC_ATTACK3 packetSCAttack3;
			Create_PACKET_SC_ATTACK3(packetSCAttack3,
				(*i)->GetID(), (*i)->GetHeadDirection(), (*i)->GetX(), (*i)->GetY());
			ChkHitANDBroadcast(dfATTACK_TYPE_ATTACK3,
				(*i)->GetID(), (*i)->GetHeadDirection(), (*i)->GetX(), (*i)->GetY());
			HeaderANDMsgBroadcast(dfPACKET_SC_ATTACK3, (char*)&packetSCAttack3,
				sizeof(stPACKET_SC_ATTACK3), (*i));
			(*i)->ResetPacketAttack3();
		}
	}
}

void PlayerManager::CreatePlayer(Session* newSession)
{
	Player* player = new Player(newSession, _give_id_to_player);
	_playerList.push_back(player);

	// Send <Allocate ID Message> to New Player
	stPACKET_SC_CREATE_MY_CHARACTER packetCreateMy;
	Create_PACKET_SC_CREATE_MY_CHARACTER(packetCreateMy,
		player->GetID(), player->GetHeadDirection(), player->GetX(), player->GetY(), player->GetHp());
	HeaderANDMsgUnicast(dfPACKET_SC_CREATE_MY_CHARACTER, (char*)&packetCreateMy,
		sizeof(stPACKET_SC_CREATE_MY_CHARACTER), player);

	// Send <Create New Player Message> to All Player
	stPACKET_SC_CREATE_OTHER_CHARACTER packetCreateOther;
	Create_PACKET_SC_CREATE_OTHER_CHARACTER(packetCreateOther,
		player->GetID(), player->GetHeadDirection(), player->GetX(), player->GetY(), player->GetHp());
	HeaderANDMsgBroadcast(dfPACKET_SC_CREATE_OTHER_CHARACTER, (char*)&packetCreateOther,
		sizeof(stPACKET_SC_CREATE_OTHER_CHARACTER), player);

	// Send <Create All Players Message> to New Player
	stPACKET_SC_CREATE_OTHER_CHARACTER packetCreateAll;
	for (std::list<Player*>::iterator i = _playerList.begin(); i != _playerList.end(); i++)
	{
		if ((*i)->GetID() != _give_id_to_player)
		{
			Create_PACKET_SC_CREATE_OTHER_CHARACTER(packetCreateAll,
				(*i)->GetID(), (*i)->GetHeadDirection(), (*i)->GetX(), (*i)->GetY(), (*i)->GetHp());
			HeaderANDMsgUnicast(dfPACKET_SC_CREATE_OTHER_CHARACTER, (char*)&packetCreateAll,
				sizeof(stPACKET_SC_CREATE_OTHER_CHARACTER), player);
		}
	}

	_give_id_to_player++;
}

void PlayerManager::HeaderANDMsgUnicast(UINT8 msgType, char* msg, int size, Player* pPlayer)
{
	stPACKET_HEADER header;
	header.Code = dfPACKET_HEADER_CODE;
	header.Size = size;
	header.Type = msgType;

	NetworkManager::GetInstance()->EnqSendBufUnicast((char*)&header, dfPACKET_HEADER_LEN, pPlayer->GetSession());
	NetworkManager::GetInstance()->EnqSendBufUnicast(msg, size, pPlayer->GetSession());
}

void PlayerManager::HeaderANDMsgBroadcast(UINT8 msgType, char* msg, int size, Player* pExpPlayer)
{
	stPACKET_HEADER header;
	header.Code = dfPACKET_HEADER_CODE;
	header.Size = size;
	header.Type = msgType;

	if (pExpPlayer != nullptr)
	{
		NetworkManager::GetInstance()->EnqSendBufBroadcast((char*)&header, dfPACKET_HEADER_LEN, pExpPlayer->GetSession());
		NetworkManager::GetInstance()->EnqSendBufBroadcast(msg, size, pExpPlayer->GetSession());
	}
	else
	{
	NetworkManager::GetInstance()->EnqSendBufBroadcast((char*)&header, dfPACKET_HEADER_LEN);
		NetworkManager::GetInstance()->EnqSendBufBroadcast(msg, size);
	}
}

void PlayerManager::ChkHitANDBroadcast(UINT8 attackType, UINT32 ID, UINT8 direction, UINT16 x, UINT16 y)
{
	UINT16 rangeX;
	UINT16 rangeY;
	UINT16 damage;

	switch (attackType)
	{
	case dfATTACK_TYPE_ATTACK1:
		rangeX = dfATTACK1_RANGE_X;
		rangeY = dfATTACK1_RANGE_Y;
		damage = dfATTACK1_DAMAGE;
		break;

	case dfATTACK_TYPE_ATTACK2:
		rangeX = dfATTACK2_RANGE_X;
		rangeY = dfATTACK2_RANGE_Y;
		damage = dfATTACK2_DAMAGE;
		break;

	case dfATTACK_TYPE_ATTACK3:
		rangeX = dfATTACK3_RANGE_X;
		rangeY = dfATTACK3_RANGE_Y;
		damage = dfATTACK3_DAMAGE;
		break;

	default:
		return;
	}

	if (direction == dfPACKET_MOVE_DIR_LL)
	{
		UINT16 minX = x - rangeX;
		UINT16 minY = y - rangeY;
		UINT16 maxY = y + rangeY;
		for (std::list<Player*>::iterator i = _playerList.begin(); i != _playerList.end(); i++)
		{
			if ((*i)->GetX() >= minX && (*i)->GetX() <= x &&
				(*i)->GetY() >= minY && (*i)->GetY() <= maxY && ID != (*i)->GetID())
			{
				(*i)->TakeDamage(damage);
				stPACKET_SC_DAMAGE packetSCDamage;
				Create_PACKET_SC_DAMAGE(packetSCDamage, ID, (*i)->GetID(), (*i)->GetHp());
				HeaderANDMsgBroadcast(dfPACKET_SC_DAMAGE, (char*)&packetSCDamage,
					sizeof(stPACKET_SC_DAMAGE));
			}
		}
	}

	else if (direction == dfPACKET_MOVE_DIR_RR)
	{
		UINT16 maxX = x + rangeX;
		UINT16 minY = y - rangeY;
		UINT16 maxY = y + rangeY;

		for (std::list<Player*>::iterator i = _playerList.begin(); i != _playerList.end(); i++)
		{
			if ((*i)->GetX() >= x && (*i)->GetX() <= maxX &&
				(*i)->GetY() >= minY && (*i)->GetY() <= maxY && ID != (*i)->GetID())
			{
				(*i)->TakeDamage(damage);
				stPACKET_SC_DAMAGE packetSCDamage;
				Create_PACKET_SC_DAMAGE(packetSCDamage, ID, (*i)->GetID(), (*i)->GetHp());
				HeaderANDMsgBroadcast(dfPACKET_SC_DAMAGE, (char*)&packetSCDamage,
					sizeof(stPACKET_SC_DAMAGE));
			}
		}
	}
}

#include "CreatePacket.h"

void Create_PACKET_HEADER(
	stPACKET_HEADER& header,
	UINT8 Size, UINT8 Type)
{
	header.Code = 0x89;
	header.Size = Size;
	header.Type = Type;
}

void Create_PACKET_SC_CREATE_MY_CHARACTER(
	stPACKET_SC_CREATE_MY_CHARACTER& pakcet,
	UINT32 ID, UINT8 Direction, UINT16 X, UINT16 Y, UINT8 HP)
{
	pakcet.ID = ID;
	pakcet.Direction = Direction;
	pakcet.X = X;
	pakcet.Y = Y;
	pakcet.HP = HP;
}

void Create_PACKET_SC_CREATE_OTHER_CHARACTER(
	stPACKET_SC_CREATE_OTHER_CHARACTER& pakcet,
	UINT32 ID, UINT8 Direction, UINT16 X, UINT16 Y, UINT8 HP)
{
	pakcet.ID = ID;
	pakcet.Direction = Direction;
	pakcet.X = X;
	pakcet.Y = Y;
	pakcet.HP = HP;
}

void Create_PACKET_SC_DELETE_CHARACTER(
	stPACKET_SC_DELETE_CHARACTER& pakcet,
	UINT32 ID)
{
	pakcet.ID = ID;
}


void Create_PACKET_CS_MOVE_START(
	stPACKET_CS_MOVE_START& pakcet,
	UINT8 Direction, UINT16 X, UINT16 Y)
{
	pakcet.Direction = Direction;
	pakcet.X = X;
	pakcet.Y = Y;
}

void Create_PACKET_SC_MOVE_START(
	stPACKET_SC_MOVE_START& pakcet,
	UINT32 ID, UINT8 Direction, UINT16 X, UINT16 Y)
{
	pakcet.ID = ID;
	pakcet.Direction = Direction;
	pakcet.X = X;
	pakcet.Y = Y;
}

void Create_PACKET_CS_MOVE_STOP(
	stPACKET_CS_MOVE_STOP& pakcet,
	UINT8 Direction, UINT16 X, UINT16 Y)
{
	pakcet.Direction = Direction;
	pakcet.X = X;
	pakcet.Y = Y;
}

void Create_PACKET_SC_MOVE_STOP(
	stPACKET_SC_MOVE_STOP& pakcet,
	UINT32 ID, UINT8 Direction, UINT16 X, UINT16 Y)
{
	pakcet.ID = ID;
	pakcet.Direction = Direction;
	pakcet.X = X;
	pakcet.Y = Y;
}

void Create_PACKET_CS_ATTACK1(
	stPACKET_CS_ATTACK1& pakcet,
	UINT8 Direction, UINT16 X, UINT16 Y)
{
	pakcet.Direction = Direction;
	pakcet.X = X;
	pakcet.Y = Y;
}

void Create_PACKET_SC_ATTACK1(
	stPACKET_SC_ATTACK1& pakcet,
	UINT32 ID, UINT8 Direction, UINT16 X, UINT16 Y)
{
	pakcet.ID = ID;
	pakcet.Direction = Direction;
	pakcet.X = X;
	pakcet.Y = Y;
}

void Create_PACKET_CS_ATTACK2(
	stPACKET_CS_ATTACK2& pakcet,
	UINT8 Direction, UINT16 X, UINT16 Y)
{
	pakcet.Direction = Direction;
	pakcet.X = X;
	pakcet.Y = Y;
}

void Create_PACKET_SC_ATTACK2(
	stPACKET_SC_ATTACK2& pakcet,
	UINT32 ID, UINT8 Direction, UINT16 X, UINT16 Y)
{
	pakcet.ID = ID;
	pakcet.Direction = Direction;
	pakcet.X = X;
	pakcet.Y = Y;
}
void Create_PACKET_CS_ATTACK3(
	stPACKET_CS_ATTACK3& pakcet,
	UINT8 Direction, UINT16 X, UINT16 Y)
{
	pakcet.Direction = Direction;
	pakcet.X = X;
	pakcet.Y = Y;
}

void Create_PACKET_SC_ATTACK3(
	stPACKET_SC_ATTACK3& pakcet,
	UINT32 ID, UINT8 Direction, UINT16 X, UINT16 Y)
{
	pakcet.ID = ID;
	pakcet.Direction = Direction;
	pakcet.X = X;
	pakcet.Y = Y;
}


void Create_PACKET_SC_DAMAGE(
	stPACKET_SC_DAMAGE& packet,
	UINT32 AttackID, UINT32 DamageID, UINT8 DamageHP)
{
	packet.AttackID = AttackID;
	packet.DamageID = DamageID;
	packet.DamageHP = DamageHP;
}

void Create_PACKET_CS_SYNC(
	stPACKET_CS_SYNC& packet,
	UINT16 X, UINT16 Y)
{
	packet.X = X;
	packet.Y = Y;
}

void Create_PACKET_SC_SYNC(
	stPACKET_SC_SYNC& packet,
	UINT16 X, UINT16 Y)
{
	packet.X = X;
	packet.Y = Y;
}
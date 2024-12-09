#ifndef __PACKET_DEFINE__
#define __PACKET_DEFINE__
#include <basetsd.h>

// PACKET TYPE Define
#define	dfPACKET_SC_CREATE_MY_CHARACTER			0
#define	dfPACKET_SC_CREATE_OTHER_CHARACTER		1
#define	dfPACKET_SC_DELETE_CHARACTER			2

#define	dfPACKET_CS_MOVE_START					10
#define	dfPACKET_SC_MOVE_START					11
#define	dfPACKET_CS_MOVE_STOP					12
#define	dfPACKET_SC_MOVE_STOP					13

#define	dfPACKET_CS_ATTACK1						20
#define	dfPACKET_SC_ATTACK1						21
#define	dfPACKET_CS_ATTACK2						22
#define	dfPACKET_SC_ATTACK2						23
#define	dfPACKET_CS_ATTACK3						24
#define	dfPACKET_SC_ATTACK3						25

#define	dfPACKET_SC_DAMAGE						30

#define	dfPACKET_CS_SYNC						250
#define	dfPACKET_SC_SYNC						251

#define dfPACKET_HEADER_CODE					0x89
#pragma pack (push, 1)
#define dfPACKET_HEADER_LEN						sizeof(stPACKET_HEADER)

struct stPACKET_HEADER
{
	UINT8	Code = dfPACKET_HEADER_CODE;
	UINT8	Size;
	UINT8	Type;
};

struct stPACKET_SC_CREATE_MY_CHARACTER
{
	UINT32 ID;
	UINT8 Direction;
	UINT16 X;
	UINT16 Y;
	UINT8 HP;
};

struct stPACKET_SC_CREATE_OTHER_CHARACTER
{
	UINT32 ID;
	UINT8 Direction;
	UINT16 X;
	UINT16 Y;
	UINT8 HP;
};

struct stPACKET_SC_DELETE_CHARACTER
{
	UINT32 ID;
};

struct stPACKET_CS_MOVE_START
{
	UINT8 Direction;
	UINT16 X;
	UINT16 Y;
};

struct stPACKET_SC_MOVE_START
{
	UINT32 ID;
	UINT8 Direction;
	UINT16 X;
	UINT16 Y;
};

struct stPACKET_CS_MOVE_STOP
{
	UINT8 Direction;
	UINT16 X;
	UINT16 Y;
};

struct stPACKET_SC_MOVE_STOP
{
	UINT32 ID;
	UINT8 Direction;
	UINT16 X;
	UINT16 Y;
};

struct stPACKET_CS_ATTACK1
{
	UINT8 Direction;
	UINT16 X;
	UINT16 Y;
};

struct stPACKET_SC_ATTACK1
{
	UINT32 ID;
	UINT8 Direction;
	UINT16 X;
	UINT16 Y;
};

struct stPACKET_CS_ATTACK2
{
	UINT8 Direction;
	UINT16 X;
	UINT16 Y;
};

struct stPACKET_SC_ATTACK2
{
	UINT32 ID;
	UINT8 Direction;
	UINT16 X;
	UINT16 Y;
};

struct stPACKET_CS_ATTACK3
{
	UINT8 Direction;
	UINT16 X;
	UINT16 Y;
};

struct stPACKET_SC_ATTACK3
{
	UINT32 ID;
	UINT8 Direction;
	UINT16 X;
	UINT16 Y;
};

struct stPACKET_SC_DAMAGE
{
	UINT32 AttackID;
	UINT32 DamageID;
	UINT8 DamageHP;
};

struct stPACKET_CS_SYNC
{
	UINT16 X;
	UINT16 Y;
};

struct stPACKET_SC_SYNC
{
	UINT16 X;
	UINT16 Y;
};
#pragma pack(pop)
#endif


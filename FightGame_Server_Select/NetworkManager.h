#pragma once

#include "RingBuffer.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <list>
#pragma comment(lib, "ws2_32")

/*========================
*		DEFINE
========================*/
# define NETWORK_DEBUG
#define IP L"0.0.0.0"
#define PORT 5000

/*========================
*		FUNCTION
========================*/


/*========================
*		CLASS
========================*/
class Player;
class NetworkManager;
class PlayerManager;

class Session
{
	friend NetworkManager;
	friend PlayerManager;
	friend Player;
private:
	void SetSessionAlive() { _bAlive = true; }
	void SetSessionDead() { _bAlive = false; }
	bool GetSessionAlive() { return _bAlive; }
private:
	bool		_bAlive;
	SOCKET		_sock;
	RingBuffer	_sendBuf;
	RingBuffer	_recvBuf;
};


class NetworkManager
{
private:
	NetworkManager();
	~NetworkManager();
	static NetworkManager*	_NetworkMgr;
	std::list<Session*>		_sessionList;
	SOCKET					_listensock;
	FD_SET					_rset;
	FD_SET					_wset;
public:
	// ���� �� �Ҵ� ����
	NetworkManager(const NetworkManager&) = delete;
	NetworkManager& operator=(const NetworkManager&) = delete;
public:
	/*========================
	*	CLASS FUNCTION
	========================*/
	void NetworkInitialize();
	void SelectModel();
	//�̱��� �ν��Ͻ� ����
	static NetworkManager* GetInstance()
	{
		if (_NetworkMgr == nullptr)
		{
			_NetworkMgr = new NetworkManager();
		}
		return _NetworkMgr;
	}
	void EnqSendBufUnicast(char* msg, int size, Session* pSession);
	void EnqSendBufBroadcast(char* msg, int size, Session* pExpSession = nullptr);
	void Terminate();

private:
	void AcceptProc();
	void RecvProc(Session* session);
	void SendProc(Session* session);
	void DisconnectDeadSessions();
};


#include "NetworkManager.h"
#include "PlayerManager.h"
#include "main.h"
#include <stdio.h>


NetworkManager::NetworkManager()
{

}
NetworkManager::~NetworkManager()
{

}

void NetworkManager::NetworkInitialize()
{
	int err;
	int bindRet;
	int listenRet;
	int ioctRet;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		g_bShutdown = true;
		return;
	}

	// Create Socket
	_listensock = socket(AF_INET, SOCK_STREAM, 0);
	if (_listensock == INVALID_SOCKET)
	{
		err = WSAGetLastError();
		printf("Error! Function %s Line %d: %d\n", __func__, __LINE__, err);
		g_bShutdown = true;
		return;
	}

	// Bind
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	InetPton(AF_INET, IP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(PORT);

	bindRet = bind(_listensock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (bindRet == SOCKET_ERROR)
	{
		err = WSAGetLastError();
		printf("Error! Function %s Line %d: %d\n", __func__, __LINE__, err);
		g_bShutdown = true;
		return;
	}

	// Listen
	listenRet = listen(_listensock, SOMAXCONN);
	if (listenRet == SOCKET_ERROR)
	{
		err = WSAGetLastError();
		printf("Error! Function %s Line %d: %d\n", __func__, __LINE__, err);
		g_bShutdown = true;
		return;
	}

	// Set Non-Blocking Mode
	u_long on = 1;
	ioctRet = ioctlsocket(_listensock, FIONBIO, &on);
	if (ioctRet == SOCKET_ERROR)
	{
		err = WSAGetLastError();
		printf("Error! Function %s Line %d: %d\n", __func__, __LINE__, err);
		g_bShutdown = true;
		return;
	}

	printf("Setting Complete!\n");
}

void NetworkManager::SelectModel()
{
	//select ��
	FD_ZERO(&_rset);
	FD_ZERO(&_wset);
	FD_SET(_listensock, &_rset);
	for (std::list<Session*>::iterator i = _sessionList.begin(); i != _sessionList.end(); i++)
	{
		FD_SET((*i)->_sock, &_rset);
		if ((*i)->_sendBuf.GetUseSize() > 0)
			FD_SET((*i)->_sock , &_wset);
	}

	// Select
	timeval time;
	time.tv_sec = 0;
	time.tv_usec = 0;
	int selectRet = select(0, &_rset, &_wset, NULL, &time);
	if (selectRet == SOCKET_ERROR)
	{
		int err = WSAGetLastError();
		printf("Error! Function %s Line %d: %d\n", __func__, __LINE__, err);
		g_bShutdown = true;
		return;
	}
	else if (selectRet > 0)
	{
		for (std::list<Session*>::iterator i = _sessionList.begin(); i != _sessionList.end(); i++)
		{
			if (FD_ISSET((*i)->_sock, &_wset))
				SendProc((*i));
		}

		if (FD_ISSET(_listensock, &_rset))
			AcceptProc();

		for (std::list<Session*>::iterator i = _sessionList.begin(); i != _sessionList.end(); i++)
		{
			if (FD_ISSET((*i)->_sock, &_rset))
				RecvProc((*i));
		}

		DisconnectDeadSessions();
	}
}

void NetworkManager::SendProc(Session* session)
{
	if (session->_sendBuf.GetUseSize() <= 0)
		return;

	char buf[MAX_BUF_SIZE];
	int sendBufSize = session->_sendBuf.GetUseSize();

	int peekRet = session->_sendBuf.Peek(buf, sendBufSize);
	if (peekRet != sendBufSize)
	{
		printf("Error! Func %s Line %d\n", __func__, __LINE__);
		session->SetSessionDead();
		return;
	}

	int sendRet = send(session->_sock, buf, peekRet, 0);
	if (sendRet == SOCKET_ERROR)
	{
		int err = WSAGetLastError();
		if (err != WSAEWOULDBLOCK)
		{
			printf("Error! Func %s Line %d: %d\n", __func__, __LINE__, err);
			session->SetSessionDead();
		}
		return;
	}

	session->_sendBuf.MoveReadPos(sendRet);

#ifdef NETWORK_DEBUG
	// For Test =======================================================
	buf[sendRet] = '\0';
	printf("\n\n");
	printf("Send Test===========================\n\n");
	int idx = 0;
	while (idx < sendRet)
	{
		printf("%d ", buf[idx]);
		idx++;
	}
	printf("\n\n===================================");
	printf("\n\n");
	// ===================================================================
#endif
}

void NetworkManager::AcceptProc()
{
	SOCKADDR_IN clientaddr;
	int addrlen = sizeof(clientaddr);
	Session* newSession = new Session;

	newSession->_sock = accept(_listensock, (SOCKADDR*)&clientaddr, &addrlen);
	if (newSession->_sock == INVALID_SOCKET)
	{
		int err = WSAGetLastError();
		printf("Error! Function %s Line %d: %d\n", __func__, __LINE__, err);
		delete newSession;
		return;
	}
	newSession->SetSessionAlive();
	_sessionList.push_back(newSession);

	PlayerManager::GetInstance()->CreatePlayer(newSession);
}

void NetworkManager::RecvProc(Session* session)
{
	char buf[MAX_BUF_SIZE];
	int recvRet = recv(session->_sock, buf, MAX_BUF_SIZE, 0);
	if (recvRet == SOCKET_ERROR)
	{
		int err = WSAGetLastError();
		if (err != WSAEWOULDBLOCK)
		{
			printf("Error! Func %s Line %d: %d\n", __func__, __LINE__, err);
			session->SetSessionDead();
			return;
		}
	}
	else if (recvRet == 0)
	{
		session->SetSessionDead();
		return;
	}

	int enqueueRet = session->_recvBuf.Enqueue(buf, recvRet);
	if (recvRet != enqueueRet)
	{
		printf("Error! Func %s Line %d\n", __func__, __LINE__);
		session->SetSessionDead();
		return;
	}

#ifdef NETWORK_DEBUG
	// For Test =======================================================
	buf[recvRet] = '\0';
	printf("\n\n");
	printf("Recv Test===========================\n\n");
	int idx = 0;
	while (idx < recvRet)
	{
		printf("%d ", buf[idx]);
		idx++;
	}
	printf("\n\n===================================");
	printf("\n\n");
	// ===================================================================
#endif
}

void NetworkManager::DisconnectDeadSessions()
{
	for (std::list<Session*>::iterator i = _sessionList.begin(); i != _sessionList.end();)
	{
		if (!(*i)->GetSessionAlive())
		{
			Session* pSession = *i;
			i = _sessionList.erase(i);
			closesocket(pSession->_sock);
			delete(pSession);
		}
		else
		{
			i++;
		}
	}
}

void NetworkManager::EnqSendBufUnicast(char* msg, int size, Session* pSession)
{
	int enqueueRet = pSession->_sendBuf.Enqueue(msg, size);
	if (enqueueRet != size)
	{
		printf("Error! Function %s Line %d\n", __func__, __LINE__);
		pSession->SetSessionDead();
	}
}

void NetworkManager::EnqSendBufBroadcast(char* msg, int size, Session* pExpSession)
{
	int enqueueRet;

	if (pExpSession == nullptr)
	{
		for (std::list<Session*>::iterator i = _sessionList.begin(); i != _sessionList.end(); i++)
		{
			if ((*i)->GetSessionAlive())
			{
				enqueueRet = (*i)->_sendBuf.Enqueue(msg, size);
				if (enqueueRet != size)
				{
					printf("Error! Function %s Line %d\n", __func__, __LINE__);
					(*i)->SetSessionDead();
				}
			}
		}
	}
	else
	{
		for (std::list<Session*>::iterator i = _sessionList.begin(); i != _sessionList.end(); i++)
		{
			if ((*i)->GetSessionAlive() && pExpSession->_sock != (*i)->_sock)
			{
				enqueueRet = (*i)->_sendBuf.Enqueue(msg, size);
				if (enqueueRet != size)
				{
					printf("Error! Function %s Line %d\n", __func__, __LINE__);
					(*i)->SetSessionDead();
				}
			}
		}
	}
}

void NetworkManager::Terminate()
{
	// Disconnect All Connected Session 
	for (std::list<Session*>::iterator i = _sessionList.begin(); i != _sessionList.end();)
	{
		Session* pSession = *i;
		i = _sessionList.erase(i);
		closesocket(pSession->_sock);
		delete(pSession);
	}
	closesocket(_listensock);
	WSACleanup();
}

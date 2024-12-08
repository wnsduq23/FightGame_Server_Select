#include "main.h"
#include "NetworkManager.h"
#include "PlayerManager.h"
#include "GameContents.h"

bool g_bShutdown = false;
NetworkManager* NetworkManager::_NetworkMgr = nullptr;
PlayerManager* PlayerManager::_PlayerMgr = nullptr;

int main(void)
{
	// network init
	NetworkManager::GetInstance()->NetworkInitialize();

	while (!g_bShutdown)
	{
		//network
		NetworkManager::GetInstance()->SelectModel();

		//Game logic
		GameContentsModule();
	}
	// terminate
	// �Ŀ� db�� ������ �����ͳ� ������ �� �ϵ��� �� ������ �� Ȯ���� �� ����
	PlayerManager::GetInstance()->DestroyAllPlayers();
	NetworkManager::GetInstance()->Terminate();
	return (0);
}
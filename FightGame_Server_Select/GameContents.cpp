#include "GameContents.h"
#include "PlayerManager.h"
#include <Windows.h>


bool SkipForFixedFrame(void)
{
	timeBeginPeriod(1);
	static DWORD oldTick = timeGetTime();
	//�ʹ� �ð��� ���� �� �������� skip
	if ((timeGetTime() - oldTick) < (1000 / FPS))
		return true;
	//���� ������ ���� ���
	oldTick += (1000 / FPS);
	return false;

}

void GameContentsModule()
{
	// ������ ���� 
	if (SkipForFixedFrame()) return;

	// �� Ȯ�强�� ����Ѵٸ� ���⿡ ����, �̺�Ʈ ����� ���� ��

	// �÷��̾� �̵� �� ���� ó��
	PlayerManager::GetInstance()->PlayerActionProc();
}
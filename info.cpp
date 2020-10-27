#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
//получение информации о ширине окна
extern "C" __declspec(dllexport) int getScreenInformation(char *infoString)
{
	sprintf(infoString, "Максимальная ширина окна %d",
		GetSystemMetrics(SM_CXFULLSCREEN));
	return 0;
}
//получение информации о поддержке MMX
extern "C" __declspec(dllexport) int getMMXInformation(char *infoString)
{
	bool MMX = false;
	__asm
	{
		MOV EAX,1
		CPUID
		BT EDX,23; проверить значение 23 бита
		JNC NO_MMX; переход, если бит равен 0
		MOV MMX,1
	NO_MMX:
	}
	if (MMX)
		sprintf(infoString, "Технология MMX поддерживается");
	else
		sprintf(infoString, "Технология MMX не поддерживается");
	return 0;
}
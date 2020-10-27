#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
//��������� ���������� � ������ ����
extern "C" __declspec(dllexport) int getScreenInformation(char *infoString)
{
	sprintf(infoString, "������������ ������ ���� %d",
		GetSystemMetrics(SM_CXFULLSCREEN));
	return 0;
}
//��������� ���������� � ��������� MMX
extern "C" __declspec(dllexport) int getMMXInformation(char *infoString)
{
	bool MMX = false;
	__asm
	{
		MOV EAX,1
		CPUID
		BT EDX,23; ��������� �������� 23 ����
		JNC NO_MMX; �������, ���� ��� ����� 0
		MOV MMX,1
	NO_MMX:
	}
	if (MMX)
		sprintf(infoString, "���������� MMX ��������������");
	else
		sprintf(infoString, "���������� MMX �� ��������������");
	return 0;
}
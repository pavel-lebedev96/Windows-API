#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
/*������� ������*/
DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
	char screenInfo[100], mmxInfo[100], * s;
	s = (char*)lpParameter;
	typedef int(*importFunction)(char*);
	importFunction DLLinfo;
	/*�������� ����������*/
	HINSTANCE hinstLib = LoadLibrary("info.dll");
	if (hinstLib == NULL)
	{
		sprintf(s, "������! �� ������� ��������� ���������� \"info.dll\"!");
		return 1;
	}
	DLLinfo = (importFunction)GetProcAddress(hinstLib, 
		"getScreenInformation");
	if (DLLinfo == NULL)
	{
		sprintf(s, "������! ������� \"getScreenInformation\" �� �������!");
		FreeLibrary(hinstLib);
		return 1;
	}
	/*��������� ���������� � ������ ������*/
	DLLinfo(screenInfo);

	DLLinfo = (importFunction)GetProcAddress(hinstLib,
		"getMMXInformation");
	if (DLLinfo == NULL)
	{
		sprintf(s, "������! ������� \"getMMXInformation\" �� �������!");
		FreeLibrary(hinstLib);
		return 1;
	}
	/*��������� ���������� � ��������� MMX*/
	DLLinfo(mmxInfo);
	FreeLibrary(hinstLib);
	sprintf(s, "%s, %s", screenInfo, mmxInfo);
	return 0;
}
/*������� ������������*/
int WINAPI WinMain(HINSTANCE hThisInst, HINSTANCE hPrevInst,
	LPSTR lpCmdLine, int nCmdShow)
{
	char szClassName[] = "MyClass";
	MSG msg;
	//������� ��������� ������ ����
	WNDCLASS wcl;
	//���������� ����
	HWND hWnd; 
	/*���������� ��������� ����������, � �������
	��������� ��������� ���������*/
	wcl.hInstance = hThisInst;
	/*��������� �� ������, ����������
	��� ������*/
	wcl.lpszClassName = szClassName;
	/*��������� �� ��������� ��������� ���������*/
	wcl.lpfnWndProc = WndProc;
	//����� ������ ����
	wcl.style = CS_HREDRAW | CS_VREDRAW;
	//���������� �����������
	wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	//���������� �������
	wcl.hCursor= LoadCursor(NULL, IDC_ARROW);
	//��������� �� ������, ���������� ��� ����
	wcl.lpszMenuName = NULL;
	//�������������� ���������
	wcl.cbClsExtra = 0;
	//�������������� ���������
	wcl.cbWndExtra = 0;
	//���� ���� ����
	wcl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	//����������� ������
	RegisterClass(&wcl);
	//�������� ����
	hWnd = CreateWindow(szClassName, 
		"�������������� ������� �� ����",
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN |
		WS_CLIPSIBLINGS, 100, 50, 700, 120,
		HWND_DESKTOP, NULL, hThisInst, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	//���� ��������� ���������
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.lParam;
}
/*��������� ��������� ���������*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, 
	WPARAM wParam, LPARAM lParam)
{
	HANDLE hThread;
	DWORD idThread;
	HDC hdc;
	PAINTSTRUCT ps;
	static char text[100];
	//��������� ���������
	switch (message)
	{
	case WM_CREATE:
		//�������� ������
		hThread = CreateThread(NULL, 0, ThreadProc,
			text, 0, &idThread);
		if (hThread == NULL)
		{
			sprintf(text, "������! �� ������� ������� �����!");
			return GetLastError();
		}
		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		//����� ������ � ����
		TextOut(hdc, 10, 10, text, strlen(text));
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}



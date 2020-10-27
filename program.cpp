#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
/*функция потока*/
DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
	char screenInfo[100], mmxInfo[100], * s;
	s = (char*)lpParameter;
	typedef int(*importFunction)(char*);
	importFunction DLLinfo;
	/*загрузка библиотеки*/
	HINSTANCE hinstLib = LoadLibrary("info.dll");
	if (hinstLib == NULL)
	{
		sprintf(s, "Ошибка! Не удалось загрузить библиотеку \"info.dll\"!");
		return 1;
	}
	DLLinfo = (importFunction)GetProcAddress(hinstLib, 
		"getScreenInformation");
	if (DLLinfo == NULL)
	{
		sprintf(s, "Ошибка! Функция \"getScreenInformation\" не найдена!");
		FreeLibrary(hinstLib);
		return 1;
	}
	/*получение информации о ширине экрана*/
	DLLinfo(screenInfo);

	DLLinfo = (importFunction)GetProcAddress(hinstLib,
		"getMMXInformation");
	if (DLLinfo == NULL)
	{
		sprintf(s, "Ошибка! Функция \"getMMXInformation\" не найдена!");
		FreeLibrary(hinstLib);
		return 1;
	}
	/*получение информации о поддержки MMX*/
	DLLinfo(mmxInfo);
	FreeLibrary(hinstLib);
	sprintf(s, "%s, %s", screenInfo, mmxInfo);
	return 0;
}
/*главная подпрограмма*/
int WINAPI WinMain(HINSTANCE hThisInst, HINSTANCE hPrevInst,
	LPSTR lpCmdLine, int nCmdShow)
{
	char szClassName[] = "MyClass";
	MSG msg;
	//элемент структуры класса окна
	WNDCLASS wcl;
	//дескриптор окна
	HWND hWnd; 
	/*дескриптор экземляра приложения, в котором
	находится процедура обработки*/
	wcl.hInstance = hThisInst;
	/*указатель на строку, содержащую
	имя класса*/
	wcl.lpszClassName = szClassName;
	/*указатель на процедуру обработки сообщений*/
	wcl.lpfnWndProc = WndProc;
	//стиль класса окна
	wcl.style = CS_HREDRAW | CS_VREDRAW;
	//дескриптор пиктограммы
	wcl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	//дескриптор курсора
	wcl.hCursor= LoadCursor(NULL, IDC_ARROW);
	//указатель на строку, содержащую имя меню
	wcl.lpszMenuName = NULL;
	//дополнительные параметры
	wcl.cbClsExtra = 0;
	//дополнительные параметры
	wcl.cbWndExtra = 0;
	//цвет фона окна
	wcl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	//регистрация класса
	RegisterClass(&wcl);
	//создание окна
	hWnd = CreateWindow(szClassName, 
		"Индивидуальное задание по УРВС",
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN |
		WS_CLIPSIBLINGS, 100, 50, 700, 120,
		HWND_DESKTOP, NULL, hThisInst, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	//цикл обработки сообщений
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.lParam;
}
/*процедура обработки сообщений*/
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, 
	WPARAM wParam, LPARAM lParam)
{
	HANDLE hThread;
	DWORD idThread;
	HDC hdc;
	PAINTSTRUCT ps;
	static char text[100];
	//обработка сообщений
	switch (message)
	{
	case WM_CREATE:
		//создание потока
		hThread = CreateThread(NULL, 0, ThreadProc,
			text, 0, &idThread);
		if (hThread == NULL)
		{
			sprintf(text, "Ошибка! Не удалось создать поток!");
			return GetLastError();
		}
		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		//вывод строки в окно
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



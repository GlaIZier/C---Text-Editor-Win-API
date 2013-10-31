#include <windows.h> // win api
#include <stdlib.h>  // работа с памятью
#include <string.h>  // работа с сишными строками
#include "declarations.h"

// generates manifest windows xp. Симпатичные кнопочки

#pragma comment(linker, \
  "\"/manifestdependency:type='Win32' "\
  "name='Microsoft.Windows.Common-Controls' "\
  "version='6.0.0.0' "\
  "processorArchitecture='*' "\
  "publicKeyToken='6595b64144ccf1df' "\
  "language='*'\"")

#pragma comment(lib, "ComCtl32.lib")
//*** закончили подключение
										
int WINAPI WinMain(HINSTANCE hInstance,   // __stdcall(WINAPI,CALLBACK, APIENTRY) точка входа
                   HINSTANCE hPrevInstance,  // дескриптор прошлого окна. Обычно нул. дескриптор уже запущенного окна(использовался в 16 бит)	
                   LPSTR lpCmdLine,          //  то же что и char* long pointer constant string, указывает на командную строку для прикладной программы
                   int nCmdShow)            // видимость окна, состояние окна

{   
	// создаем структуру класса окна. Содержит инфу о классе окна
	if (Reg_Class(hInstance)) { // моя функция  в func
        MessageBox(NULL,
            L"Call to RegisterClassEx failed!",
            L"Win32 Guided Tour",
            NULL);
        return 1;
    }

    if (Create_Components(hInstance,nCmdShow)) { // моя функция  в func 
        MessageBox(NULL,
           L"Call to CreateWindow failed!",
           L"Win32 Guided Tour",
           NULL);
		return 1;
     }

	// Main message loop:
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
		ProcessMessage(msg);
    }

    return (int) msg.wParam;
}











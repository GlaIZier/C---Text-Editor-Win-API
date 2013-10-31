#include <windows.h> // win api
#include <stdlib.h>  // ������ � �������
#include <string.h>  // ������ � ������� ��������
#include "declarations.h"

// generates manifest windows xp. ����������� ��������

#pragma comment(linker, \
  "\"/manifestdependency:type='Win32' "\
  "name='Microsoft.Windows.Common-Controls' "\
  "version='6.0.0.0' "\
  "processorArchitecture='*' "\
  "publicKeyToken='6595b64144ccf1df' "\
  "language='*'\"")

#pragma comment(lib, "ComCtl32.lib")
//*** ��������� �����������
										
int WINAPI WinMain(HINSTANCE hInstance,   // __stdcall(WINAPI,CALLBACK, APIENTRY) ����� �����
                   HINSTANCE hPrevInstance,  // ���������� �������� ����. ������ ���. ���������� ��� ����������� ����(������������� � 16 ���)	
                   LPSTR lpCmdLine,          //  �� �� ��� � char* long pointer constant string, ��������� �� ��������� ������ ��� ���������� ���������
                   int nCmdShow)            // ��������� ����, ��������� ����

{   
	// ������� ��������� ������ ����. �������� ���� � ������ ����
	if (Reg_Class(hInstance)) { // ��� �������  � func
        MessageBox(NULL,
            L"Call to RegisterClassEx failed!",
            L"Win32 Guided Tour",
            NULL);
        return 1;
    }

    if (Create_Components(hInstance,nCmdShow)) { // ��� �������  � func 
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











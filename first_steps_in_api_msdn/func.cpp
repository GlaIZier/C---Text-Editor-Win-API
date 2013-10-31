#include <windows.h>

#include "stdio.h"
#include "declarations.h"
#include "resource.h"
#include <string>
// Variables
// main window class name
static wchar_t szWindowClass[] = L"win32app";
// string in tittle bar
static wchar_t szTitle[] = L"Simple Notepad";
// id icon
static wchar_t szIconName[]= L"IDI_ICON1"; 

 HWND hEdit;
 HWND hWnd;
 HACCEL hAccel;
 wchar_t szTitleSR[100];

 HWND hDlg;
 HINSTANCE hInstanceDialog;
 COLORREF EditColor = RGB(255,255,255);		// цвет edit
 COLORREF TextColor = RGB(0,0,0);
 int WindowTransparent;
 HFONT hFont;
 int FontWeight = -8;

// Declare functions
// Catch messages
LRESULT __stdcall WndProc(HWND, UINT, WPARAM, LPARAM); //  реакци€ на сообщени€ от ќ—, котора€ сама обрабатывает событи€                  
														   //- позвол€ет операционной системе общатьс€ с окном и передавать свойства окна ќ—, hwnd дескриптор окна(номер), 
                                                       // HWND hwnd;      // окно, которое получит это сообщение
                                                       // UINT message;   // код сообщени€
                                                       // WPARAM wParam;  // параметр, специальна€ инфа сообщени€
                                                       // LPARAM lParam;  // параметр, специальна€ инфа сообщени€
                                                       // DWORD time;     // врем€, когда произошло сообщение
                                                       // POINT pt;       // координаты курсора мыши

INT_PTR CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);  // обработка сообщений от диалога

void file_save();

bool Reg_Class(HINSTANCE hInstance) // возвращает 0 в хорошем случае
{
	WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, szIconName);
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCE(IDR_MENU1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	return !(RegisterClassEx(&wcex)); // != 0);   // 0 в хорошем случае
}

 bool Create_Components(HINSTANCE hInstance,int nCmdShow) // возвращает 0 в хорошем случае
{   
	hAccel = LoadAccelerators (hInstance, MAKEINTRESOURCE (IDR_ACCELERATOR1));

	LoadString(hInstance, IDS_STRING103, szTitleSR, sizeof(szTitleSR)/sizeof(wchar_t));
	
	// будем выволить в центр экрана
	// вычисление координат центра экрана
	RECT screen_rect = {0};
    GetWindowRect(GetDesktopWindow(),&screen_rect); // разрешение экрана
    int x = screen_rect.right / 2 - 250; 
    int y = screen_rect.bottom / 2 - 150;

	hWnd = CreateWindow(
                szWindowClass,
                szTitleSR,
                WS_OVERLAPPEDWINDOW,
                x, y,
                500, 250,
                NULL,
                NULL,
                hInstance,
                NULL
                );

	// custom resource 
	HRSRC hRsrc = FindResource(hInstance, MAKEINTRESOURCE(IDR_CUSTOM1), L"custom");
	HGLOBAL hGlobal = LoadResource( hInstance, hRsrc );   // загружаем ресурс в пам€ть
	int SizeOfResource = SizeofResource( hInstance, hRsrc );  // определ€ем размер
	wchar_t* EditText = (wchar_t*)( LockResource( hGlobal ) ); // блокируем

	// перевод из ansi в unicode. —охранил в юникоде текстовый документ 
	/*
	char* Buffer = new wchar_t[SizeOfResource + 1];
	memset( Buffer, '\0', SizeOfResource + 1 );
	memcpy( Buffer, EditFirstText, SizeOfResource);
	char* EditText[255];
	MultiByteToWideChar( CP_ACP, 0, (LPCSTR)Buffer, -1, EditText, 255);
	//*** закончили перевод
	*/

	hEdit = CreateWindow(L"Edit", EditText,
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_MULTILINE,
			0,0, 500, 250, hWnd, NULL, 
			hInstance, NULL); // multiline перенос строк
    // устанавливаем шрифт по умолчанию самый маленький
	LOGFONT LF={FontWeight, 0, 0, 0, FW_HEAVY, 0, 0, 0, RUSSIAN_CHARSET,
							OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DRAFT_QUALITY, 0, L"Arial"};
				hFont = CreateFontIndirect(&LF);
				SendMessage(hEdit,WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE,0));

	ShowWindow(hWnd,
              nCmdShow);
    UpdateWindow(hWnd);

	return !hWnd;  // возвращаем 0, если все хорошо
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps; //
    HDC hdc; // дескриптор устройства, куда рисовать
    TCHAR greeting[] = L"Hello, World!";
	RECT wnd_rect = {0}; // структура с новыми координатами окна 

	
    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;
	case WM_SIZE:
		GetClientRect(hWnd, &wnd_rect);
		SetWindowPos( hEdit,0, 0, 0, wnd_rect.right-wnd_rect.left,wnd_rect.bottom-wnd_rect.top, SWP_DRAWFRAME );
		break;

    case WM_DESTROY:
        PostQuitMessage(0);
		DeleteObject(hFont);
		DeleteObject(hWnd);
		DeleteObject(hEdit);
		DeleteObject(hDlg);
		DeleteObject(hAccel);
        break;
	case WM_CTLCOLOREDIT:
		if((HWND)lParam == hEdit) {
			//SetTextColor((HDC)wParam, RGB(255, 0, 0));
			SetBkColor((HDC)wParam, EditColor);
			SetTextColor((HDC)wParam, TextColor);
			//return (BOOL)CreateSolidBrush(RGB(0, 0, 255)); // !!!!
			}
		break;

	// menu processing
	case WM_COMMAND:
        switch(LOWORD(wParam))
        {
            case ID_FILE_EXIT:
                PostMessage(hWnd, WM_DESTROY, 0, 0);
            break;
            case ID__SAVE:
				file_save();
            break;
			case ID_ACCELERATOR40003:
				file_save();
			break;
			case ID_ACCELERATOR40004:
				PostMessage(hWnd, WM_DESTROY, 0, 0);
			break;
			case ID_FILE_SETTINGS:
				//ShowWindow(hDlg, SW_SHOW);
				DialogBox(hInstanceDialog, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, DialogProc);
				//MessageBox(hWnd, L"Settings", NULL, MB_OK);
			break;
        }
    break;
	
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    return 0;
}

void file_save()
{
	OPENFILENAME saveWin; // structure for saving file
	ZeroMemory( &saveWin, sizeof( saveWin ) );
	wchar_t szFile[260];  // string for name of file

	// implementation of dialog structure
	saveWin.lpstrFile = szFile;
	saveWin.nMaxFile = sizeof( szFile );
	saveWin.lpstrFile[0] = '\0';
	saveWin.lStructSize = sizeof( saveWin );
	saveWin.hwndOwner = 0;
	saveWin.lpstrFilter = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	saveWin.lpstrDefExt = (LPCWSTR)L"doc";
	saveWin.nFilterIndex = 1;
	saveWin.lpstrFileTitle = NULL;
	saveWin.nMaxFileTitle = 0;
	saveWin.lpstrInitialDir = NULL;
	saveWin.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
	////////////////////////////////////

	if (GetSaveFileName(&saveWin) == true)  {
		HANDLE hFile = CreateFile( saveWin.lpstrFile, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0 );
		wchar_t buffer[300]; 
		if (GetWindowText(hEdit, buffer, 300)>0) {
			DWORD dwBytesWritten = 0; // число записанных байт
			WriteFile(hFile, buffer, (wcslen(buffer))*sizeof(wchar_t),&dwBytesWritten,NULL);
		}
		CloseHandle(hFile);
	}
}

void ProcessMessage(MSG& mymsg)
{ if (!TranslateAccelerator(hWnd, hAccel, &mymsg)) {
		TranslateMessage(&mymsg);
		DispatchMessage(&mymsg);
	}
}


//********************** dialog**********************

INT_PTR CALLBACK DialogProc(HWND hDlg, UINT dMessage, WPARAM wParam, LPARAM lParam)
{
	// »нициализаци€ CHOOSECOLOR  
	static CHOOSECOLOR colorStruct;
	static COLORREF colorArray[16];	// массив пользовательских цветов
	// слайдеры 
	static HWND hSlider1;
	static HWND hSlider2;

	double x = 99.99; // дл€ просмотра преобразовани€ 
	switch (dMessage)
	{
			
		/*case WM_DESTROY:
			DestroyWindow(hDlg);
			return true;
			*/
		case WM_INITDIALOG:
					//»нициализируем станд окно выбора цвета
					//ZeroMemory(&colorStruct, sizeof(CHOOSECOLOR));
					colorStruct.lStructSize = sizeof(CHOOSECOLOR);
					colorStruct.hwndOwner = hDlg;
					colorStruct.lpCustColors = (LPDWORD)colorArray;
					colorStruct.Flags = CC_FULLOPEN || CC_RGBINIT;
					
					// слайдеры
					hSlider1 = GetDlgItem(hDlg, IDC_SLIDER1);
					hSlider2 = GetDlgItem(hDlg, IDC_SLIDER2);
					//SendDlgItemMessage(hdlg, IDC_SLIDER, TBM_SETRANGE, (WPARAM)1, (LPARAM)MAKELONG(1,10));
					//SetScrollRange(hDlg, IDC_SLIDER1, 0, 255, TRUE);
					//SetScrollRange(hDlg, IDC_SLIDER2, 0, 255, TRUE);
					//SetScrollPos(hDlg, IDC_SLIDER1, 127, TRUE);
					return true;
		case WM_HSCROLL: // прокрутка слайдеров
			if (hSlider1 == (HWND)lParam) {          // добираемс€ сначала до нужного слайдера. ” мен€ их два. lParam приводим к типу HWND 
				int SliderPosition1 = (int)HIWORD(wParam);
				switch (LOWORD(wParam)) {
					case SB_PAGELEFT:
						//WindowTransparent = SliderPosition; 
						WindowTransparent -= 51;
						break;
					case SB_PAGERIGHT:
						wchar_t buffer_w[10];
						_itow(SliderPosition1, buffer_w, 10);
						MessageBox(hDlg, buffer_w, NULL, MB_OK);  // дл€ себ€. ѕеревод int и double в wchar_t 

						swprintf(buffer_w, L"%g", x);
						MessageBox(hDlg, buffer_w, NULL, MB_OK);  // дл€ себ€. ѕеревод int и double в wchar_t 
						WindowTransparent += 51;
						break;
					case SB_THUMBPOSITION:
						
						WindowTransparent = SliderPosition1; 
						int(WindowTransparent *= 2.55);
						break;
					case SB_THUMBTRACK:
						 WindowTransparent = SliderPosition1; 
						 int(WindowTransparent *= 2.55);
						break;
				}
				SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);	
				SetLayeredWindowAttributes(hWnd,
                            NULL, // цвет прозрачности
                            255 - WindowTransparent,              // величина прозрачности
                            LWA_ALPHA);
				}
			else if (GetDlgItem(hDlg, IDC_SLIDER2) == (HWND)lParam) {
				int SliderPosition2 = (int)HIWORD(wParam);
				switch (LOWORD(wParam)) {
					case SB_PAGELEFT:
						//WindowTransparent = SliderPosition; 
						int(FontWeight -= -14.4);
						break;
					case SB_PAGERIGHT:
						//WindowTransparent = SliderPosition;
						int(FontWeight += -14.4);
						break;
					case SB_THUMBPOSITION: 
						if (SliderPosition2 <= 11) { FontWeight = -8; }
						else { FontWeight = int(-SliderPosition2*0.72); }
						break;
					case SB_THUMBTRACK:
						if (SliderPosition2 <= 11) { FontWeight = -8; }
						else { FontWeight = int(-SliderPosition2*0.72); }
						break;
				}
				LOGFONT LF={FontWeight, 0, 0, 0, FW_HEAVY, 0, 0, 0, RUSSIAN_CHARSET,
							OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DRAFT_QUALITY, 0, L"Arial"};
				hFont = CreateFontIndirect(&LF);
				SendMessage(hEdit,WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE,0));
			}
			break;
		case WM_COMMAND:
			switch(LOWORD(wParam)) {
				case IDC_BUTTON3:		
					if (ChooseColor(&colorStruct) == true) {
						EditColor = colorStruct.rgbResult; 
					}
					break;
				case IDC_BUTTON2:
					if (ChooseColor(&colorStruct) == true) {
						TextColor = colorStruct.rgbResult; 
					}
					break;
				case IDOK:
					SendMessage(hWnd,WM_CTLCOLOREDIT,0,0);
					EndDialog(hDlg,0);
					SetFocus(hEdit);
					break;
				case IDCANCEL:
					EndDialog(hDlg,0);
					SetFocus(hEdit);
					break;
			}
			break;
	 }	
	return false;
}



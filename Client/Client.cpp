﻿#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "WinSock2.h" 
#include <ws2tcpip.h> 
#pragma comment(lib, "Ws2_32.lib") 
#include "Windows.h"
#include "tchar.h"

#include "Resource.h"

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

using namespace std;

HWND hRead, hWrite, hConnect, hSend, hialog;



void InitEdit(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    hialog = hDlg;
    hRead = GetDlgItem(hDlg, IDC_EDIT1);
    hWrite = GetDlgItem(hDlg, IDC_EDIT2);

    hConnect = GetDlgItem(hDlg, IDC_BUTTON1);
    hSend = GetDlgItem(hDlg, IDC_BUTTON2);



}

LRESULT CALLBACK DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    const int MAXSTRLEN = 255;
    WSADATA wsaData;
    SOCKET _socket; 
    sockaddr_in addr;

    switch (uMsg)
    {
    case WM_INITDIALOG:
        InitEdit(hDlg, uMsg, wParam, lParam);
        break;

    case WM_CLOSE:
        EndDialog(hDlg, NULL);
        break;
    case WM_COMMAND:
    {

		if ((wParam) == IDC_BUTTON2)
		{

			WSAStartup(MAKEWORD(2, 2), &wsaData);

			_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

			addr.sin_family = AF_INET;

			inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
			addr.sin_port = htons(20000);
			connect(_socket, (SOCKADDR*)&addr, sizeof(addr));

            char buf[MAXSTRLEN];
            int i = recv(_socket, buf, MAXSTRLEN - 1, 0);

            if (i == SOCKET_ERROR) {
                // Обработка ошибки при приеме данных
                MessageBox(hDlg, L"Failed to receive data", L"Error", MB_OK | MB_ICONERROR);
            }
            else if (i == 0) {
                MessageBox(hDlg, L"Connection closed by the server", L"Info", MB_OK | MB_ICONINFORMATION);
            }
            else {
                buf[i] = '\0';
                SetWindowTextA(hWrite, buf);
            }
            closesocket(_socket);
            WSACleanup();
		}
    }
    break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    }
    return 0;
}

int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrev, LPTSTR lpszCmdLine, int nCmdShow)
{
    return DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
}
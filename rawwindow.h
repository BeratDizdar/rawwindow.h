/*
  rawwindow.h - Single Header Raw Window Handling Library
  
  Copyright (c) 2026 BeratDizdar

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#ifndef RAWWINDOW_H
#define RAWWINDOW_H

// win32 libs: -mwindows -lgdi32 -luser32 -lkernel32

int rawCreateWindow(const char* title, int w, int h, char exit_key);
void rawMainloop(void(*update)(float), void(*draw)());

// BUNDAN HENUZ EMIN DEGILIM -> HWND rawGetHandler();

#endif
#ifdef RAWWINDOW_IMPL
#include <windows.h>

// TEHLIKELI OLABILIR //
static char __exit_key = 0;
static HWND __hwnd = NULL; 
static int  __running = 0;
// ================== //

LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch(msg) {
	case WM_CREATE: break;
	case WM_CLOSE: DestroyWindow(hWnd); break;
	case WM_DESTROY: PostQuitMessage(0); break;
	case WM_SIZE: break;
		//case WM_PAINT: break;
	default: return DefWindowProc(hWnd, msg, wp, lp);
	}
	return 0;
}

int rawCreateWindow(const char* title, int w, int h, char exit_key) {
	HINSTANCE instance = GetModuleHandle(NULL);
	WNDCLASSA wc = {0};
	wc.lpfnWndProc   = WinProc;
    wc.hInstance     = instance;
    wc.lpszClassName = "WINDOW1";
	wc.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(52, 78, 78));
	wc.hCursor       = LoadCursor(instance, IDC_ARROW);
	wc.style         = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);

	RECT r = {0, 0, w, h};
    AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, FALSE);
    
	__hwnd = CreateWindowA(wc.lpszClassName, title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, r.right - r.left, r.bottom - r.top, 0, 0, wc.hInstance, 0);
	if (__hwnd == NULL) return 0;
	ShowWindow(__hwnd, SW_SHOW);
	UpdateWindow(__hwnd);

	__exit_key = (char)exit_key;
	__running = 1;
}


void rawMainloop(void(*update)(float), void(*draw)()) {
	MSG msg = {0};

	while (__running) {
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) __running = 0;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

		// EXIT KEY
	    if (GetAsyncKeyState(__exit_key)) break;
		if (!__running) break;

		update(0.016f);
		draw();

	}
}

#endif

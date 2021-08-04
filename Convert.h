//
// Created by User on 027 27.03.21.
//

#ifndef NUMCONVERTER_CONVERT_H
#define NUMCONVERTER_CONVERT_H

#pragma once
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <vector>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <malloc.h>

#pragma warning(disable : 4996)

// The main window class name.
static TCHAR szWindowClass[] = _T("DesktopApp");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("Converter");

//HINSTANCE hInst;
//
//// Forward declarations of functions included in this code module:
//LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

class Converter {
    enum class CTL_ID {
        BUTTON_ID,
        NUMFIELD_ID,
        SYSFIELD_ID
    };
public:
    explicit Converter();
    ~Converter();
    int Run();
    void print(wchar_t arr[]);
    int WideToInt(wchar_t arr[]);
    int miniPow(int a, int b);
    void transformation(wchar_t* text_num, int kf, wchar_t*& res);
    wchar_t converting_num(int a);
    void push_back(wchar_t*& arr, wchar_t num);
    void reverse(wchar_t*& temp);
private:
    void init_new_window();
    void init_controls();

    static LRESULT CALLBACK application_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);



private:
    HWND text_field_1, text_field_2, text_field_3, calc_button, hWnd;
    const int x_size = 390, y_size = 180;
    wchar_t *res;
};

#endif //NUMCONVERTER_CONVERT_H

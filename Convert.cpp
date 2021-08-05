#include "Convert.h"

class exception;

inline Converter::Converter() {
    res = new wchar_t[0];
    try {
        this->init_new_window();
        this->init_controls();
    }
    catch (const std::exception& e) {
        std::string expt_data = e.what();
        //MessageBox(nullptr, std::wstring(begin(expt_data), end(expt_data)).c_str(), _T("Error"), MB_ICONERROR | MB_OK);
        ExitProcess(EXIT_FAILURE);
    }
}

inline Converter::~Converter() {
    delete[]res;
}

inline int Converter::Run() {
    ShowWindow(this->hWnd, SW_SHOWDEFAULT);
    UpdateWindow(this->hWnd);
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return static_cast<int>(msg.wParam);
}

inline void Converter::init_new_window() {
    /* CreateConsole();
 printf("Yohooo!\n"); */

    AllocConsole();
    std::freopen("CONOUT$", "wt", stdout);
    std::freopen("CONIN$", "rt", stdin);


    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = Converter::application_proc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = GetModuleHandle(nullptr);
    wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
    {
       /* MessageBox(NULL,
                   _T("Call to RegisterClassEx failed!"),
                   _T("Windows Desktop Guided Tour"),
                   NULL); */

        throw std::runtime_error("Error, can't register main window class!");
    }

    /* hInst = hInstance;*/

    HWND hWnd = CreateWindow(
            szWindowClass,
            szTitle,
            WS_OVERLAPPEDWINDOW & ~(WS_SIZEBOX | WS_MAXIMIZEBOX),
            CW_USEDEFAULT, CW_USEDEFAULT,
            x_size, y_size,
            NULL,
            NULL,
            NULL,
            this
    );

    if (!hWnd)
    {
      /*  MessageBox(NULL,
                   _T("Call to CreateWindow failed!"),
                   _T("Windows Desktop Guided Tour"),
                   NULL);*/

        throw std::runtime_error("Error, can't create main Window");
    }
    //return (int)msg.wParam;

}

inline void Converter::init_controls() {
    //RECT DialogRect;
    //GetClientRect(hWnd, &DialogRect);
    //int LEFT = (DialogRect.right - 100) / 2;
    //int TOP = (DialogRect.bottom - 25) / 2;

    //Convert button
    this->calc_button = CreateWindowExA(0, _T("BUTTON"), _T("Convert"), WS_VISIBLE | WS_CHILD | WS_BORDER, 142, 105, 105,
                                       30, hWnd, reinterpret_cast<HMENU>(Converter::CTL_ID::BUTTON_ID), nullptr,
                                       nullptr);
    if (this->calc_button);
    else if (!this->calc_button)throw std::runtime_error("Error! Can't create button");

    //Field to enter num to convert
    this->text_field_1 = CreateWindowExA(WS_EX_CLIENTEDGE, _T("EDIT"), _T("0"),
                                        WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_NUMBER , 76, 26, 276, 25, hWnd, reinterpret_cast <HMENU> (Converter::CTL_ID::NUMFIELD_ID), nullptr, nullptr);

    if (this->text_field_1);
    else if (!this->text_field_1) throw std::runtime_error("Error! Can't create field 1");

    //Field to enter system num
    this->text_field_2 = CreateWindowExA(WS_EX_CLIENTEDGE, _T("EDIT"), _T("0"),
                                        WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL , 37, 26, 30, 25, hWnd,
                                        reinterpret_cast<HMENU>(Converter::CTL_ID::SYSFIELD_ID), nullptr, nullptr);
    if (this->text_field_2);
    else if (!this->text_field_2) throw std::runtime_error("Error! Can't create field 2");

    SendMessage(this->text_field_2, EM_SETLIMITTEXT, 2, 0);

    //Field to output result
    this->text_field_3 = CreateWindowExA(WS_EX_CLIENTEDGE, _T("EDIT"), _T("your result"),
                                        WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | WM_SETTEXT , 37, 63, 315, 25, hWnd,
                                        reinterpret_cast<HMENU>(Converter::CTL_ID::SYSFIELD_ID), nullptr, nullptr);
    if (this->text_field_3);
    else if (!this->text_field_3) throw std::runtime_error("Error! Can't create field 3");
    //you can add your font there
}

inline LRESULT Converter::application_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    Converter* pApp;
    if (message == WM_NCCREATE) {
        pApp = static_cast<Converter*> (reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
        SetLastError(0);
        if (!SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pApp))) {
            if (GetLastError() != 0)
                return false;
        }
    }
    else {
        pApp = reinterpret_cast<Converter*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }
    if (pApp) {
        pApp->hWnd = hWnd;
        return pApp->WndProc(hWnd, message, wParam, lParam);
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

inline char Converter::IntToChar(int num) {
    char symb;
    if (num >= 0 && num <= 9) {
        symb = num | 0x30;
    }
    else {
        switch (num)
        {
            case 10: {
                symb = 'A';
                break;
            }case 11: {
                symb = 'B';
                break;
            }case 12: {
                symb = 'C';
                break;
            }case 13: {
                symb = 'D';
                break;
            }case 14: {
                symb = 'E';
                break;
            }case 15: {
                symb = 'F';
                break;
            }case 16: {
                symb = 'G';
                break;
            }
            default:
                symb = 'x';
                break;
        }
    }
    return symb;
}

inline TCHAR* Converter::StrToChar(std::string str) {
    int size = str.size();
    TCHAR* result = new TCHAR[size];
    for ( int i = 0; i < size; i++) {
        result[i] = str[i];
    }
    return result;
}

inline TCHAR* Converter::transform(TCHAR* system, TCHAR* number) {

    int num = _ttoi(number);

    int sys = _ttoi(system);

    std::string result;

    while (num >= sys ) {
        int rem = num % sys;
        num /= sys;
        result.push_back (IntToChar(rem));
    }
    result.push_back(IntToChar(num % sys));
    std::reverse (result.begin(), result.end());
    std::cout << result << "\n";

    return StrToChar(result);
}

inline LRESULT Converter::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

    switch (message)
    {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc;
            hdc = BeginPaint(hWnd, &ps);
            hdc = GetDC(hWnd);
            /*TextOut(hdc, 12, 27, "Num: ", 5);
            TextOut(hdc, 12, 67, "Sys: ", 5);*/
            ReleaseDC(hWnd, hdc);
            EndPaint(hWnd, &ps);
            break;
        }
        case WM_COMMAND: {
            switch(static_cast<Converter::CTL_ID>(LOWORD(wParam))) {
                case(Converter::CTL_ID::BUTTON_ID):
                try {
                    //wchar_t* text_num =  new wchar_t[GetWindowTextLength(this->text_field_1)];
                    //wchar_t* text_sys =  new wchar_t[GetWindowTextLength(this->text_field_2)];

                    TCHAR* text_num = new TCHAR[GetWindowTextLength(this->text_field_1) + 1];
                    TCHAR* text_sys = new TCHAR[GetWindowTextLength(this->text_field_2) + 1];

                        GetWindowText(text_field_1, text_num,
                                      GetWindowTextLength(this->text_field_1) + 1);
                        GetWindowText(text_field_2, text_sys,
                                      GetWindowTextLength(this->text_field_2) + 1);


                        SetWindowText(this->text_field_3, (transform(text_sys, text_num)) );           // output command

                    delete[]text_num;
                    delete[]text_sys;
                }
                    catch (...) {
                        MessageBox(hWnd, _T("Error of input"), _T("Warning!"), MB_ICONINFORMATION | MB_OK);
                    }
                break;
            }
        break;
        }
        return 0;
        case WM_DESTROY: {
            PostQuitMessage(EXIT_SUCCESS);
            break;
        }
            return 0;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}



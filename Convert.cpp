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
                                        WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL , 76, 26, 276, 25, hWnd, reinterpret_cast <HMENU> (Converter::CTL_ID::NUMFIELD_ID), nullptr, nullptr);
    /*this->text_field_1 = CreateWindowEx(WS_EX_CLIENTEDGE, _T("EDIT"), _T("0"),
                                        WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | WM_SETFONT, 76, 26, 276,
                                        25, hWnd, reinterpret_cast<HMENU>(Converter::CTL_ID::NUMFIELD_ID), nullptr,
                                        nullptr);*/

    if (this->text_field_1);
    else if (!this->text_field_1) throw std::runtime_error("Error! Can't create field 1");

    //Field to enter system num
    this->text_field_2 = CreateWindowExA(WS_EX_CLIENTEDGE, _T("EDIT"), _T("0"),
                                        WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 37, 26, 30, 25, hWnd,
                                        reinterpret_cast<HMENU>(Converter::CTL_ID::SYSFIELD_ID), nullptr, nullptr);
    if (this->text_field_2);
    else if (!this->text_field_2) throw std::runtime_error("Error! Can't create field 2");

    //Field to output result
    this->text_field_3 = CreateWindowExA(WS_EX_CLIENTEDGE, _T("EDIT"), _T("your result"),
                                        WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 37, 63, 315, 25, hWnd,
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

inline void Converter::print(wchar_t arr[]) {
    int size = _msize(arr) / sizeof (wchar_t);
    for (int i = 0; i < size ; ++i){
        std::wcout << arr[i];
    }
    std::cout << "\n";
}

inline int Converter::miniPow(int a, int b)
{
    int r = 1;
    for (int i = 0; i < b; i++)
    {
        r *= a;
    }
    return r;
}

inline int Converter::WideToInt(wchar_t arr[])
{
    int i = 0;
    int len = wcslen(arr);
    int coefficient = 0;
    int RawNumber = 0;
    int Number = 0;

    for (int k = 0; k < len; k++)
    {
        coefficient = miniPow(10, len - 1 - k);
        RawNumber = (int)arr[k];
        Number = RawNumber - 48;
        i += coefficient * Number;
    }
    return i;
}

inline wchar_t Converter::converting_num(int a) {
    wchar_t res;
    if (a >= 0 && a <= 9) {
        res = a | 0x30;
    }
    else {
        switch (a)
        {
            case 10: {
                res = 'A';
                break;
            }case 11: {
                res = 'B';
                break;
            }case 12: {
                res = 'C';
                break;
            }case 13: {
                res = 'D';
                break;
            }case 14: {
                res = 'E';
                break;
            }case 15: {
                res = 'F';
                break;
            }case 16: {
                res = 'G';
                break;
            }
            default:
                res = 'q';
                break;
        }
    }
    return res;
}

inline void Converter::push_back(wchar_t*& arr, wchar_t num) {
    int size = _msize(arr) / sizeof(wchar_t);
    wchar_t* newArr = new wchar_t[size + 1];
    for (int i = 0; i < size; ++i) {
        newArr[i] = arr[i];
    }
    newArr[size] = num;
    delete[]arr;
    arr = newArr;
}

inline void Converter::reverse(wchar_t*& temp) {
    int size = _msize(temp) / sizeof(wchar_t);
    wchar_t* newArr = new wchar_t[size];
    for (int i = 0; i < size; ++i) {
        newArr[i] = temp[size - i - 1];
    }
    delete[]temp;
    temp = newArr;
}

inline void Converter::transformation(wchar_t* text_num, int kf, wchar_t*& res) {
    wchar_t* temp = new wchar_t[0];
    size_t len = _msize(res) / sizeof(wchar_t);
    double num = _wtof(text_num);
    int num_whole = num / 1;
    double num_fractional = num - num_whole;
    while (num_whole >= kf) {
        push_back(temp, converting_num(num_whole % kf));
        num_whole /= kf;
    }
    push_back(temp, converting_num(num_whole % kf));
    for (size_t i = 0; i < len / 2; i++)
    {
        wchar_t tmp = temp[i];
        temp[i] = temp[len - i - 1];
        temp[len - i - 1] = tmp;
    }
    reverse(temp);
    if (num_fractional > 0) {
        push_back(temp, '.');
        int counter = 0;
        while (num_fractional != 0 && counter <= 5) {
            num_fractional *= kf;
            int temp_num = num_fractional / 1;
            push_back(temp, converting_num(temp_num));
            num_fractional -= temp_num;
            ++counter;
        }
    }
    delete[]res;
    res = temp;
}

inline LRESULT Converter::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    wchar_t* text_num = new wchar_t[GetWindowTextLength(text_field_1) + 1];
    wchar_t* text_sys = new wchar_t[GetWindowTextLength(text_field_2) + 1];

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
                        GetWindowText(text_field_1, reinterpret_cast<LPSTR>(text_num),
                                      _msize(text_num) / sizeof(wchar_t));
                        GetWindowText(text_field_2, reinterpret_cast<LPSTR>(text_sys),
                                      _msize(text_sys) / sizeof(wchar_t));
                        /*MessageBox(hWnd, reinterpret_cast<LPCSTR>(text_num), reinterpret_cast<LPSTR>(text_sys), MB_ICONINFORMATION | MB_OK);
                        Converter::print(text_sys);
                        std::cout <<  " " << sys;*/
                        //Converter::transformation(text_num, Converter::WideToInt(text_sys), res);
                        int sys = Converter::WideToInt(text_sys);
                        sys++;
                        std::cout << sys;
                        SetWindowTextA(this->text_field_3, reinterpret_cast<LPSTR>(sys));
                        MessageBox(hWnd, reinterpret_cast<LPSTR>(sys), _T("RESULT"), MB_ICONINFORMATION | MB_OK);
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
    delete[]text_num;
    delete[]text_sys;
    return DefWindowProc(hWnd, message, wParam, lParam);
}
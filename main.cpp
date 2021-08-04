#include "Convert.cpp"

int CALLBACK WinMain(               //it is correct function calling, check other errors
        _In_ HINSTANCE hInstance,
        _In_opt_ HINSTANCE hPrevInstance,
        _In_ LPSTR     lpCmdLine,
        _In_ int       nCmdShow
)  {

Converter app{};

app.Run();

return 0;

};
#define _USE_MATH_DEFINES 
#include<Windows.h>
#include"Graphics.h"

using namespace std;



const int bits = 32; 
bool full_screen_ = false;
HWND hwnd;
DWORD last_style;
RECT last_rect;
HDC screen_hdc;
HDC hCompatibleDC;
HBITMAP hCompatibleBitmap;
HBITMAP hOldBitmap;
BITMAPINFO binfo;
HFONT nowFont;

MiniColor BUFFER[SCREEN_HEIGHT * SCREEN_WIDTH];

double DecToRad(double ang)
{
    return ang * M_PI / 180.0;
}


void RestoreFullScreen() {
    SetWindowLong(hwnd, GWL_STYLE, last_style); // 恢复最大化之前的窗口风格
    SetWindowPos(hwnd, NULL, last_rect.left, last_rect.top, last_rect.right - last_rect.left, last_rect.bottom - last_rect.top, NULL); // 恢复最大化之前的窗口位置和大小
    full_screen_ = false;
}

void FullScreen() {
    last_style = GetWindowLong(hwnd, GWL_STYLE); //存储上次的窗口风格
    GetWindowRect(hwnd, &last_rect);             //存储上次的窗口位置和大小
    int w = GetSystemMetrics(SM_CXSCREEN);
    int h = GetSystemMetrics(SM_CYSCREEN);       // 获取最大化的窗口大小
    SetWindowLongPtr(hwnd, GWL_STYLE, WS_VISIBLE | WS_POPUP); // 去掉标题栏
    SetWindowPos(hwnd, NULL, 0, 0, w, h, SWP_FRAMECHANGED); // 设置位置和大小
    full_screen_ = true;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_LBUTTONDOWN:
        break;
    case WM_PAINT:
        break;
    case WM_CLOSE:
        DestroyWindow(hwnd);  //销毁窗口
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_KEYDOWN:
        if ('F' == wParam) {
            if (full_screen_) {
                RestoreFullScreen();
            }
            else {
                FullScreen();
            }
        }

        if (wParam == VK_ESCAPE) {
            exit(0);
        }
        break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

/// <summary>
/// 初始化并显示窗口
/// </summary>
void showWindow()
{
    for (int i = 0; i < SCREEN_HEIGHT; i++)
    {
        for (int j = 0; j < SCREEN_WIDTH; j++)
        {
            BUFFER[i * SCREEN_WIDTH + j].r = 0;
            BUFFER[i * SCREEN_WIDTH + j].g = 0;
            BUFFER[i * SCREEN_WIDTH + j].b = 0;

        }
    }

    HINSTANCE hInstance = GetModuleHandle(0);
    WNDCLASS wndcls;
    wndcls.cbClsExtra = 0;
    wndcls.cbWndExtra = 0;
    wndcls.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndcls.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndcls.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndcls.hInstance = hInstance;
    wndcls.lpfnWndProc = WndProc;
    wndcls.lpszClassName = L"miniEngine2D";
    wndcls.lpszMenuName = NULL;
    wndcls.style = CS_HREDRAW | CS_VREDRAW;

    RegisterClass(&wndcls);
    hwnd = CreateWindow(L"miniEngine2D", L"miniEngine2D", WS_OVERLAPPEDWINDOW,
        0, 0, 1280, 720, NULL, NULL, hInstance, NULL);
    ShowWindow(hwnd, SW_SHOWNORMAL);
    UpdateWindow(hwnd);

    ZeroMemory(&binfo, sizeof(BITMAPINFO));
    binfo.bmiHeader.biBitCount = bits;
    binfo.bmiHeader.biCompression = BI_RGB;
    binfo.bmiHeader.biHeight = -SCREEN_HEIGHT;
    binfo.bmiHeader.biPlanes = 1;
    binfo.bmiHeader.biSizeImage = 0;
    binfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    binfo.bmiHeader.biWidth = SCREEN_WIDTH;

    screen_hdc = GetDC(hwnd);
    hCompatibleDC = CreateCompatibleDC(screen_hdc);
    hCompatibleBitmap = CreateCompatibleBitmap(screen_hdc, SCREEN_WIDTH, SCREEN_HEIGHT);
    hOldBitmap = (HBITMAP)SelectObject(hCompatibleDC, hCompatibleBitmap);


}

void update()
{
    MSG msg;
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    putBufferToScreen();
}

void putBufferToScreen()
{
    SetDIBits(screen_hdc, hCompatibleBitmap, 0, SCREEN_HEIGHT, BUFFER, (BITMAPINFO*)&binfo, DIB_RGB_COLORS);
    BitBlt(screen_hdc, -1, -1, SCREEN_WIDTH, SCREEN_HEIGHT, hCompatibleDC, 0, 0, SRCCOPY);
}

void clearScreen()
{

    for (int i = 0; i < SCREEN_HEIGHT; i++)
    {
        for (int j = 0; j < SCREEN_WIDTH; j++)
        {
            //测试颜色:YANAGIZOME
            double nowR = (double)i /(double) SCREEN_HEIGHT*255;
            BUFFER[i * SCREEN_WIDTH + j].r = nowR;
            BUFFER[i * SCREEN_WIDTH + j].g = 173;
            BUFFER[i * SCREEN_WIDTH + j].b = 112;
            BUFFER[i * SCREEN_WIDTH + j].a = 255;

        }
    }
}

void drawPoint(int x, int y, const MiniColor&c)
{
    if (x<0 || x>SCREEN_WIDTH || y<0 || y>SCREEN_HEIGHT)return;
    double _a = static_cast<double>(c.a) / 255.0;
    double a = 1 - _a;
    BUFFER[y * SCREEN_WIDTH + x].r = BUFFER[y * SCREEN_WIDTH + x].r * a + c.r * _a;
    BUFFER[y * SCREEN_WIDTH + x].g = BUFFER[y * SCREEN_WIDTH + x].g * a + c.g * _a;
    BUFFER[y * SCREEN_WIDTH + x].b = BUFFER[y * SCREEN_WIDTH + x].b * a + c.b * _a;
}

void drawLine(int x0, int y0, int x1, int y1, const MiniColor&c)
{

    if (x0 > x1)
    {
        int temp = x0;
        x0 = x1;
        x1 = temp;
        temp = y0;
        y0 = y1;
        y1 = temp;
    }
    int dy = y1 - y0;
    int dx = x1 - x0;
    double k = (double)dy / (double)dx;

    if (abs(k) > 1)
    {
        double _k = 1 / k;
        if (y1 > y0) {
            for (int y = y0; y < y1; y++)
            {
                int nowX = x0 + _k * (y - y0);
                drawPoint(nowX, y, c);
            }
        }
        else {
            for (int y = y1; y < y0; y++)
            {
                int nowX = x1 + _k * (y - y1);
                drawPoint(nowX, y,c);
            }
        }

    }
    else {
        for (int x = x0; x < x1; x++)
        {
            int nowY = y0 + k * (x - x0);
            drawPoint(x, nowY,c);
        }
    }
}


void drawRect(int x, int y, int width, int height, const MiniColor&c)
{
    drawLine(x, y, x + width, y, c);
    drawLine(x + width, y, x + width, y + height, c);
    drawLine(x, y, x, y + height, c);
    drawLine(x, y + height, x + width, y + height, c);

}

void drawCircle(int x, int y, double r, const MiniColor&c)
{
    int lastX=x+r, lastY=y;


    double stepAng = 0.2;
    int nowX, nowY;

    for (double nowAng = 0; nowAng < 360; nowAng++)
    {
        double angRad = DecToRad(nowAng);
        nowX = x + cos(angRad) * r;
        nowY = y - sin(angRad) * r;
        drawLine(lastX, lastY, nowX, nowY, c);
        lastX = nowX;
        lastY = nowY;
    }
    drawLine(lastX, lastY, nowX, nowY, c);

}

void drawImage( int x, int y, const MiniImage& img)
{
    for (int i = y; i < img.height + y; i++)
    {
        for (int j = x; j < img.width + x; j++)
        {
            drawPoint(j, i, img._data[(i - y) * img.width + (j - x)]);
 
        }
    }
}

// 将string转换成wstring
wstring string2wstring(string str)
{
    wstring result;
    //获取缓冲区大小，并申请空间，缓冲区大小按字符计算  
    int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
    TCHAR* buffer = new TCHAR[len + 1];
    //多字节编码转换成宽字节编码  
    MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
    buffer[len] = '\0';             //添加字符串结尾  
    //删除缓冲区并返回值  
    result.append(buffer);
    delete[] buffer;
    return result;
}



MiniImage makeFontToMiniImage(std::string str, int size)
{
    HDC hFontCompatibleDC = CreateCompatibleDC(screen_hdc);

    HBITMAP hFontCompatibleBitmap = CreateCompatibleBitmap(screen_hdc, SCREEN_WIDTH, SCREEN_HEIGHT);
    HBITMAP hFontOldBitmap = (HBITMAP)SelectObject(hFontCompatibleDC, hFontCompatibleBitmap);
    //SetDIBits(screen_hdc, hCompatibleBitmap, 0, SCREEN_HEIGHT, BUFFER, (BITMAPINFO*)&binfo, DIB_RGB_COLORS);
    //BitBlt(screen_hdc, -1, -1, SCREEN_WIDTH, SCREEN_HEIGHT, hFontCompatibleDC, 0, 0, SRCCOPY);
    
    int bufferWidth = 1000;
    int bufferHeight = 100;




    HFONT hFontOld;
    hFontOld = (HFONT)SelectObject(hFontCompatibleDC, nowFont);
    std::wstring strNow = string2wstring(str);
    TextOut(hFontCompatibleDC, 0, 0, strNow.c_str(), strNow.size());
    LPSIZE Size=(LPSIZE)malloc(sizeof(LPSIZE));
    GetTextExtentPoint32A(hFontCompatibleDC, str.c_str(), str.size(), Size);
    


    MiniImage img;
    img.width = Size->cx;
    img.height = Size->cy;
    img._data = (MiniColor*)malloc(sizeof(MiniColor) * img.width * img.height);
    
    double scale = 1;
    int width = 7.5 * strNow.size()* scale;
    int height = 15* scale;

    int drawX = 100, drawY = 100;
    for (int i = 0; i < Size->cy; i++)
    {
        for (int j = 0; j < Size->cx; j++)
        {
            COLORREF nowColor = GetPixel(hFontCompatibleDC, j,i);
            MiniColor nowMiniColor;

            nowMiniColor.r = GetRValue(nowColor);
            nowMiniColor.g = GetGValue(nowColor);
            nowMiniColor.b = GetBValue(nowColor);
            if (nowMiniColor.r == 255 && nowMiniColor.g == 255 && nowMiniColor.b == 255) {
                nowMiniColor.a = 0;
            }
            else {
                nowMiniColor.r = 0;
                nowMiniColor.g = 0;
                nowMiniColor.b = 0;

            }
            img._data[i * img.width + j] = nowMiniColor;
        }
    }
    return img;
   

}

void setFont(const std::string& fontName, int size)
{
    double fontHeight = size;
    double fontWidth = fontHeight / 2;
    nowFont = CreateFont(-fontHeight, -fontWidth, 0, 0, 400, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,
        DEFAULT_QUALITY,
        FF_DONTCARE,
        L"微软雅黑");

    SendMessage(hwnd, WM_SETFONT, (WPARAM)nowFont, NULL);
}

void setTitle(const char* title)
{
    SetWindowTextA(hwnd, title);
}

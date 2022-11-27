#include "MiniEngine2D.h"
#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"

//引擎内部使用
extern _InputEventHookConfig mouseEventHookConfig;
extern _InputEventHookConfig keyboardEventHookConfig;
extern unsigned char keyStatus[0xFF];
extern std::pair<int, int> mousePosition;

bool MiniImage::load(const char* filepath)
{
	int channel;
	unsigned char* idata = stbi_load(filepath, &this->width, &this->height, &channel, 0);
	if (width == 0 || height == 0) {
		return false;
	}
	int totalPixelSum = width * height;
	_data = new MiniColor[totalPixelSum];
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			if (channel == 3) {
				int nowPos = y * width + x;
				int nowPosIndex = nowPos * 3;
				MiniColor color(idata[nowPosIndex], idata[nowPosIndex + 1], idata[nowPosIndex + 2]);
				_data[nowPos] = color;
			}
			else if (channel == 4) {
				int nowPos = y * width + x;
				int nowPosIndex = nowPos * 4;
				MiniColor color(idata[nowPosIndex], idata[nowPosIndex + 1], idata[nowPosIndex + 2],idata[nowPosIndex+3]);
				_data[nowPos] = color;
			}
		}
	}
	return true;

}

bool MiniImage::makeRectImage(int width, int height, const MiniColor& color)
{

	this->width = width;
	this->height = height;
	_data = (MiniColor*)malloc(sizeof(MiniColor) * width * height);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			_data[i * width + j] = color;
		}
	}
	return true;
}

MiniEngine2D::MiniEngine2D()
{

}

MiniEngine2D::~MiniEngine2D()
{
}

void MiniEngine2D::setTitle(const char* title)
{
	::setTitle(title);
}


bool MiniEngine2D::isKeyDown(int keyCode)
{
	bool result = keyStatus[keyCode] & 0x10;
	keyStatus[keyCode] &= 0x0F;
	return result;
}

bool MiniEngine2D::isKeyUp(int keyCode)
{
	bool result = keyStatus[keyCode] & 0x01;
	keyStatus[keyCode] &= 0xF0;
	//return GetAsyncKeyState(keyCode) & 0x0001;
	return result;
}

std::pair<int, int> MiniEngine2D::getMousePosition()
{
	return mousePosition;
}

int __cdecl MiniEngine2D::log(char const* const _Format, ...)
{
	va_list _ArgList;
	__crt_va_start(_ArgList, _Format);
	int result = FunctionDebugPrintlnA(_Format, _ArgList);
	__crt_va_end(_ArgList);
	return result;
}

int __cdecl MiniEngine2D::log(wchar_t const* const _Format, ...)
{
	va_list _ArgList;
	__crt_va_start(_ArgList, _Format);
	int result = FunctionDebugPrintlnW(_Format, _ArgList);
	__crt_va_end(_ArgList);
	return result;
}

int __cdecl MiniEngine2D::log(int obj)
{
	return log("%d", obj);
}

int __cdecl MiniEngine2D::log(unsigned long obj)
{
	return log("%u", obj);
}

int __cdecl MiniEngine2D::log(float obj)
{
	return log("%f", obj);
}

int __cdecl MiniEngine2D::log(double obj)
{
	return log("%lf", obj);
}

int __cdecl MiniEngine2D::logDetail(char const* const _Format, ...)
{
	va_list _ArgList;
	__crt_va_start(_ArgList, _Format);
	int result = FunctionDebugPrintlnDetailA(_Format, _ArgList);
	__crt_va_end(_ArgList);
	return result;
}

int __cdecl MiniEngine2D::logDetail(wchar_t const* const _Format, ...)
{
	va_list _ArgList;
	__crt_va_start(_ArgList, _Format);
	int result = FunctionDebugPrintlnDetailW(_Format, _ArgList);
	__crt_va_end(_ArgList);
	return result;
}

void MiniEngine2D::exportLogToFile(const std::string logFilePath)
{
	::exportLogToFile(logFilePath);
}


void MiniEngine2D::showWindow()
{
	::showWindow();
}


void MiniEngine2D::update()
{
	::update();
}

void MiniEngine2D::clearScreen()
{
	::clearScreen();
}

void MiniEngine2D::drawPoint(int x, int y, const MiniColor&c)
{
	::drawPoint(x, y, c);
}

void MiniEngine2D::drawLine(int x0, int y0, int x1, int y1, const MiniColor&c)
{
	::drawLine(x0, y0, x1, y1, c);
}

void MiniEngine2D::drawRect(int x, int y, int width, int height, const MiniColor&c)
{
	::drawRect(x, y, width, height, c);
}

void MiniEngine2D::drawCircle(int x, int y, double r, const MiniColor&c)
{
	::drawCircle(x, y, r, c);
}

void MiniEngine2D::drawImage( int x, int y, const MiniImage& img)
{
	::drawImage( x, y, img);
}

void MiniEngine2D::setFont(const std::string& fontName, int size)
{
	::setFont(fontName, size);
}


MiniImage MiniEngine2D::makeFontToMiniImage(std::string str, int size) 
{
	return ::makeFontToMiniImage(str, size);
}

/// <summary>
/// 给MiniEngine添加一个用于处理鼠标事件的函数，在发生鼠标事件时，
/// MiniEngine内部会先处理保留事件，处理完成之后会启动一个线程调用pfunc
/// 线程默认等待时间1000 ms
/// </summary>
/// <param name="pfunc">处理函数</param>
/// <param name="timeout">等待时间，单位毫秒，-1则无限等待，默认值1000ms</param>
void MiniEngine2D::addEventHook(PFuncMouseEvent pfunc, int timeout)
{
	//可能会有跨线程写的风险
	mouseEventHookConfig.pFuncMouseEvent = pfunc;
	mouseEventHookConfig.timeout = timeout;
}

/// <summary>
/// 给MiniEngine添加一个用于处理键盘事件的函数，在发生键盘事件时，
/// MiniEngine内部会先处理保留事件，处理完成之后会启动一个线程调用pfunc
/// 线程默认等待时间1000 ms
/// </summary>
/// <param name="pfunc">处理函数</param>
/// <param name="timeout">等待时间，单位毫秒，-1则无限等待，默认值1000ms</param>
void MiniEngine2D::addEventHook(PFuncKeyboardEvent pfunc, int timeout)
{
	//可能会有跨线程写的风险
	keyboardEventHookConfig.pFuncKeyboardEvent = pfunc;
	keyboardEventHookConfig.timeout = timeout;
}

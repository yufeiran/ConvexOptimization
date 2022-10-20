#pragma once

// 项目GitHub地址  https://github.com/miniProduction/miniEngine2D
// 函数命名文档    https://github.com/miniProduction/miniEngine2D/wiki/MiniEngine-%E5%91%BD%E5%90%8D%E5%8F%8A%E6%95%B0%E6%8D%AE%E7%BB%93%E6%9E%84
//
//
//

#include <string>


const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

struct MiniColor
{
	//a 通道 0是透明 255是不透明
	unsigned char b, g, r, a;
	MiniColor(int R, int G, int B, int A) :b(B), g(G), r(R), a(A) {};
	MiniColor(int R, int G, int B) :b(B), g(G), r(R), a(255) {};
	MiniColor() :b(0), g(0), r(0), a(255) {};

};

struct MiniImage
{
	int height, width;
	MiniColor* _data;
	MiniImage() :height(0), width(0), _data(nullptr) {};
	bool load(const char* filepath);
	bool makeRectImage(int width, int height, const MiniColor& color);

};


class MiniEngine2D {
public:
	MiniEngine2D();
	~MiniEngine2D();

	void showWindow();
	void gameLoop();
	void update();
	void clearScreen();
	void drawPoint(int x, int y, const MiniColor&);
	void drawLine(int x0, int y0, int x1, int y1, const MiniColor&);
	void drawRect(int x, int y, int width, int height, const MiniColor&);
	void drawCircle(int x, int y, double r, const MiniColor&);
	void drawImage( int x, int y, const MiniImage&);
	void setFont(const std::string& fontName, int size);
	void setTitle(const char* title);


	MiniImage makeFontToMiniImage(std::string str, int size);
	

	static int __cdecl log(char const* const _Format, ...);
	static int __cdecl log(wchar_t const* const _Format, ...);
	static int __cdecl log(int obj);
	static int __cdecl log(float obj);
	static int __cdecl log(double obj);
	static int __cdecl logDetail(char const* const _Format, ...);
	static int __cdecl logDetail(wchar_t const* const _Format, ...);
	static void exportLogToFile(const std::string logFilePath);
private:


};

#include "Trace.h"
#include "Graphics.h"



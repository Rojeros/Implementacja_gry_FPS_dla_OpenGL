
#include <windows.h>
#include "GameContener.h"

#if       _WIN32_WINNT < 0x0500
#undef  _WIN32_WINNT
#define _WIN32_WINNT   0x0500
#endif




int main(int argc, char *args[])
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	GameContener newGame;
	newGame.Run();

	return 0;
}

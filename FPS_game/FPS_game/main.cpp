
#include "GameContener.h"


#ifdef WIN32
#undef main
#endif
int main(int argc, char *argv[])
{
	GameContener newGame;
	newGame.Run();

	return 0;
}

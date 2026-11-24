#include "2d-game.h"
#include "3d-game.h"

int main() {
	#ifdef THREED
	game3d();
	#else
	game2d();
	#endif

	return 0;
}

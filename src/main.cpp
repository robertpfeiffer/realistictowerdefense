// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-
//detect memory leaks (see at output window of MS VC; only in debug mode active)
#ifdef _MSC_VER
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include <game.h>
#include <iostream>

int main(int argc, const char *argv[])
{
	#ifdef _MSC_VER
		_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	#endif

	srand(time(NULL));

	Game game;
	
	game.mapFile = argc == 2 ? argv[1] : "maps/snake.tdmap";

	game.run();

	return 0;
}

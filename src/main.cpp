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

	if(argc==2) {
		Game game;
		game.mapFile = argv[1];
		game.run();
		return 0;
	} else {
		std::cout << "missing map parameter" << std::endl;
	}
	return -1;
}

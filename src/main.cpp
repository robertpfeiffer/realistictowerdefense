// -*- mode: c++; coding: utf-8; c-basic-offset: 4; tab-width: 4; indent-tabs-mode:t; c-file-style: "stroustrup" -*-
//detect memory leaks (see at output window of MS VC; only in debug mode active)
#ifdef _MSC_VER
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include <game.h>
#include <iostream>
#include <osg/ArgumentParser>

/**
 * \fn	int main(int argc, char **argv)
 *
 * \brief	Main entry-point for this application.
 *
 * \param	argc	Number of command-line arguments.
 * \param	argv	Array of command-line argument strings.
 *
 * \return	Exit-code for the process - 0 for success, else an error code.
 */
int main(int argc, char **argv)
{
	#ifdef _MSC_VER
		_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	#endif

	srand(time(NULL));

	osg::ArgumentParser arguments(&argc, argv);

	Game game;
	game.run(arguments);

	return 0;
}

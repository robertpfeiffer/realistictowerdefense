//detect memory leaks (see at output window of MS VC; only in debug mode active)
#ifdef _MSC_VER
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif

#include <world.h>

int main()
{
	#ifdef _MSC_VER
		_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	#endif

	World world;
	world.run();

    return 0;
}

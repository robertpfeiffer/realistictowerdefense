cmake_minimum_required(VERSION 2.6) 
project(Towerdefense)

SET(OUTPUT_LIBDIR ${PROJECT_BINARY_DIR}/lib)
SET(OUTPUT_BINDIR ${PROJECT_BINARY_DIR}/bin)

FOREACH(CONF ${CMAKE_CONFIGURATION_TYPES})        # For each configuration (Debug, Release, MinSizeRel... and/or anything the user chooses)
	string(TOUPPER "${CONF}" CONF)                # Go uppercase (DEBUG, RELEASE...)
	set("CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONF}" "${OUTPUT_LIBDIR}")
	set("CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONF}" "${OUTPUT_BINDIR}")
	if(WIN32)
		set("CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONF}" "${OUTPUT_BINDIR}")
	else()
		set("CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONF}" "${OUTPUT_LIBDIR}")
	endif()
ENDFOREACH()

cmake_policy(SET CMP0015 NEW)

# ========================
# 		OpenSceneGraph
# ========================
include_directories("OSG/include")
link_directories("OSG/lib")
link_directories("OSG/bin")

include_directories("OSG/3rdParty/x86/include")
link_directories("OSG/3rdParty/x86/lib")

# ========================
# 		OpenSteer
# ========================
include_directories("opensteer/include")
link_directories("opensteer/lib")

# ========================
# 		Towerdefense
# ========================

include_directories("include")

# Executables
# -----------
set(sources
	#source files
	"src/assetlibrary.cpp"
	"src/contextmenu.cpp"
	"src/convert.cpp"
	"src/creep.cpp"
	"src/creepsteering.cpp"
	"src/field.cpp"
	"src/fieldcontextmenu.cpp"
	"src/fieldtype.cpp"
	"src/game.cpp"
	"src/gametimer.cpp"
	"src/graveyard.cpp"
	"src/hatchery.cpp"
	"src/healthbar.cpp"
	"src/hud.cpp"
	"src/inscenetext.cpp"
	"src/main.cpp"
	"src/tdviewer.cpp"
	"src/map.cpp"
	"src/menubutton.cpp"
	"src/projectile.cpp"
	"src/skybox.cpp"
	"src/terrain.cpp"
	"src/tower.cpp"
	"src/transformhelper.cpp"
	"src/updatablenode.cpp"
	"src/updatecallback.cpp"
	"src/userinteractionhandler.cpp"
	"src/wave.cpp"
	"src/world.cpp"
	
	#header files
	"include/assetlibrary.h"
	"include/constants.h"
	"include/contextmenu.h"
	"include/convert.h"
	"include/creep.h"
	"include/creepattributes.h"
	"include/creepsteering.h"	
	"include/field.h"
	"include/fieldcontextmenu.h"
	"include/fieldtype.h"
	"include/game.h"
	"include/tdviewer.h"
	"include/gametimer.h"
	"include/graveyard.h"
	"include/hatchery.h"
	"include/healthbar.h"
	"include/hud.h"
	"include/inscenetext.h"
	"include/keyboardeventhandler.h"
	"include/map.h"
	"include/menubutton.h"
	"include/modeldata.h"
	"include/mouseeventhandler.h"
	"include/player.h"
	"include/projectile.h"
	"include/projectileattributes.h"
	"include/skybox.h"
	"include/terrain.h"
	"include/tower.h"
	"include/towerattributes.h"
	"include/transformhelper.h"
	"include/updatablenode.h"
	"include/updatecallback.h"
	"include/userinteractionhandler.h"
	"include/wave.h"
	"include/world.h"
)

# Creeps and Waves
# -------------------
source_group("Source Files\\Creeps and Waves" FILES
	"src/creep.cpp"
	"src/creepsteering.cpp"
	"src/wave.cpp"
)

source_group("Header Files\\Creeps and Waves" FILES
	"include/creep.h"
	"include/creepattributes.h"
	"include/creepsteering.h"
	"include/wave.h"
)

# Handlers and Callbacks
# -------------------
source_group("Source Files\\Handlers and Callbacks" FILES
	"src/updatecallback.cpp"
	"src/userinteractionhandler.cpp"
)

source_group("Header Files\\Handlers and Callbacks" FILES
	"include/keyboardeventhandler.h"
	"include/mouseeventhandler.h"
	"include/updatecallback.h"
	"include/userinteractionhandler.h"
)

# Towers and Projectiles
# -------------------
source_group("Source Files\\Towers and Projectiles" FILES
	"src/projectile.cpp"
	"src/tower.cpp"
)

source_group("Header Files\\Towers and Projectiles" FILES
	"include/projectile.h"
	"include/projectileattributes.h"
	"include/tower.h"
	"include/towerattributes.h"
)

# Resources managment
# -------------------
source_group("Source Files\\Resources managment" FILES
	"src/assetlibrary.cpp"
	"src/hatchery.cpp"
	"src/graveyard.cpp"
)

source_group("Header Files\\Resources managment" FILES
	"include/assetlibrary.h"
	"include/hatchery.h"
	"include/graveyard.h"
)

# User Interface
# -------------------
source_group("Source Files\\User Interface" FILES
	"src/contextmenu.cpp"
	"src/fieldcontextmenu.cpp"
	"src/healthbar.cpp"
	"src/hud.cpp"
	"src/inscenetext.cpp"
	"src/menubutton.cpp"
)

source_group("Header Files\\User Interface" FILES
	"include/contextmenu.h"
	"include/fieldcontextmenu.h"
	"include/healthbar.h"
	"include/hud.h"
	"include/inscenetext.h"
	"include/menubutton.h"
)

add_executable(Towerdefense ${sources})

# linked libraries
# ----------------

# Release
target_link_libraries(Towerdefense optimized osg)
target_link_libraries(Towerdefense optimized osgParticle)
target_link_libraries(Towerdefense optimized osgViewer)
target_link_libraries(Towerdefense optimized osgGA)
target_link_libraries(Towerdefense optimized osgDB)
target_link_libraries(Towerdefense optimized osgText)
target_link_libraries(Towerdefense optimized OpenThreads)
target_link_libraries(Towerdefense optimized opensteer)

# Debug
target_link_libraries(Towerdefense debug osgd)
target_link_libraries(Towerdefense debug osgParticled)
target_link_libraries(Towerdefense debug osgViewerd)
target_link_libraries(Towerdefense debug osgGAd)
target_link_libraries(Towerdefense debug osgDBd)
target_link_libraries(Towerdefense debug osgTextd)
target_link_libraries(Towerdefense debug OpenThreadsd)
target_link_libraries(Towerdefense debug opensteerd)

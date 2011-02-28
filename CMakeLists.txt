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
	"src/creep.cpp"
	"src/creepsteering.cpp"
	"src/creepcallback.cpp"
	"src/field.cpp"
	"src/field_type.cpp"
	"src/game.cpp"
	"src/game_timer.cpp"
	"src/graveyard.cpp"
	"src/hud.cpp"
	"src/main.cpp"
	"src/map.cpp"
	"src/menubutton.cpp"
	"src/projectile.cpp"
	"src/terrain.cpp"
	"src/user_interaction_handler.cpp"
	"src/wave.cpp"
	"src/world.cpp"
	
	#header files
	"include/constants.h"
	"include/creep.h"
	"include/creepattributes.h"
	"include/creepcallback.h"
	"include/creepsteering.h"	
	"include/field.h"
	"include/menubutton.h"
	"include/field_type.h"
	"include/game.h"
	"include/game_timer.h"
	"include/graveyard.h"
	"include/hud.h"
	"include/keyboard_event_handler.h"
	"include/map.h"
	"include/model_data.h"
	"include/mouse_event_handler.h"
	"include/player.h"
	"include/projectile.h"
	"include/projectileattributes.h"
	"include/terrain.h"
	"include/user_interaction_handler.h"
	"include/wave.h"
	"include/world.h"
)

# Interaction Handler
# -------------------
source_group("Source Files\\Interaction Handler" FILES
	"src/user_interaction_handler.cpp"
)

source_group("Header Files\\Interaction Handler" FILES
	"include/keyboard_event_handler.h"
	"include/mouse_event_handler.h"
	"include/user_interaction_handler.h"
)

# Creeps
# -------------------
source_group("Source Files\\Creeps" FILES
	"src/creep.cpp"
	"src/creepsteering.cpp"
	"src/creepcallback.cpp"
)

source_group("Header Files\\Creeps" FILES
	"include/creep.h"
	"include/creepattributes.h"
	"include/creepcallback.h"
	"include/creep_event_handler.h"
	"include/creepsteering.h"
)

add_executable(Towerdefense ${sources})

# linked libraries
# ----------------

# Release
target_link_libraries(Towerdefense optimized osg)
target_link_libraries(Towerdefense optimized osgViewer)
target_link_libraries(Towerdefense optimized osgGA)
target_link_libraries(Towerdefense optimized osgDB)
target_link_libraries(Towerdefense optimized OpenThreads)
target_link_libraries(Towerdefense optimized opensteer)

# Debug
target_link_libraries(Towerdefense debug osgd)
target_link_libraries(Towerdefense debug osgViewerd)
target_link_libraries(Towerdefense debug osgGAd)
target_link_libraries(Towerdefense debug osgDBd)
target_link_libraries(Towerdefense debug OpenThreadsd)
target_link_libraries(Towerdefense debug opensteerd)
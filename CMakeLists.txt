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

# ========================
# 		Towerdefense
# ========================

include_directories("include")

# Executables
# -----------
set(sources
	#source files
	"src/field_node.cpp"
	"src/main.cpp"
	"src/map.cpp"
	
	#header files
	"include/constants.h"
	"include/field_node.h"
	"include/map.h"
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

# Debug
target_link_libraries(Towerdefense debug osgd)
target_link_libraries(Towerdefense debug osgViewerd)
target_link_libraries(Towerdefense debug osgGAd)
target_link_libraries(Towerdefense debug osgDBd)
target_link_libraries(Towerdefense debug OpenThreadsd)

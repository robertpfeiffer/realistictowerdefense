cmake_minimum_required(VERSION 2.6) 
project(Towerdefense)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

# ========================
# 		OpenSceneGraph
# ========================
# Includes
# --------
include_directories("OSG/include/OpenThreads")
include_directories("OSG/include/osg")
include_directories("OSG/include/osgAnimation")
include_directories("OSG/include/osgDB")
include_directories("OSG/include/osgFX")
include_directories("OSG/include/osgGA")
include_directories("OSG/include/osgIntrospection")
include_directories("OSG/include/osgManipulator")
include_directories("OSG/include/osgParticle")
include_directories("OSG/include/osgShadow")
include_directories("OSG/include/osgSim")
include_directories("OSG/include/osgTerrain")
include_directories("OSG/include/osgText")
include_directories("OSG/include/osgUtil")
include_directories("OSG/include/osgViewer")
include_directories("OSG/include/osgVolume")
include_directories("OSG/include/osgWidget")

# Libraries
# ---------

# ========================
# 		Towerdefense
# ========================

# Executables
# ---------
add_executable(Towerdefense "src/main.cpp")
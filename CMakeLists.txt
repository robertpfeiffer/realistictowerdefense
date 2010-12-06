cmake_minimum_required(VERSION 2.6) 
project(Towerdefense)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
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

# Executables
# -----------
add_executable(Towerdefense "src/main.cpp")

# linked libraries
# ----------------
# Release
target_link_libraries(Towerdefense optimized osg)
target_link_libraries(Towerdefense optimized osgViewer)
target_link_libraries(Towerdefense optimized osgGA)

# Debug
target_link_libraries(Towerdefense debug osgd)
target_link_libraries(Towerdefense debug osgViewerd)
target_link_libraries(Towerdefense debug osgGAd)
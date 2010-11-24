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
include_directories("OSG/include")

# Libraries
# ---------

# ========================
# 		Towerdefense
# ========================

# Executables
# ---------
add_executable(Towerdefense "src/main.cpp")
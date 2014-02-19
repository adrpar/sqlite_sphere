cmake_minimum_required (VERSION 2.6)
project (sqlitesphere)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/Modules/")

#FIND SQLITE
find_package (SQLITE3)
message("Found SQLITE3: ${SQLITE3_FOUND}")
if(SQLITE3_FOUND)
	include_directories(${SQLITE3_INCLUDE_DIR})
else()
	message( FATAL_ERROR "SQlite3 developer files not found!")
endif()

set(CMAKE_BUILD_TYPE Release)
SET(BUILD_SHARED_LIBS ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -lm")

include_directories ("${PROJECT_SOURCE_DIR}/src")

file(GLOB FILES_SRC "${PROJECT_SOURCE_DIR}/src/*.h" "${PROJECT_SOURCE_DIR}/src/*.cpp" "${PROJECT_SOURCE_DIR}/src/*.cc" "${PROJECT_SOURCE_DIR}/src/*.c")

add_library (sqlitesphere ${FILES_SRC})

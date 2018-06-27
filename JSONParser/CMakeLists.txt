CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

SET(PROJECT_NAME JSONParser)

PROJECT(${PROJECT_NAME})

SET(HEADERS
	sources/include/ProjectLib.h
)

SET(SOURCES
	sources/src/ProjectLib.cpp
)

INCLUDE_DIRECTORIES(sources/include)
add_library(
	${PROJECT_NAME}
	STATIC
	${HEADERS}
	${SOURCES}
)
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

SET(PROJECT_NAME JSONParserTest)


PROJECT(${PROJECT_NAME})

SET(CATCH_LIB_INCLUDE  CACHE PATH  REQUIRED)

SET(SOURCES
	src/Source.cpp
)

set(RESOURCES
resources/out.json
resources/colors.json
resources/colors2.json
resources/merged.json
)

SOURCE_GROUP("resources" FILES ${RESOURCES})

INCLUDE_DIRECTORIES(
../JSONParser/sources/include
${CATCH_LIB_INCLUDE}
)
link_directories(
../build/JSONParser/
)
 add_executable(
 ${PROJECT_NAME}
 ${SOURCES}
 ${RESOURCES}
 )
 
 target_link_libraries(
 ${PROJECT_NAME}
 JSONParser.lib
 )
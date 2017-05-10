project(homework)

aux_source_directory(. MAIN_SRC_LIST)

SET(CMAKE_BUILD_TYPE Debug)
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")

add_executable(${PROJECT_NAME} ${MAIN_SRC_LIST})
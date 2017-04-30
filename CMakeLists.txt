project(yapc) 
cmake_minimum_required(VERSION 2.8) 
aux_source_directory(. SRC_LIST) 
SET(CMAKE_BUILD_TYPE Debug)
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
LINK_LIBRARIES(m)

add_executable(${PROJECT_NAME} ${SRC_LIST})

# add_library(${PROJECT_NAME} SHARED ${SRC_LIST})

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

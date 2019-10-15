cmake_minimum_required(VERSION 3.5)

PROJECT(DMFB)

INCLUDE_DIRECTORIES(
    "include/"
)

AUX_SOURCE_DIRECTORY(src DIR_SRCS)
AUX_SOURCE_DIRECTORY(src/core DIR_SRCS)
AUX_SOURCE_DIRECTORY(src/droplet DIR_SRCS)
AUX_SOURCE_DIRECTORY(src/entities DIR_SRCS)
AUX_SOURCE_DIRECTORY(src/grid DIR_SRCS)
AUX_SOURCE_DIRECTORY(src/math_models DIR_SRCS)
AUX_SOURCE_DIRECTORY(src/placement DIR_SRCS)
AUX_SOURCE_DIRECTORY(src/useless DIR_SRCS)

add_compile_options("-Ofast")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
ADD_EXECUTABLE(${PROJECT_NAME} ${DIR_SRCS} main.cpp)
ADD_EXECUTABLE(test_time ${DIR_SRCS} test.cpp)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
# ADD_LIBRARY(i_dmfb SHARED ${DIR_SRCS})
# add_compile_options(-fPIC)
# ADD_LIBRARY(pylib SHARED ${DIR_SRCS} pylib.cpp)
# target_link_libraries(pylib boost_python36)

# add_definitions(-DBONE)
add_definitions(-DNDEBUG)

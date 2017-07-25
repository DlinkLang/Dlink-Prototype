cmake_minimum_required(VERSION 2.6)
project(Dlink)

set(AUTHOR_NAME "funniest_ms, kmc7468, dev_kr")

set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

set(TARGET_DIR 	"./bin")
set(INCLUDE_DIR "./include")
set(SRC_DIR 	"./src")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${TARGET_DIR})

include_directories(${INCLUDE_DIR})
file(GLOB srcs ${SRC_DIR}/*.cc)

add_executable(${PROJECT_NAME} ${srcs})

set(CMAKE_CXX_COMPILER "clang++")

set(EXTRA_COMPILE_OPTIONS "-std=c++14")
set(EXTRA_LINK_OPTIONS "")

target_compile_options(${PROJECT_NAME} PRIVATE ${EXTRA_COMPILE_OPTIONS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${EXTRA_LINK_OPTIONS})

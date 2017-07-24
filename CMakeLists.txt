cmake_minimum_required(VERSION 2.6)
project(Dlink)

find_package(BISON)
find_package(FLEX)

set(AUTHOR_NAME "funniest_ms, kmc7468, dev_kr")

set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

set(TARGET_DIR 	"${CMAKE_SOURCE_DIR}/bin")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(SRC_DIR 	"${CMAKE_SOURCE_DIR}/src")

bison_target(Parser "${SRC_DIR}/parser.yy" "${SRC_DIR}/parser.tab.cc")
flex_target(Scanner "${SRC_DIR}/lexer.l"  "${SRC_DIR}/lex.yy.cc")
ADD_FLEX_BISON_DEPENDENCY(Scanner Parser)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${TARGET_DIR})

include_directories(${CMAKE_SOURCE_DIR}/include)
file(GLOB srcs ${SRC_DIR}/*.cc ${BISON_Parser_OUTPUTS} ${FLEX_Scanner_OUTPUTS})

add_executable(${PROJECT_NAME} ${srcs})

add_custom_command(TARGET ${PROJECT_NAME}
				   PRE_BUILD
				   COMMAND mv ${SRC_DIR}/*.hh ${INCLUDE_DIR}/)

set(CMAKE_CXX_COMPILER "clang++")

set(EXTRA_COMPILE_OPTIONS "-std=c++14")
set(EXTRA_LINK_OPTIONS "")

target_compile_options(Dlink PRIVATE ${EXTRA_COMPILE_OPTIONS})
target_link_libraries(Dlink PRIVATE ${EXTRA_LINK_OPTIONS})

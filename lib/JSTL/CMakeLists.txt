# /lib/CMakeLists.txt

file(GLOB JSTL_SOURCES "*.cpp")
add_library(JSTL ${JSTL_SOURCES})

target_include_directories(JSTL PUBLIC ${CMAKE_SOURCE_DIR}/include/JSTL)
target_compile_options(JSTL PRIVATE -Wall -Werror)
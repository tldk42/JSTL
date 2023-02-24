# /lib/CMakeLists.txt

file(GLOB JSTL_SOURCES
        *.cpp)
file(GLOB JSTL_HEADERS
        ${CMAKE_SOURCE_DIR}/include/JSTL/*.h
        )
set (ALL_FILES
        ${JSTL_HEADERS}
        ${JSTL_SOURCES}
        )
include_directories(${CMAKE_SOURCE_DIR}/include)
add_library(JSTL ${ALL_FILES})

target_include_directories(JSTL PUBLIC ${CMAKE_SOURCE_DIR}/include/JSTL)
target_compile_options(JSTL PRIVATE -Wall -Werror)
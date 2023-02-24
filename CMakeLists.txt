# CMAKE 최소 버전
cmake_minimum_required(VERSION 3.1)
# 프로젝트 정보
project(Engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall")
file(GLOB HEADER_FILES ${CMAKE_SOURCE_DIR}/include/*.h)

# 라이브러리 디렉터리 추가
add_subdirectory(lib/JSTL)

add_executable(Engine main.cpp ${HEADER_FILES})

target_include_directories(Engine PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(Engine JSTL)
target_compile_options(Engine PUBLIC -Wall -Werror)

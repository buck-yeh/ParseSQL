cmake_minimum_required (VERSION 3.18)
cmake_policy(VERSION 3.18)

#set(CMAKE_CXX_COMPILER "clang++")

include(FetchContent)

FetchContent_Declare(
  FetchedBux
  GIT_REPOSITORY https://github.com/buck-yeh/bux.git
  GIT_TAG        main
  SOURCE_DIR     ../bux
)
FetchContent_MakeAvailable(FetchedBux)

project(ParserSQL)

add_subdirectory (src)

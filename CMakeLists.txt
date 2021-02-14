cmake_minimum_required (VERSION 3.18)
cmake_policy(VERSION 3.18)

#set(CMAKE_CXX_COMPILER_ID "Clang")

include(FetchContent)

FetchContent_Declare(
  FetchedBux
  GIT_REPOSITORY https://github.com/buck-yeh/bux.git
  GIT_TAG        main
  SOURCE_DIR     ../bux
)
FetchContent_MakeAvailable(FetchedBux)

project(ParserSQL)

IF(CMAKE_CXX_COMPILER_ID MATCHES Clang|GNU)
    string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra -Wshadow -Wconversion -Ofast -std=c++2a")
    IF(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        message("Clang")
        string(APPEND CMAKE_CXX_FLAGS " -Wno-potentially-evaluated-expression -Wno-deprecated-anon-enum-enum-conversion")
    ELSEIF(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        message("GNU")
    ELSE()
        message("Else 1/2")
    ENDIF()
ELSE()
    message("Else 2/2")
ENDIF()

add_subdirectory (src)

cmake_minimum_required (VERSION 3.18)
cmake_policy(VERSION 3.18)

#set(CMAKE_CXX_COMPILER_ID "Clang")

if(NOT DEFINED DEPENDEE_ROOT)
set(DEPENDEE_ROOT "..")
endif()

if(DEFINED FETCH_DEPENDEES)
include(FetchContent)

FetchContent_Declare(
  FetchedBux
  GIT_REPOSITORY https://github.com/buck-yeh/bux.git
  GIT_TAG        main
  SOURCE_DIR     ${DEPENDEE_ROOT}/bux
)
FetchContent_MakeAvailable(FetchedBux)
endif()

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

    IF(DEFINED FETCH_DEPENDEES)
        string(APPEND CMAKE_CXX_FLAGS " -I${fetchedbux_SOURCE_DIR}/include -L${fetchedbux_SOURCE_DIR}/src")
    ENDIF()
ELSE()
    message("Else 2/2")
ENDIF()
message("Root/CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")

add_subdirectory (src)

string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra -Wshadow -Wno-parentheses -Wno-unknown-pragmas -g3 -Og -std=c++2a")

add_library(parsesql STATIC
    SqlObjs.cpp Scanner.cpp ParseSQL.cpp Parser.cpp)
target_include_directories(parsesql PRIVATE ../include ../../bux/include)

add_executable(formatsql formatsql.cpp)
target_include_directories(formatsql PRIVATE ../include ../../bux/include)
target_link_libraries(formatsql parsesql bux fmt)

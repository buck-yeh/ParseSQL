IF(NOT DEFINED FETCH_DEPENDEES)
    string(APPEND CMAKE_CXX_FLAGS " -I../${DEPENDEE_ROOT}/bux/include -L../${DEPENDEE_ROOT}/bux/src")
ENDIF()

add_library(parsesql STATIC
    SqlObjs.cpp Scanner.cpp ParseSQL.cpp Parser.cpp)
target_include_directories(parsesql PRIVATE ../include)

add_executable(formatsql formatsql.cpp)
target_include_directories(formatsql PRIVATE ../include)
target_link_libraries(formatsql parsesql bux fmt stdc++)

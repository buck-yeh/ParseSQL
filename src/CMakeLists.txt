add_library(parsesql STATIC
    SqlObjs.cpp Scanner.cpp ParseSQL.cpp Parser.cpp)
target_include_directories(parsesql PRIVATE ../include ../${DEPENDEE_ROOT}/bux/include)

add_executable(formatsql formatsql.cpp)
target_include_directories(formatsql PRIVATE ../include ../${DEPENDEE_ROOT}/bux/include)
target_link_directories(formatsql PRIVATE ../${DEPENDEE_ROOT}/bux/src)
target_link_libraries(formatsql parsesql bux fmt stdc++)

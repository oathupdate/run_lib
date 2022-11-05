cmake_minimum_required(VERSION 3.6)

PROJECT(run)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -O0")

include_directories("${CMAKE_SOURCE_DIR}/")

file(GLOB_RECURSE SRC_LIST ${CMAKE_SOURCE_DIR}/widget/*.cc)
file(GLOB_RECURSE MODULES_SRC_LIST ${CMAKE_SOURCE_DIR}/modules/*.cc)
file(GLOB_RECURSE RUN_SRC_LIST ${CMAKE_SOURCE_DIR}/run.cc)
list(APPEND SRC_LIST ${MODULES_SRC_LIST})
list(APPEND SRC_LIST ${RUN_SRC_LIST})


add_library(run STATIC ${SRC_LIST})

target_link_libraries(run PUBLIC
    sqlite3
    glog
    jsoncpp
    stdc++
    )

add_subdirectory(main)

set_target_properties(run PROPERTIES 
    PUBLIC_HEADER ${CMAKE_SOURCE_DIR}/run.h
    )

install(TARGETS ${PROJECT_NAME}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

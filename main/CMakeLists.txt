include_directories("${CMAKE_SOURCE_DIR}/")

add_executable(main main.cc)

target_link_libraries(main
    run
    sqlite3
    glog
    jsoncpp
    stdc++
    )



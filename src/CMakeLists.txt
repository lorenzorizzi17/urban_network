cmake_minimum_required(VERSION 3.10)

project(UrbanNetworkModel)

set(CMAKE_CXX_FLAGS_RELEASE "-O3")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-D_DEBUG)
endif()

include_directories("~/boost_1_87_0/")

find_package(SFML 2.5 REQUIRED COMPONENTS window system graphics)

set(SOURCES
    agents.cpp
    dynamics.cpp
    main.cpp
    statistics.cpp
    graph_builder.cpp
    ODModel.cpp
    config.cpp
    gridlock_exception.cpp
    parser.cpp
    renderer.cpp
)

add_executable(main ${SOURCES})

target_link_libraries(main
    sfml-window
    sfml-system
    sfml-graphics
)

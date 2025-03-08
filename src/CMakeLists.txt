cmake_minimum_required(VERSION 3.10)

project(UrbanNetworkModel)

set(CMAKE_CXX_FLAGS_RELEASE "-O3")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-D_DEBUG)
endif()

if(NOT DEFINED ANALYSIS)
    set(ANALYSIS FALSE)
endif()

include_directories("~/boost")

find_package(SFML 2.5 REQUIRED COMPONENTS window system graphics)
find_package(OpenMP REQUIRED)

set(SOURCES
    agents.cpp
    dynamics.cpp
    statistics.cpp
    graph_builder.cpp
    ODModel.cpp
    config.cpp
    gridlock_exception.cpp
    parser.cpp
    renderer.cpp
)

if(ANALYSIS)
    list(APPEND SOURCES analysis.cpp)
    add_executable(analysis ${SOURCES})
    target_link_libraries(analysis
        sfml-graphics
        sfml-window
        sfml-system
        OpenMP::OpenMP_CXX )
else()
    list(APPEND SOURCES main.cpp)
    add_executable(main ${SOURCES})
    target_link_libraries(main
        sfml-graphics
        sfml-window
        sfml-system 
        OpenMP::OpenMP_CXX )
    
endif()





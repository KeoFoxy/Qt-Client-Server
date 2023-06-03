cmake_minimum_required(VERSION 3.5)
project(Server VERSION 0.1 LANGUAGES CXX C)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC_SEARCH_PATHS
#        ${CMAKE_CURRENT_LIST_DIR}/UI
        )

find_package(Qt6 COMPONENTS
        Qt6::Core
        Gui
        Widgets
        REQUIRED
        Qt6::Network)

add_executable(Server 
        main.cpp 
        server.h
        server.cpp)

target_link_libraries(Server 
        PRIVATE 
        Qt6::Core 
        Qt6::Network)

target_include_directories(Server 
        PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR})

if(WIN32)
    target_sources(Server PRIVATE server_icon.rc)
endif()

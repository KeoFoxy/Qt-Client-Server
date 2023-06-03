cmake_minimum_required(VERSION 3.5)
project(Client VERSION 0.1 LANGUAGES CXX C)

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

add_executable(Client 
        main.cpp 
        client.h
        client.cpp)

target_link_libraries(Client 
        PRIVATE 
        Qt6::Core 
        Qt6::Network)

target_include_directories(Client 
        PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR})

if(WIN32)
    target_sources(Client PRIVATE client_icon.rc)
endif()

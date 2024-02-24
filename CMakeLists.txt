project(ToDo)
cmake_minimum_required(VERSION 3.20)

#----------------------------------------------------------------
# Complier options
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)
qt_standard_project_setup()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME}
    include/task.h
    include/taskmanager.h
    include/mainwidget.h
    include/taskeditwidget.h

    source/main.cpp
    source/taskmanager.cpp
    source/task.cpp
    source/mainwidget.cpp
    source/taskeditwidget.cpp
    )

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)
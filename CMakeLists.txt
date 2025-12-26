cmake_minimum_required(VERSION 3.16)
project(student_records)

set(CMAKE_CXX_STANDARD 20)

# Console version (original)
add_executable(student_records main.cpp RBTree.cpp RBTree.h)

# GUI version with Qt
option(BUILD_GUI "Build GUI version" ON)

if(BUILD_GUI)
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOUIC ON)
    
    find_package(Qt6 COMPONENTS Widgets QUIET)
    
    if(Qt6_FOUND)
        add_executable(student_records_gui 
            main_gui.cpp 
            MainWindow.cpp 
            MainWindow.h
            RBTree.cpp 
            RBTree.h
        )
        
        target_link_libraries(student_records_gui Qt6::Widgets)
        
        message(STATUS "Qt6 found - GUI version will be built")
    else()
        message(WARNING "Qt6 not found - GUI version will not be built. Install Qt6 to enable GUI.")
    endif()
endif()

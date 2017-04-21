cmake_minimum_required(VERSION 3.7)
project(TestTask1)

set(CMAKE_CXX_STANDARD 14)

find_package(Qt5Core REQUIRED)
find_package(Qt5Charts REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5OpenGL REQUIRED)
find_package(Qt5Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(
        Editor
        Math
        ${Qt5Core_INCLUDE_DIRS}
        ${Qt5Charts_INCLUDE_DIRS}
        ${Qt5Gui_INCLUDE_DIRS}
        ${Qt5OpenGL_INCLUDE_DIRS}
        ${Qt5Widgets_INCLUDE_DIRS}
        )

set(SOURCE_FILES
        main.cpp
        Editor/EllipseCreator.cpp
        Editor/EllipseCreator.h
        Editor/FileIO.cpp
        Editor/FileIO.h
        Editor/GeometricPrimitiveCreator.cpp
        Editor/GeometricPrimitiveCreator.h
        Editor/LineCreator.cpp
        Editor/LineCreator.h
        Editor/MainWindow.cpp
        Editor/MainWindow.h
        Editor/MainWindowHandler.cpp
        Editor/MainWindowHandler.h
        Editor/PointCreator.cpp
        Editor/PointCreator.h
        Math/Ellipse.cpp
        Math/Ellipse.h
        Math/GeometricPrimitive.h
        Math/Line.cpp
        Math/Line.h
        Math/Point.cpp
        Math/Point.h
        )

add_executable(TestTask1 ${SOURCE_FILES})

qt5_use_modules(TestTask1 Core Charts Gui OpenGL Widgets)
#find_package(OpenGL REQUIRED)
#target_link_libraries(TestTask1 ${OPENGL_LIBRARIES} ${GLU_LIBRARY})

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

option(UTESTS "Build with Utests" OFF)
if (UTESTS)
	set(UTestSrc Math/UnitTests/Tests.cpp)
	include_directories(Math/UnitTests)
else ()
	set(UTestSrc)
endif(UTESTS)

option(EDITOR "Compile Editor" ON)
if (EDITOR)
	set(EditorSrc main.cpp
		Editor/CreatorHandler.cpp
		Editor/CreatorHandler.h
		Editor/DisplayedObject.h
		Editor/DisplayedObject.cpp
		Editor/FileIO.cpp
		Editor/FileIO.h
		Editor/MainWindow.cpp
		Editor/MainWindow.h
		Editor/MainWindowHandler.cpp
		Editor/MainWindowHandler.h
		Editor/Polyline.cpp
		Editor/CommonConstants.h
		Editor/CommonConstants.cpp)
else(EDITOR)
	set(EditorSrc)
endif(EDITOR)

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
		${EditorSrc}
	    Math/C2Curve.cpp
        Math/C2Curve.h
        Math/Ellipse.cpp
        Math/Ellipse.h
        Math/GeometricPoint.cpp
        Math/GeometricPoint.h
        Math/GeometricPrimitive.h
        Math/Line.cpp
        Math/Line.h
        Math/NurbsCurve.cpp
        Math/NurbsCurve.h
        Math/Point.cpp
        Math/Point.h
        Math/GeomPolyLine.cpp
        Math/GeomPolyline.h
        Math/Range.cpp
        Math/Range.h
		Math/Vector.cpp
		Math/Vector.h
        Serealizer/CurveSerializer.h
		Serealizer/CurveSerializer.cpp
		Serealizer/LineSerializer.h
		Serealizer/LineSerializer.cpp
		${UtestSrc}
        )

add_executable(TestTask1 ${SOURCE_FILES})

qt5_use_modules(TestTask1 Core Charts Gui OpenGL Widgets)
#find_package(OpenGL REQUIRED)
#target_link_libraries(TestTask1 ${OPENGL_LIBRARIES} ${GLU_LIBRARY})

cmake_minimum_required(VERSION 3.7)
project(TestTask1)
#set (CMAKE_PREFIX_PATH "C:\\Qt\\5.8\\msvc2015_64")
set(CMAKE_CXX_STANDARD 14)

option(UTESTS "Build with Utests" OFF)
if (UTESTS)
	set(UTestSrc $ENV{GTEST_ROOT}/src/gtest-all.cc
			Math/UnitTests/utest_main.cpp
			Math/UnitTests/utest_line.cpp
			Math/UnitTests/utest_ellipse.cpp
			Math/UnitTests/utest_nurbs.cpp
			Math/UnitTests/utest_polyline.cpp
			Math/UnitTests/utest_intersector.cpp
			)
	include_directories($ENV{GTEST_ROOT}
			$ENV{GTEST_ROOT}/include
			Math/UnitTests
			)
else ()
	set(UTestSrc)
endif(UTESTS)

option(EDITOR "Compile Editor" OFF)
if (EDITOR)
	set(CMAKE_AUTOMOC ON)
	set(CMAKE_AUTOUIC ON)
	set(CMAKE_INCLUDE_CURRENT_DIR ON)

	find_package(Qt5Core REQUIRED)
	find_package(Qt5Charts REQUIRED)
	find_package(Qt5Gui REQUIRED)
	find_package(Qt5OpenGL REQUIRED)
	find_package(Qt5Widgets REQUIRED)

	set(EditorSrc
			main.cpp
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
     	    Editor/CommonConstantsEditor.cpp
		    Editor/CommonConstantsEditor.h
			)

	include_directories(${Qt5Core_INCLUDE_DIRS}
			${Qt5Charts_INCLUDE_DIRS}
			${Qt5Gui_INCLUDE_DIRS}
			${Qt5OpenGL_INCLUDE_DIRS}
			${Qt5Widgets_INCLUDE_DIRS})
else(EDITOR)
	set(EditorSrc)
endif(EDITOR)

include_directories(
		Editor
		Math
)

message("UTEST: " ${UTestSrc})
message("EDITOR: " ${EditorSrc})
set(SOURCE_FILES
		${EditorSrc}
		${UTestSrc}
		Math/BoundingBox.cpp
		Math/BoundingBox.h
		Math/Curve.cpp
		Math/Curve.h
		Math/CommonConstantsMath.cpp
		Math/CommonConstantsMath.h
		Math/Ellipse.cpp
		Math/Ellipse.h
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
		Math/Intersector.h
		Math/Intersector.cpp
		Serializer/CurveSerializer.h
		Serializer/CurveSerializer.cpp
		Serializer/LineSerializer.h
		Serializer/LineSerializer.cpp
		Serializer/EllipseCurveSerializer.cpp
		Serializer/EllipseCurveSerializer.h
		Serializer/NURBSCurveSerializer.cpp
		Serializer/NURBSCurveSerializer.h
		Serializer/PolylineSerializer.cpp
		Serializer/PolylineSerializer.h
		Serializer/Serializer.cpp
		Serializer/Serializer.h
		)

add_executable(TestTask1 ${SOURCE_FILES})


if (UTESTS)
	target_link_libraries(TestTask1 pthread)
endif (UTESTS)

if (EDITOR)
	qt5_use_modules(TestTask1 Core Charts Gui OpenGL Widgets)
endif (EDITOR)

#find_package(OpenGL REQUIRED)
#target_link_libraries(TestTask1 ${OPENGL_LIBRARIES} ${GLU_LIBRARY})

cmake_minimum_required(VERSION 3.5)

project(Viewer3D LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH /opt/Qt/5.15.2/gcc_64/bin)
set(QT_QMAKE_EXECUTABLE /opt/Qt/5.15.2/gcc_64/bin/qmake)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools OpenGL REQUIRED)

set(TS_FILES Viewer3D_el_GR.ts)
message(${TS_FILES})
set(PROJECT_SOURCES
	main.cpp
	mainwindow.cpp
	mainwindow.h
	scene3d.cpp
	scene3d.h
	stb_image.h
	${TS_FILES}
)


if(ANDROID)
	add_library(Viewer3D SHARED
		${PROJECT_SOURCES}
	)
else()
	add_executable(Viewer3D
		${PROJECT_SOURCES}
	)
endif()

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
message(${QM_FILES})
target_link_libraries(Viewer3D PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::OpenGL)


# copy folder resources and its contents
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release")
	message(STATUS "Build type not specified: Use Release by default")
endif(NOT CMAKE_BUILD_TYPE)

file(GLOB files "resources/**/*")
foreach(file ${files})
	get_filename_component(name ${file} NAME)
	get_filename_component(folder ${file} PATH)
	string(REPLACE "${CMAKE_SOURCE_DIR}/" "" folder "${folder}")
	message(STATUS "path to copy = ${folder}/${name}")
	if (MSVC)
		configure_file(${file} "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${folder}/${name}" COPYONLY)
	else()
		configure_file(${file} "${CMAKE_CURRENT_BINARY_DIR}/${folder}/${name}" COPYONLY)
	endif()
endforeach()

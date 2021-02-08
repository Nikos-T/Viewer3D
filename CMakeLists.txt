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
	shaderprogram.cpp
	shaderprogram.h
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

# resources
file(GLOB files "resources/*")
foreach(file ${files})
	get_filename_component(name ${file} NAME)
	message(${name})
	configure_file(${file} resources/${name} COPYONLY)
endforeach()

# shaders
file(GLOB files "shaders/*")
foreach(file ${files})
	get_filename_component(name ${file} NAME)
	configure_file(${file} shaders/${name}. COPYONLY)
endforeach()

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
message(${QM_FILES})
target_link_libraries(Viewer3D PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::OpenGL)

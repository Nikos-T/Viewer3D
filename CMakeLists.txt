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


# Define the link libraries
#target_link_libraries(${PROJECT_NAME} ${LIBS})
#link_directories(${CMAKE_SOURCE_DIR}/lib)
#
set(TS_FILES Viewer3D_el_GR.ts)
message(${TS_FILES})

# Add .lib files
link_directories(${CMAKE_SOURCE_DIR}/lib)

# Add source files
file(GLOB_RECURSE SOURCE_FILES
        ${CMAKE_SOURCE_DIR}/src/*.c
        ${CMAKE_SOURCE_DIR}/src/*.cpp)

# Add header files
file(GLOB_RECURSE HEADER_FILES
        ${CMAKE_SOURCE_DIR}/src/*.h
        ${CMAKE_SOURCE_DIR}/src/*.hpp)


# Configure assets header file
configure_file(src/helpers/RootDir.h.in src/helpers/RootDir.h)
# Define the include DIRs
include_directories(${CMAKE_BINARY_DIR}/src)

if(ANDROID)
	add_library(Viewer3D SHARED
                ${HEADER_FILES} ${SOURCE_FILES}
	)
else()
	add_executable(Viewer3D
                ${HEADER_FILES} ${SOURCE_FILES}
	)
endif()

# We need a CMAKE_DIR with some code to find external dependencies
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

#######################################
# LOOK for the packages that we need! #
#######################################
#
#
# GLM
find_package(GLM REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")

# ASSIMP
find_package(ASSIMP REQUIRED)
message(STATUS "Found ASSIMP at ${ASSIMP_INCLUDE_DIR}")
if(ASSIMP_FOUND)
    include_directories(${ASSIMP_INCLUDE_DIR})
endif()

set(LIBS assimp)

# Define the include DIRs
include_directories(
        "${CMAKE_SOURCE_DIR}/src"
        "${CMAKE_SOURCE_DIR}/include"
)
#
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
	configure_file(${file} shaders/${name} COPYONLY)
endforeach()

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
message(${QM_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::OpenGL ${LIBS})

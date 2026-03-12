cmake_minimum_required(VERSION 3.10)

# Set C++ standard to 17
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS "${CXX_FLAGS}")
message(STATUS "Final compile flags: ${CMAKE_CXX_FLAGS}")

project(playback)
if(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include")
endif()
string(REPLACE "-ffloat-store" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# Try to find PCL, but don't fail if not found
find_package(PCL 1.10 QUIET)
if(PCL_FOUND)
    message(STATUS "PCL found: ${PCL_VERSION}")
    include_directories(${CMAKE_SOURCE_DIR}/include)
    include_directories(${PCL_INCLUDE_DIRS})
    link_directories(${PCL_LIBRARY_DIRS})
    add_definitions(${PCL_DEFINITIONS})
    list(REMOVE_ITEM PCL_LIBRARIES "vtkproj4")
    set(PCL_LIBRARIES_FOUND ${PCL_LIBRARIES})
else()
    message(STATUS "PCL not found - building without visualization")
    set(PCL_LIBRARIES_FOUND "")
endif()

include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/Eigen)

# Headless version (no GUI)
add_executable (ukf_highway
    src/main.cpp
    src/ukf.cpp
    src/tools.cpp
)

# GUI version (requires PCL)
if(PCL_FOUND)
    add_executable (ukf_highway_gui
        src/main_gui.cpp
        src/ukf.cpp
        src/tools.cpp
        src/render/render.cpp
    )
    target_link_libraries (ukf_highway_gui ${PCL_LIBRARIES_FOUND})
endif()

if(PCL_FOUND)
    target_link_libraries (ukf_highway ${PCL_LIBRARIES_FOUND})
endif()

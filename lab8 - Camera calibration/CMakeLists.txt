cmake_minimum_required(VERSION 3.10)

project(lab8)

include_directories(include)

add_library(lab8
    include/main_utils.h
    src/main_utils.cpp
    )

add_executable(camera_calib
    src/main.cpp
    )

target_link_libraries(camera_calib lab8)

find_package(OpenCV 4 REQUIRED)
if(OpenCV_FOUND)
    message(STATUS "OpenCV library found.")
    include_directories(${OpenCV_INCLUDE_DIRS})
    target_link_libraries(camera_calib
        opencv_highgui 
        opencv_core
        opencv_imgcodecs
        #opencv_imgproc
        #opencv_features2d
        opencv_calib3d
        )
else()
    message(FATAL_ERROR "OpenCV library not found.")
endif()

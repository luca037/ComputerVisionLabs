cmake_minimum_required(VERSION 3.10)

project(lab7)

include_directories(include)

add_library(lab7
    include/panoramic_utils.h
    include/main_utils.h
    src/panoramic_utils.cpp
    src/main_utils.cpp
    )

add_executable(img_stitching
    src/main.cpp
    )

target_link_libraries(img_stitching lab7)

find_package(OpenCV 4 REQUIRED)
if(OpenCV_FOUND)
    message(STATUS "OpenCV library found.")
    include_directories(${OpenCV_INCLUDE_DIRS})
    target_link_libraries(img_stitching
        opencv_highgui 
        opencv_core
        opencv_imgcodecs
        opencv_imgproc
        opencv_features2d
        opencv_calib3d
        )
else()
    message(FATAL_ERROR "OpenCV library not found.")
endif()

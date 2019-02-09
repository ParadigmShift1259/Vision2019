cmake_minimum_required(VERSION 2.8)
project(VisionProcessing)

set (CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} /usr/local/frc/share/OpenCV)

find_package(OpenCV REQUIRED)
#find_package(wpilib REQUIRED)
#include_directories(${/usr/local/frc/include})

set (CMAKE_CXX_FLAGS "-L/usr/local/frc/lib -lntcore -I/usr/local/frc/include")

add_executable( VisionProcessing
                VisionProcessing.cpp
                CameraWrapper.cpp
                OffBoardComms.cpp 
                ProcessingBase.cpp 
                ProcessingHatch.cpp 
                ProcessingRetro.cpp 
                ProcessingLine.cpp 
                ProcessingCargo.cpp )
#target_link_libraries( VisionTest ${/usr/local/frc/lib/})
#target_link_libraries( VisionTest ${/usr/local/frc/lib})
target_link_libraries(VisionProcessing ${OpenCV_LIBS})

#message(STATUS "OpenCV libs: " {${OpenCV_LIBS})

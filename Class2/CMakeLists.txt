cmake_minimum_required (VERSION 2.8) 
project (Class2)
list(APPEND CMAKE_PREFIX_PATH "~/Desktop/raspicam-0.1.3/build")

find_package(raspicam REQUIRED)
find_package(OpenCV)
# Locate libraries and headers
find_package(WiringPi REQUIRED) 
find_package(Threads REQUIRED) 
#list(APPEND CMAKE_PREFIX_PATH "~/Desktop/JHPWMDriver/build")
#find_package(JHPWMDriver REQUIRED)
#include_directories("~/Desktop/JHPWMDriver/src"
#link_directories("~/Desktop/JHPWMDriver/src")

# Include headers
include_directories(${WIRINGPI_INCLUDE_DIRS}) 
include_directories("~/Desktop/Advanced-Training-Program/Class2)



IF  ( OpenCV_FOUND AND raspicam_CV_FOUND)
	MESSAGE(STATUS "COMPILING OPENCV TESTS")
	add_executable (Class2 Class2.cpp JHPWMPCA9685_RPI.cpp)  
	target_link_libraries (Class2 ${raspicam_CV_LIBS} ${WIRINGPI_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
ELSE()
	MESSAGE(FATAL_ERROR "OPENCV NOT FOUND IN YOUR SYSTEM")
ENDIF()

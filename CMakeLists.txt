cmake_minimum_required(VERSION 2.8)
PROJECT( AK_Calibration )

SET( CMAKE_CXX_COMPILER "g++" )
SET( CMAKE_BUILD_TYPE Debug )
SET( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin )
SET( LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib )

INCLUDE_DIRECTORIES( ${PROJECT_SOURCE_DIR}/include )
INCLUDE_DIRECTORIES( ${PROJECT_SOURCE_DIR}/include/Tools )
LINK_DIRECTORIES( ${PROJECT_SOURCE_DIR}/lib )

# Set cmake prefix path to enable cmake to find freenect2
# set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} $ENV{HOME}/freenect2/lib/cmake/freenect2)
# find_package(freenect2 REQUIRED)

ADD_SUBDIRECTORY( ${PROJECT_SOURCE_DIR}/src )




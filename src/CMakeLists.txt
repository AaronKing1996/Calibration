FIND_PACKAGE( OpenCV REQUIRED )

# Eigen
FIND_PACKAGE(Eigen3)
INCLUDE_DIRECTORIES(${EIGEN3_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})

add_library( Tools Tools/AK_cameraParam.cpp Tools/AK_paramLoader.cpp )
target_link_libraries( Tools ${OpenCV_LIBS} )

add_executable( Main main.cpp )
target_link_libraries( Main Tools )

#add_library( calibrationBase calibrationBase.cpp )

#add_executable( CameraCalibration CameraCalibration.cpp )
#target_link_libraries( CameraCalibration calibrationBase ${OpenCV_LIBS} )

#add_executable( CaculateCameraMatrix CaculateCameraMatrix.cpp )
#target_link_libraries( CaculateCameraMatrix  CameraMatrix ${OpenCV_LIBS} )



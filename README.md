# Calibration
calibrate the depth camera and rgb camera

# Usage

AK_CameraParam akCameraParam;
AK_CAMERA_INFO_Eigen colorCameraInfo = akCameraParam.extractCameraInnerParam(color_image_file, boardInfo, true);
AK_CAMERA_INFO_Eigen depthCameraInfo = akCameraParam.extractCameraInnerParam(depth_image_file, boardInfo, true);
AK_CAMERA_EXTR_PARAM extrParam = akCameraParam.extractCameraExtrParam(colorCameraInfo, depthCameraInfo);

...
More detail can be seen in main.cpp :)

# Result


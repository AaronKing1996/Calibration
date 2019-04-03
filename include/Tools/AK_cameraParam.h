#ifndef AK_CAMERAPARAM_H_
#define AK_CAMERAPARAM_H_

#include "Tools/coutColor.h"

// Eigen
#include "Eigen/Dense"
#include "Eigen/Core"

// OpenCV
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include <opencv2/highgui/highgui.hpp>

#include "Tools/AK_common.h"
#include <fstream>
#include <string>

// chessboard info
struct AK_CHESSBOARD_INFO {
    cv::Size board_size;
    cv::Size square_size;
};

// camera info
struct AK_CAMERA_INFO_OpenCV {
	cv::Mat camera_matrix33; 
	cv::Mat distCoeffs;
	std::vector<cv::Mat> t_vec;
	std::vector<cv::Mat> r_vec;
};

// camera info
struct AK_CAMERA_INFO_Eigen {
	void operator = (const AK_CAMERA_INFO_OpenCV camera_info) 
	{
	    cv::Mat rotation_matrix = cv::Mat(3,3,CV_32FC1, cv::Scalar::all(0));
	    cv::Rodrigues(camera_info.r_vec[0],rotation_matrix);

	    for (int i=0; i<3; i++) {
		    for (int j=0; j<3; j++) {
			    this->camera_matrix33(i,j) = camera_info.camera_matrix33.at<double>(i,j);
			    this->r_matrix33(i,j) = rotation_matrix.at<double>(i,j);
		    }		
	    }
	    
	    for (int j=0; j<6; j++) {
		    this->distCoeffs(0,j) = camera_info.distCoeffs.at<double>(0,j);
	    }	
	    
	    for (int k=0; k<3; k++) {
		    this->t_vec(k,0) = camera_info.t_vec[0].at<double>(0,k);
	    }
	}
  
	Eigen::Matrix3d camera_matrix33; 
	Eigen::Matrix<double,1,6> distCoeffs;
	Eigen::Matrix3d r_matrix33;
	Eigen::Matrix<double,3,1> t_vec;
	// Vector3d r_vec;
};

// extrinsic param
struct AK_CAMERA_EXTR_PARAM {
	Eigen::Matrix3d r_matrix33;
	Eigen::Matrix<double,3,1> t_vec;
};

class AK_CameraParam {
public:
    AK_CameraParam();
    ~AK_CameraParam();
    
    /**
     * @brief 
     * @param dataFilePath 
     */
    AK_CAMERA_INFO_Eigen extractCameraInnerParam(std::string imageDataFile, AK_CHESSBOARD_INFO boardInfo ,bool printData);
    AK_CAMERA_EXTR_PARAM extractCameraExtrParam(AK_CAMERA_INFO_Eigen depth_camera, AK_CAMERA_INFO_Eigen rgb_camera);
    cv::Mat correctImage(cv::Mat colorMat, cv::Mat depthMat, AK_CAMERA_INFO_Eigen camera_color_info, AK_CAMERA_INFO_Eigen camera_depth_info, AK_CAMERA_EXTR_PARAM camera_extr);
    
private:
    AK_CAMERA_INFO_Eigen camera_info_eigen;
};

#endif
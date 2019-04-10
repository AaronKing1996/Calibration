#include "Tools/AK_cameraParam.h"

// remove the carriage return of "getLine"
#ifdef linux
std::string LTrim(const std::string& str) {
    return str.substr(str.find_first_not_of(" \n\r\t"));
}

std::string RTrim(const std::string& str) {
    return str.substr(0, str.find_last_not_of(" \n\r\t") + 1);
}

std::string trim(const std::string& str)   
{  
    return LTrim(RTrim(str));   
}  
#endif

AK_CameraParam::AK_CameraParam() {}

AK_CameraParam::~AK_CameraParam() {}

AK_CAMERA_INFO_Eigen AK_CameraParam::extractCameraInnerParam(std::__cxx11::string imageDataFile, AK_CHESSBOARD_INFO boardInfo, bool printData)
{
    AK_CAMERA_INFO_Eigen camera_info_eigen;
    std::string imageName = "";
    
    std::ifstream fin( imageDataFile.c_str() );
    if (!fin) { 
	std::cout << RED << "Error:" << WHITE << "can't open file:" << imageDataFile << std::endl;
	return camera_info_eigen;
    }
  
    std::vector< std::vector<cv::Point2f> > corners_seq;
    std::vector<cv::Point2f> cornerBuf;	// corner cache
    cv::Size image_size;
    int image_count = 0;
    
    while ( std::getline(fin,imageName) ) {

	image_count++;
	if (printData)
		std::cout << BLUE << "getted image " << image_count-1 << " named: " << imageName << WHITE << std::endl;
	
	#ifdef linux
		const std::string completeImagePath = trim(imageName);
	#endif
	
	#ifdef _WINDOWS_
		const std::string completeImagePath = imageName;
	#endif
	
	cv::Mat imageInput = cv::imread(completeImagePath);
	if (!imageInput.data) {
		std::cout << RED << "Error:" << WHITE << completeImagePath << " has not data" << std::endl;
		return camera_info_eigen;
	} else if (image_count == 1) {
		image_size.width = imageInput.cols;
		image_size.height = imageInput.rows;
		// cout<<BLUE<<"image_width = "<<WHITE<<image.cols<<BLUE<<"image_height = "<<WHITE<<image.rows<<endl;

	}
		
	if (imageInput.cols == 0) {
		std::cout << RED << "Error reading:" << completeImagePath << std::endl;	
	}
	
	// extract corners
	if (0 == cv::findChessboardCorners(imageInput,boardInfo.board_size, cornerBuf)) {
	  
		std::cout << YELLOW << "Can't find chessboard corners " << image_count-1 << WHITE << std::endl;
		continue;
		
	} else {
		cv::Mat image_gray;
		cv::cvtColor(imageInput, image_gray, CV_RGB2GRAY);
		// find the accurate position of subpix
		// cv::find4QuadCornerSubpix(image_gray, cornerBuf, boardInfo.square_size);
		corners_seq.push_back(cornerBuf);
		
		if (printData) {
		    cv::drawChessboardCorners(image_gray,boardInfo.board_size,cornerBuf,true);
		    cv::imshow("Find chessboard corners", image_gray);
		    std::cout << YELLOW << "Find chessboard corners " << image_count-1 << WHITE << std::endl;
		    
		    cv::waitKey(100);
		}
	}	
				
    }	
	
    std::cout << GREEN << "Finish finding chessboard corners" << WHITE << std::endl;

    std::cout<<GREEN<<"Start calibrating..."<<WHITE<<std::endl;	
    
    // init chessboard
    std::vector< std::vector<cv::Point3f> > points_coordinate;
    int i,j,t;
    for (t=0; t<image_count; t++) {
	std::vector<cv::Point3f> tempPointSet;
	for (i=0; i<boardInfo.board_size.height; i++) 
	{
		for (j=0; j<boardInfo.board_size.width; j++) 
		{
			cv::Point3f realPoint;
			/* 假设标定板放在世界坐标系中z=0的平面上 */
			realPoint.x = i*boardInfo.square_size.width;
			realPoint.y = j*boardInfo.square_size.height;
			realPoint.z = 0;
			tempPointSet.push_back(realPoint);
		}
	}
	points_coordinate.push_back(tempPointSet);
    }
	
    // start calibrate
    AK_CAMERA_INFO_OpenCV cameraInfo_OpenCV;
    cv::calibrateCamera(points_coordinate,corners_seq,image_size,cameraInfo_OpenCV.camera_matrix33,cameraInfo_OpenCV.distCoeffs,cameraInfo_OpenCV.r_vec,cameraInfo_OpenCV.t_vec,0);
    std::cout<<GREEN<<"Finish calibrating"<<WHITE<<std::endl;

    camera_info_eigen = cameraInfo_OpenCV;	
	
    return camera_info_eigen;
}

AK_CAMERA_EXTR_PARAM AK_CameraParam::extractCameraExtrParam(AK_CAMERA_INFO_Eigen depth_camera, AK_CAMERA_INFO_Eigen rgb_camera)
{
    AK_CAMERA_EXTR_PARAM result;
    result.r_matrix33 = depth_camera.r_matrix33.inverse() * rgb_camera.r_matrix33;

    result.t_vec = rgb_camera.t_vec - result.r_matrix33 * depth_camera.t_vec;
    return result;
}

cv::Mat AK_CameraParam::correctImage(cv::Mat colorMat, cv::Mat depthMat, AK_CAMERA_INFO_Eigen camera_color_info, AK_CAMERA_INFO_Eigen camera_depth_info, AK_CAMERA_EXTR_PARAM camera_extr)
{
    cv::Mat correctMat(depthMat.rows, depthMat.cols, CV_8UC3, cv::Scalar(0, 0, 0));
    
    if (colorMat.empty() || depthMat.empty()) {
	std::cout << "Correct Image ... empty ! " << std::endl;
	return correctMat;
    }
    
    for (size_t y=0; y < depthMat.rows; y++) {
	
	unsigned char* row_ptr = depthMat.ptr<unsigned char> ( y );  // row_ptr
	for (size_t x=0; x < depthMat.cols; x++) {
	    unsigned char* data_ptr = &row_ptr[ x * 3 ]; // data_ptr 
	    unsigned char data = data_ptr[0]; 
	    // unsigned short realDepth = (data & 0xfff8) >> 3;
	    unsigned int realDepth = data_ptr[0] * 0xffff + data_ptr[1] * 0xff + data_ptr[2];
	    
	    unsigned char* d_row_ptr = correctMat.ptr<unsigned char> ( y );  // row_ptr
	    unsigned char* d_data_ptr = &d_row_ptr[ x*correctMat.channels() ]; // data_ptr
	    
	    // (1)
	    Eigen::Matrix<double,3,1> d_p_position;
	    d_p_position << double(x) * double(realDepth),double(y) * double(realDepth),double(realDepth);
	    // (2)
	    Eigen::Matrix<double,3,1> realWorldPosition = camera_depth_info.camera_matrix33.inverse() * d_p_position;
	    // (3)
	    Eigen::Matrix<double,3,1> r_c_position = camera_extr.r_matrix33 * realWorldPosition + camera_extr.t_vec;
	    // (4)	
	    Eigen::Matrix<double,3,1> r_p_position = camera_color_info.camera_matrix33 * r_c_position;
	
	    int c_z = int(r_p_position(2,0));			
	    int c_x = int(r_p_position(0,0)/r_p_position(2,0));
	    int c_y = int(r_p_position(1,0)/r_p_position(2,0));

	    if (c_x > colorMat.cols || c_y > colorMat.rows) {
		// each channel
		for ( int c = 0; c != correctMat.channels(); c++ )
		{
			d_data_ptr[c] = 0; 
		}
		continue;
	    }
	    
	    // rgb data
	    unsigned char* c_row_ptr = colorMat.ptr<unsigned char> ( c_y );
	    unsigned char* c_data_ptr = &c_row_ptr[ c_x * colorMat.channels() ];
	
	    // each channel
	    for ( int c = 0; c != correctMat.channels(); c++ )
	    {
		d_data_ptr[c] = c_data_ptr[c]; 
	    }
	}
    }
    
    return correctMat;
}
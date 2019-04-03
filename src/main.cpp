#include <iostream>
#include "Tools/AK_paramLoader.h"
#include "Tools/AK_cameraParam.h"

int main() {
  
  std::string fileName = "../config/config.yaml";
  AK_ParamLoader akParamLoader(fileName);
  
  std::string color_image_file;
  akParamLoader.getParam("color_image_file", color_image_file);
  std::cout << color_image_file;
  std::string depth_image_file;
  akParamLoader.getParam("depth_image_file", depth_image_file);
  std::cout << depth_image_file;
  
  int boardSizeW, boardSizeH;
  akParamLoader.getParam("boardW", boardSizeW);
  akParamLoader.getParam("boardH", boardSizeH);
  
  int squareW, squareH;
  akParamLoader.getParam("squareW", squareW);
  akParamLoader.getParam("squareH", squareH);
  
  std::string correctingColorImagePath, correctingDepthImagePath;
  akParamLoader.getParam("correcting_color_image", correctingColorImagePath);
  akParamLoader.getParam("correcting_depth_image", correctingDepthImagePath);
  
  AK_CHESSBOARD_INFO boardInfo = {cv::Size(boardSizeW, boardSizeH), cv::Size(squareW, squareH)};	//棋盘格信息，
  AK_CameraParam akCameraParam;
  AK_CAMERA_INFO_Eigen colorCameraInfo = akCameraParam.extractCameraInnerParam(color_image_file, boardInfo, true);
  AK_CAMERA_INFO_Eigen depthCameraInfo = akCameraParam.extractCameraInnerParam(depth_image_file, boardInfo, true);
  AK_CAMERA_EXTR_PARAM extrParam = akCameraParam.extractCameraExtrParam(colorCameraInfo, depthCameraInfo);
  
  std::cout<<YELLOW<<"+++++++++++++++++++++++++++Print Matrix+++++++++++++++++++++++++++++++++"<<WHITE<<std::endl;
  std::cout.precision(10);
  std::cout<<GREEN<<"--> camera_inner_matrix33 <--\n"<<WHITE<<std::fixed<<colorCameraInfo.camera_matrix33<<std::endl;
  std::cout<<GREEN<<"-->   camera_distortion   <--\n"<<WHITE<<colorCameraInfo.distCoeffs<<std::endl;
  std::cout<<GREEN<<"-->   camera_r_matrix33   <--\n"<<WHITE<<colorCameraInfo.r_matrix33<<std::endl;
  std::cout<<GREEN<<"-->     camera_t_vec      <--\n"<<WHITE<<colorCameraInfo.t_vec<<std::endl;
  std::cout<<YELLOW<<"+++++++++++++++++++++++++++Print Matrix+++++++++++++++++++++++++++++++++"<<WHITE<<std::endl;
  std::cout.precision(10);
  std::cout<<GREEN<<"--> camera_inner_matrix33 <--\n"<<WHITE<<std::fixed<<depthCameraInfo.camera_matrix33<<std::endl;
  std::cout<<GREEN<<"-->   camera_distortion   <--\n"<<WHITE<<depthCameraInfo.distCoeffs<<std::endl;
  std::cout<<GREEN<<"-->   camera_r_matrix33   <--\n"<<WHITE<<depthCameraInfo.r_matrix33<<std::endl;
  std::cout<<GREEN<<"-->     camera_t_vec      <--\n"<<WHITE<<depthCameraInfo.t_vec<<std::endl;
  
  std::cout<<GREEN<<"==============R==============\n"<<WHITE<<extrParam.r_matrix33<<std::endl;
  std::cout<<GREEN<<"==============T==============\n"<<WHITE<<extrParam.t_vec<<std::endl;
  std::cout<<YELLOW<<"++++++++++++++++++++++++++++++++CAPTURE++++++++++++++++++++++++++++++++++"<<WHITE<<std::endl;
	
  std::cout<<YELLOW<<"++++++++++++++++++++++++++++++CORRECTION+++++++++++++++++++++++++++++++++"<<WHITE<<std::endl;
  std::cout<<GREEN<<"start correcting rgb and dep images..."<<WHITE<<std::endl;
	
  cv::Mat colorImage = cv::imread(correctingColorImagePath);
  cv::Mat depthImage = cv::imread(correctingDepthImagePath);
  cv::Mat correctedImage = akCameraParam.correctImage(colorImage, depthImage, colorCameraInfo, depthCameraInfo, extrParam);
  
  cv::namedWindow( "color",  CV_WINDOW_AUTOSIZE );
  cv::namedWindow( "depth",  CV_WINDOW_AUTOSIZE );
  cv::namedWindow( "corrected",  CV_WINDOW_AUTOSIZE );
  cv::imshow("color", colorImage);
  cv::imshow("depth", depthImage);
  cv::imshow("corrected", correctedImage);
  
  cv::waitKey(0);
  return 0;
}

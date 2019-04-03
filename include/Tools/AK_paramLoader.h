#ifndef AK_PARAMLOADER_H_
#define AK_PARAMLOADER_H_

#include <opencv2/opencv.hpp>
#include <iostream>

class AK_ParamLoader {
public:
  AK_ParamLoader(std::string _fileName, cv::FileStorage::Mode mode = cv::FileStorage::READ );
  ~AK_ParamLoader();
    
  template<class Type>
  void getParam(const char* _node, Type & _item, cv::FileStorage::Mode mode = cv::FileStorage::READ)
  {
      if (mode == cv::FileStorage::READ) 
      {
	_item = (Type)fileStorage[_node];
      } 
      else if (mode == cv::FileStorage::WRITE) 
      {
	
      }
  }
  
  void setFileName(std::string _fileName, cv::FileStorage::Mode mode = cv::FileStorage::READ);
  
private:
  
  std::string fileName;
  cv::FileStorage fileStorage;
};

#endif
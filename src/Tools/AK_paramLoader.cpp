#include <Tools/AK_paramLoader.h>

AK_ParamLoader::AK_ParamLoader(std::string _fileName, cv::FileStorage::Mode mode ) 
{
    this->fileName = _fileName;
    fileStorage.open(fileName, mode);
}

AK_ParamLoader::~AK_ParamLoader() {
    fileStorage.release();
}

void AK_ParamLoader::setFileName(std::string _fileName, cv::FileStorage::Mode mode)
{
    fileStorage.open(fileName, mode);
}
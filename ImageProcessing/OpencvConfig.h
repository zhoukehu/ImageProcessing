//OpenCVconfig.h  
#pragma once  
#include <opencv2/opencv.hpp>    

using namespace cv;

#define CV_VERSION_ID CVAUX_STR(CV_MAJOR_VERSION) \
                      CVAUX_STR(CV_MINOR_VERSION) \
                      CVAUX_STR(CV_SUBMINOR_VERSION)

#ifdef _DEBUG
#define cvLIB(name) "opencv_" name CV_VERSION_ID "d"
#else
#define cvLIB(name) "opencv_" name CV_VERSION_ID
#endif

#if CV_MAJOR_VERSION==2
	#pragma comment( lib, cvLIB("core") )    
	#pragma comment( lib, cvLIB("features2d") )  
	#pragma comment( lib, cvLIB("flann") )    
	#pragma comment( lib, cvLIB("gpu") ) 
	#pragma comment( lib, cvLIB("calib3d") )    
	#pragma comment( lib, cvLIB("contrib") )    
	#pragma comment( lib, cvLIB("highgui") )    
	#pragma comment( lib, cvLIB("imgproc") )    
	#pragma comment( lib, cvLIB("legacy") )    
	#pragma comment( lib, cvLIB("ml") )    
	#pragma comment( lib, cvLIB("nonfree") )    
	#pragma comment( lib, cvLIB("legacy") )    
	#pragma comment( lib, cvLIB("objdetect") )    
	#pragma comment( lib, cvLIB("photo") )    
	#pragma comment( lib, cvLIB("stitching") )    
	#pragma comment( lib, cvLIB("ts") )    
	#pragma comment( lib, cvLIB("superres") )    
	#pragma comment( lib, cvLIB("video") )  
	#pragma comment( lib, cvLIB("videostab") )  
#endif

#if CV_MAJOR_VERSION==3
	//下面代码适用官方编译的DLL，不包含contrib
	//#pragma comment( lib, cvLIB("world") ) 
	//下面代码适用自己编译的DLL，包含contrib
	#pragma comment( lib, cvLIB("aruco") )    
	#pragma comment( lib, cvLIB("bgsegm") )  
	#pragma comment( lib, cvLIB("bioinspired") )
	#pragma comment( lib, cvLIB("calib3d") )    
	#pragma comment( lib, cvLIB("ccalib") ) 
	#pragma comment( lib, cvLIB("core") )    
	#pragma comment( lib, cvLIB("datasets") )    
	#pragma comment( lib, cvLIB("dnn") ) 
	#pragma comment( lib, cvLIB("dnn_objdetect") )
	#pragma comment( lib, cvLIB("dpm") )    
	#pragma comment( lib, cvLIB("face") )    
	#pragma comment( lib, cvLIB("features2d") )    
	#pragma comment( lib, cvLIB("flann") )    
	#pragma comment( lib, cvLIB("fuzzy") )  
	#pragma comment( lib, cvLIB("hfs") )
	#pragma comment( lib, cvLIB("highgui") )   
	#pragma comment( lib, cvLIB("img_hash") )
	#pragma comment( lib, cvLIB("imgcodecs") )    
	#pragma comment( lib, cvLIB("imgproc") )    
	#pragma comment( lib, cvLIB("line_descriptor") )    
	#pragma comment( lib, cvLIB("ml") )    
	#pragma comment( lib, cvLIB("objdetect") )  
	#pragma comment( lib, cvLIB("optflow") ) 
	#pragma comment( lib, cvLIB("phase_unwrapping") ) 
	#pragma comment( lib, cvLIB("photo") ) 
	#pragma comment( lib, cvLIB("plot") ) 
	#pragma comment( lib, cvLIB("reg") ) 
	#pragma comment( lib, cvLIB("rgbd") )
	#pragma comment( lib, cvLIB("saliency") ) 
	#pragma comment( lib, cvLIB("shape") ) 
	#pragma comment( lib, cvLIB("stereo") ) 
	#pragma comment( lib, cvLIB("stitching") ) 
	#pragma comment( lib, cvLIB("structured_light") ) 
	#pragma comment( lib, cvLIB("superres") ) 
	#pragma comment( lib, cvLIB("surface_matching") ) 
	#pragma comment( lib, cvLIB("text") ) 
	#pragma comment( lib, cvLIB("tracking") ) 
	#pragma comment( lib, cvLIB("video") ) 
	#pragma comment( lib, cvLIB("videoio") ) 
	#pragma comment( lib, cvLIB("videostab") ) 
	#pragma comment( lib, cvLIB("xfeatures2d") ) 
	#pragma comment( lib, cvLIB("ximgproc") ) 
	#pragma comment( lib, cvLIB("xobjdetect") ) 
	#pragma comment( lib, cvLIB("xphoto") ) 

#endif
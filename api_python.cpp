#include "api_python.h"
#include "mngSdk.h"
#include "mat_warper.h"



int face_init()
{	
	return CMngSdkS->initSDK();
}

void face_input_image(py::array_t<unsigned char>& input)
{	
	cv::Mat input_mat = numpy_uint8_3c_to_cv_mat(input);
	if (input_mat.empty()) {
		cout << "it's empty to call" << endl;
		return;
	}
	CMngSdkS->inputImage(input_mat);
}

bool face_check_person()
{
	bool bResult = CMngSdkS->check_person();
	return bResult;
}


std::string face_get_face_info()
{
	return CMngSdkS->get_face_result();
}
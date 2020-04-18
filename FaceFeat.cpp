#include "FaceFeat.h"



CFaceFeat::CFaceFeat() :image_width(0)
, image_heigh(0)
{
	faceFeature = std::make_shared<ASF_FaceFeature>();
	faceFeature->feature = nullptr;
	faceFeature->featureSize = 0;	
}


CFaceFeat::~CFaceFeat()
{
	if (faceFeature->feature)
		SafeFree(faceFeature->feature);
}

bool CFaceFeat::getFeatureOne(MHandle handle, cv::Mat & mat, int & res)
{
	bool bResult = false;

	cv::Mat & image1 = mat;

	int width = image1.cols;
	int heigh = image1.rows;
	int pad = width % 4;
	if (0 != pad) {
		width -= pad;
		cv::resize(image1, image1, cv::Size(width, heigh));
	}

	ASF_MultiFaceInfo detectedFaces1 = { 0 };//多人脸信息；
	ASF_SingleFaceInfo SingleDetectedFaces1 = { 0 };
	ASF_FaceFeature feature1 = { 0 };
	ASF_FaceFeature & copyfeature1 = *faceFeature;
	res = ASFDetectFaces(handle, image1.cols, image1.rows, ASVL_PAF_RGB24_B8G8R8, (MUInt8*)image1.data, &detectedFaces1);
	if ((MOK == res) && (detectedFaces1.faceNum > 0 )){
		SingleDetectedFaces1.faceRect.left = detectedFaces1.faceRect[0].left;
		SingleDetectedFaces1.faceRect.top = detectedFaces1.faceRect[0].top;
		SingleDetectedFaces1.faceRect.right = detectedFaces1.faceRect[0].right;
		SingleDetectedFaces1.faceRect.bottom = detectedFaces1.faceRect[0].bottom;
		SingleDetectedFaces1.faceOrient = detectedFaces1.faceOrient[0];

		//单人脸特征提取
		res = ASFFaceFeatureExtract(handle, image1.cols, image1.rows, ASVL_PAF_RGB24_B8G8R8, (MUInt8*)image1.data, &SingleDetectedFaces1, &feature1);
		if (res == MOK)
		{
			//拷贝feature
			copyfeature1.featureSize = feature1.featureSize;
			copyfeature1.feature = (MByte *)malloc(feature1.featureSize);
			memset(copyfeature1.feature, 0, feature1.featureSize);
			memcpy(copyfeature1.feature, feature1.feature, feature1.featureSize);

			bResult = true;		

		}
		else
			printf("ASFFaceFeatureExtract 1 fail: %xd\n", res);
	}
	else {
		printf("ASFDetectFaces 1 fail: %d\n", res);
	}

	return bResult;
}

bool CFaceFeat::buildFeature(MHandle handle, string & strFilename, int & res)
{
	bool bResult = false;

	cv::Mat image1 = cv::imread(strFilename.c_str());
	if (image1.empty()) {
		cout << "it's empty for call imread() " << endl;
		return bResult;
	}

	bResult = getFeatureOne(handle, image1, res);
	if (bResult) {
		string::size_type pos = strFilename.rfind("/");
		if (string::npos != pos) {
			pos += 1;
			string::size_type size = strFilename.size();
			faceName = strFilename.substr(pos, size - pos);
		}
		else {
			faceName = strFilename;
		}

		//cout << "face_name :" << faceName << endl;
	}	
	return bResult;
}

bool CFaceFeat::verifyFace(MHandle handle, ASF_FaceFeature & faceF)
{
	bool bResult = false;
	MFloat confidenceLevel = -1;
	ASF_FaceFeature &copyfeature = * (this->faceFeature);
	printf("feature_size : %d\n", copyfeature.featureSize);

	int res = ASFFaceFeatureCompare(handle, &copyfeature, &faceF, &confidenceLevel);
	if (res != MOK)
		printf("ASFFaceFeatureCompare fail: %d\n", res);
	else {
		printf("ASFFaceFeatureCompare sucess: %lf\n", confidenceLevel);
		if (confidenceLevel >= FACE_THRESOLD_VALUE) {
			bResult = true;
		}
	}
	
	return bResult;
}

string CFaceFeat::getName()
{
	return faceName;
}


bool CFaceFeat::getFeatureMulti(MHandle handle, cv::Mat & mat, int & res)
{
	bool bResult = false;
	cv::Mat & image1 = mat;

	int width = image1.cols;
	int heigh = image1.rows;
	int pad = width % 4;
	if (0 != pad) {
		width -= pad;
		cv::resize(image1, image1, cv::Size(width, heigh));
	}
	

	image_width = image1.cols;
	image_heigh = image1.rows;

	ASF_MultiFaceInfo detectedFaces1 = { 0 };//多人脸信息；
	ASF_SingleFaceInfo SingleDetectedFaces1 = { 0 };
	ASF_FaceFeature feature1 = { 0 };	
	res = ASFDetectFaces(handle, image1.cols, image1.rows, ASVL_PAF_RGB24_B8G8R8, (MUInt8*)image1.data, &detectedFaces1);
	if ((MOK == res) && (detectedFaces1.faceNum > 0)) {
		bResult = true;
		cout << "face_num :" << detectedFaces1.faceNum << endl;
		for (int i = 0; i < detectedFaces1.faceNum; i++) {
			SingleDetectedFaces1.faceRect.left = detectedFaces1.faceRect[i].left;
			SingleDetectedFaces1.faceRect.top = detectedFaces1.faceRect[i].top;
			SingleDetectedFaces1.faceRect.right = detectedFaces1.faceRect[i].right;
			SingleDetectedFaces1.faceRect.bottom = detectedFaces1.faceRect[i].bottom;
			SingleDetectedFaces1.faceOrient = detectedFaces1.faceOrient[i];

			//单人脸特征提取
			res = ASFFaceFeatureExtract(handle, image1.cols, image1.rows, ASVL_PAF_RGB24_B8G8R8, (MUInt8*)image1.data, &SingleDetectedFaces1, &feature1);
			if (res == MOK)
			{			
				auto faceItem = std::make_shared<CFaceItem>();
				faceItem->faceInfo = SingleDetectedFaces1;				

				ASF_FaceFeature & copyfeature1 = faceItem->faceFeat;
				copyfeature1.featureSize = feature1.featureSize;
				copyfeature1.feature = (MByte *)malloc(feature1.featureSize);
				memset(copyfeature1.feature, 0, feature1.featureSize);
				memcpy(copyfeature1.feature, feature1.feature, feature1.featureSize);

				vecFaceItem.push_back(faceItem);
				bResult = true;

				//MRECT & rect = faceItem->faceInfo.faceRect;
				//cout << "rect(" << rect.left << " " << rect.top << " " << rect.right << " " << rect.bottom << ")" << endl;

				
			}
			else
				printf("ASFFaceFeatureExtract 1 fail: %xd\n", res);
		}
	}
	else
	{
		printf("ASFDetectFaces 1 fail: %xd\n", res);
	}
	return bResult;
}


std::shared_ptr<ASF_FaceFeature> CFaceFeat::getFacePtr()
{
	return faceFeature;
}


std::vector<std::shared_ptr<CFaceItem> > & CFaceFeat::getAllFaceItem()
{
	return vecFaceItem;
}

void  CFaceFeat::getRect(int & width, int & heigh)
{
	width = image_width;
	heigh = image_heigh;
}


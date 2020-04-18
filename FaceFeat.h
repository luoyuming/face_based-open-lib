#pragma once
#include "faceItem.h"

class CFaceFeat
{
public:
	CFaceFeat();
	~CFaceFeat();
	bool buildFeature(MHandle handle, string & strFilename, int & res);
	bool getFeatureOne(MHandle handle, cv::Mat & mat, int & res);
	bool getFeatureMulti(MHandle handle, cv::Mat & mat, int & res);
	bool verifyFace(MHandle handle, ASF_FaceFeature & faceF);		
	string getName();
	std::shared_ptr<ASF_FaceFeature> getFacePtr();
	std::vector<std::shared_ptr<CFaceItem> > & getAllFaceItem();
	void getRect(int & width, int & heigh);
public:
	
private:
	std::shared_ptr<ASF_FaceFeature>			faceFeature;
	std::string									faceName;
	std::vector<std::shared_ptr<CFaceItem> >    vecFaceItem;
	int  image_width;
	int  image_heigh;
};


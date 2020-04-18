#pragma once
#include "Singleton.h"
#include "common.h"
using namespace std;

#include "FaceFeat.h"
#include "QueueInfo.h"
#include "faceResult.h"

#define APPID "BRfaACubGYD597izex7xhqhSdGfZdkqC1YYzzYBBJX72"
#define SDKKEY "aAqorEPQpHqbe3yGXvfsN4vRWnEN3yNTd5vSoEycw3T"



#define NSCALE 30 
#define FACENUM	10

static const std::string IMAGE_PATH = "face_image";
class CMngSdk : public SingletionEX<CMngSdk>
{
    SINGLETON_INIT_EX(CMngSdk);

    CMngSdk();
public:	
    int initSDK();
	void inputImage(cv::Mat & image_mat);
	bool check_person();
	string get_face_result();
private:
	void threadHandleCamera();

	void handleVerify(std::shared_ptr<CFaceFeat> & faceFeat, cv::Mat & mat);
	void handleMat(cv::Mat & mat);
	string buildFacePath();
private:
	int testFaceSDK();
	void testVerifyFace(vector<string> & vecFiles);
  
	bool get_all_files(const std::string& dir_in, std::vector<std::string> & files);
	string getPwd();
	int MakeDir(char* dirname);
	bool checkFaceDbEmpyt();
private:
	std::shared_ptr<CFaceResult>   m_faceResult;
	std::shared_ptr<CQueue>   m_matQueue;
	MHandle m_handle;	
	vector<std::shared_ptr<CFaceFeat> > m_vecFeatDb;
	std::mutex	m_lockFeatDb;
	bool m_sucess;
};

#define CMngSdkS  CMngSdk::getInstance()
#include "mngSdk.h"
#include "face_error.h"
#include "HandleJson.h"

CMngSdk::CMngSdk()
{
	m_sucess = false;
	m_matQueue = std::make_shared<CQueue>();
	m_faceResult = std::make_shared<CFaceResult>();
}


void CMngSdk::Quit()
{
	if (m_sucess) {
		//反初始化
		int res = ASFUninitEngine(m_handle);
		if (res != MOK)
			printf("ALUninitEngine fail: %d\n", (int)res);
		else
			printf("ALUninitEngine sucess: %d\n", (int)res);
	}
}

bool CMngSdk::get_all_files(const std::string& dir_in, std::vector<std::string> & files)
{
	if (dir_in.empty()) {
		return false;
	}
	struct stat s;
	stat(dir_in.c_str(), &s);
	if (!S_ISDIR(s.st_mode)) {
		return false;
	}
	DIR* open_dir = opendir(dir_in.c_str());
	if (NULL == open_dir) {
		std::exit(EXIT_FAILURE);
	}
	dirent* p = nullptr;
	while ((p = readdir(open_dir)) != nullptr) {
		struct stat st;
		if (p->d_name[0] != '.') {
			string strFileName = std::string(p->d_name);
			std::string name = dir_in + std::string("/") + strFileName;
			stat(name.c_str(), &st);
			if (S_ISDIR(st.st_mode)) {
				get_all_files(name, files);
			}
			else if (S_ISREG(st.st_mode)) {
				cout << name <<  "  : " << strFileName << endl;
				if (string::npos != strFileName.rfind(".jpg") || (string::npos != strFileName.rfind(".png")))
				{
					files.push_back(name);
				}
			}
		}
	}
	closedir(open_dir);
	return true;	
}

int  CMngSdk::MakeDir(char* dirname)
{
	int res;
	char path[512] = { 0 };
	memcpy(path, dirname, strlen(dirname));
	char* pEnd = strstr(path + 1, "/");
	if (NULL == pEnd)
	{
		res = mkdir(path, 0777);
	}
	while (pEnd)
	{
		*pEnd = 0;
		res = mkdir(path, 0777);
		*pEnd = '/';
		if (NULL == strstr(pEnd + 1, "/"))
		{
			if ('\0' != *(pEnd + 1))
			{
				mkdir(path, 0777);
			}
			break;
		}
		else
		{
			pEnd = strstr(pEnd + 1, "/");
		}
	}

	if (-1 == res)
	{
		return -1;
	}
	return 0;
}

string CMngSdk::getPwd()
{
	string strPath;
	strPath.resize(1024);
	getcwd(&strPath[0], 1024);
	return strPath.c_str();
}

string CMngSdk::buildFacePath()
{
	string path = getPwd();
	path += ("/" + IMAGE_PATH);
	cout << path << endl;
	MakeDir(&path[0]);
	return path;	
}


void CMngSdk::testVerifyFace(vector<string> & vecFiles)
{
	for (auto & strFilename : vecFiles) {
		cv::Mat image1 = cv::imread(strFilename.c_str());
		if (image1.empty()) {
			cout << "it's empty for call imread() " << endl;
			return;
		}
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
		int res = ASFDetectFaces(m_handle, image1.cols, image1.rows, ASVL_PAF_RGB24_B8G8R8, (MUInt8*)image1.data, &detectedFaces1);
		if (MOK == res) {
			SingleDetectedFaces1.faceRect.left = detectedFaces1.faceRect[0].left;
			SingleDetectedFaces1.faceRect.top = detectedFaces1.faceRect[0].top;
			SingleDetectedFaces1.faceRect.right = detectedFaces1.faceRect[0].right;
			SingleDetectedFaces1.faceRect.bottom = detectedFaces1.faceRect[0].bottom;
			SingleDetectedFaces1.faceOrient = detectedFaces1.faceOrient[0];

			//单人脸特征提取
			res = ASFFaceFeatureExtract(m_handle, image1.cols, image1.rows, ASVL_PAF_RGB24_B8G8R8, (MUInt8*)image1.data, &SingleDetectedFaces1, &feature1);
			if (res == MOK)
			{
				for (auto & featP : m_vecFeatDb) {
					if (featP->verifyFace(m_handle, feature1)) {
						cout << "checking file:" << strFilename << "    checked file:" << featP->getName() << endl;
					}					
				}
							

			}
			else
				printf("ASFFaceFeatureExtract 1 fail: %xd\n", (int)res);
		}
		else {
			printf("ASFDetectFaces 1 fail: %d\n", (int)res);
		}
	}

}

int CMngSdk::testFaceSDK()
{
	int nResult = 0;

	//激活SDK
	MRESULT res = ASFOnlineActivation((MPChar)APPID, (MPChar)SDKKEY);
	if (MOK != res && MERR_ASF_ALREADY_ACTIVATED != res)
		printf("ASFOnlineActivation fail: %d\n", (int)res);
	else
		printf("ASFOnlineActivation sucess: %d\n", (int)res);


	//初始化引擎
	MHandle handle = NULL;
	MInt32 mask = ASF_FACE_DETECT | ASF_FACERECOGNITION | ASF_AGE | ASF_GENDER | ASF_FACE3DANGLE | ASF_LIVENESS | ASF_IR_LIVENESS;
	res = ASFInitEngine(ASF_DETECT_MODE_IMAGE, ASF_OP_0_ONLY, NSCALE, FACENUM, mask, &handle);
	if (res != MOK) {
		printf("ALInitEngine fail: %d\n", (int)res);
		return ERR_AUTH;
	}
	else
		printf("ALInitEngine sucess: %d\n", (int)res);

	cv::Mat image1 = cv::imread("4.jpg");
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
	ASF_FaceFeature copyfeature1 = { 0 };
	res = ASFDetectFaces(handle, image1.cols, image1.rows, ASVL_PAF_RGB24_B8G8R8, (MUInt8*)image1.data, &detectedFaces1);
	if (MOK == res) {
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
		}
		else
			printf("ASFFaceFeatureExtract 1 fail: %xd\n", (int)res);
	}
	else {
		printf("ASFDetectFaces 1 fail: %d\n", (int)res);
	}


	cv::Mat image2 = cv::imread("3.jpg");
	width = image2.cols;
	heigh = image2.rows;
	pad = width % 4;
	if (0 != pad) {
		width -= pad;
		cv::resize(image2, image2, cv::Size(width, heigh));
	}
	//第二张人脸提取特征
	ASF_MultiFaceInfo   detectedFaces2 = { 0 };
	ASF_SingleFaceInfo SingleDetectedFaces2 = { 0 };
	ASF_FaceFeature feature2 = { 0 };
	res = ASFDetectFaces(handle, image2.cols, image2.rows, ASVL_PAF_RGB24_B8G8R8, (MUInt8*)image2.data, &detectedFaces2);
	if (MOK == res)
	{
		SingleDetectedFaces2.faceRect.left = detectedFaces2.faceRect[0].left;
		SingleDetectedFaces2.faceRect.top = detectedFaces2.faceRect[0].top;
		SingleDetectedFaces2.faceRect.right = detectedFaces2.faceRect[0].right;
		SingleDetectedFaces2.faceRect.bottom = detectedFaces2.faceRect[0].bottom;
		SingleDetectedFaces2.faceOrient = detectedFaces2.faceOrient[0];

		res = ASFFaceFeatureExtract(handle, image2.cols, image2.rows, ASVL_PAF_RGB24_B8G8R8, (MUInt8*)image2.data, &SingleDetectedFaces2, &feature2);
		if (MOK != res)
			printf("ASFFaceFeatureExtract 2 fail: %d\n", (int)res);
	}
	else
		printf("ASFDetectFaces 2 fail: %d\n", (int)res);
	
	// 单人脸特征比对
	MFloat confidenceLevel;
	res = ASFFaceFeatureCompare(handle, &copyfeature1, &feature2, &confidenceLevel);
	if (res != MOK)
		printf("ASFFaceFeatureCompare fail: %d\n", (int)res);
	else
		printf("ASFFaceFeatureCompare sucess: %lf\n", confidenceLevel);



	// 人脸信息检测
	MInt32 processMask = ASF_AGE | ASF_GENDER | ASF_FACE3DANGLE;
	res = ASFProcess(handle, image2.cols, image2.rows, ASVL_PAF_RGB24_B8G8R8, (MUInt8*)image2.data, &detectedFaces1, processMask);
	if (res != MOK)
		printf("ASFProcess fail: %d\n", (int)res);
	else
		printf("ASFProcess sucess: %d\n", (int)res);


	// 获取年龄
	ASF_AgeInfo ageInfo = { 0 };
	res = ASFGetAge(handle, &ageInfo);
	if (ageInfo.num > 0)
		printf("age: %d\n", *(ageInfo.ageArray));
	if (res != MOK)
		printf("ASFGetAge fail: %d\n", (int)res);
	else
		printf("ASFGetAge sucess: %d\n", (int)res);

	// 获取性别
	ASF_GenderInfo genderInfo = { 0 };
	res = ASFGetGender(handle, &genderInfo);
	if (genderInfo.num > 0)
	   printf("gender: %d\n", *(genderInfo.genderArray));
	if (res != MOK)
		printf("ASFGetGender fail: %d\n", (int)res);
	else
		printf("ASFGetGender sucess: %d\n", (int)res);

	// 获取3D角度
	ASF_Face3DAngle angleInfo = { 0 };
	res = ASFGetFace3DAngle(handle, &angleInfo);
	if (res != MOK)
		printf("ASFGetFace3DAngle fail: %d\n", (int)res);
	else
		printf("ASFGetFace3DAngle sucess: %d\n", (int)res);

	SafeFree(copyfeature1.feature); //释放内存


									//获取版本信息
	const ASF_VERSION* pVersionInfo = ASFGetVersion(handle);
	printf("版本号: %s date %s\n", pVersionInfo->Version, pVersionInfo->BuildDate);

	ASF_ActiveFileInfo fileInfo = { 0 };
	res = ASFGetActiveFileInfo(&fileInfo);
	if (res != MOK)
		printf("ASFGetActiveFileInfo fail: %d\n", (int)res);
	else {
		printf("ASFGetActiveFileInfo sucess: %d\n", (int)res);
		printf("startTime:%s endTime:%s  fileVersion:%s platform:%s\n", fileInfo.startTime,
			fileInfo.endTime, fileInfo.fileVersion, fileInfo.platform);
		time_t begin_time = (time_t) atoi(fileInfo.startTime);
		printf("%s", asctime(gmtime(&begin_time)));
		time_t end_time = (time_t) atoi(fileInfo.endTime);
		printf("%s", asctime(gmtime(&end_time)));
	}

	//反初始化
	res = ASFUninitEngine(handle);
	if (res != MOK)
		printf("ALUninitEngine fail: %d\n", (int)res);
	else
		printf("ALUninitEngine sucess: %d\n", (int)res);

	
	return nResult;
}



int CMngSdk::initSDK()
{
	int nResult = MOK;
	
	//nResult = testFaceSDK();

	//激活SDK
	MRESULT res = ASFOnlineActivation((MPChar)APPID, (MPChar)SDKKEY);
	if (MOK != res && MERR_ASF_ALREADY_ACTIVATED != res)
		printf("ASFOnlineActivation fail: %d\n", (int)res);
	else
		printf("ASFOnlineActivation sucess: %d\n", (int)res);


	//初始化引擎
	MHandle handle = NULL;
	MInt32 mask = ASF_FACE_DETECT | ASF_FACERECOGNITION | ASF_AGE | ASF_GENDER | ASF_FACE3DANGLE | ASF_LIVENESS | ASF_IR_LIVENESS;
	res = ASFInitEngine(ASF_DETECT_MODE_IMAGE, ASF_OP_0_ONLY, NSCALE, FACENUM, mask, &handle);
	if (res != MOK) {
		printf("ALInitEngine fail: %d\n", (int)res);
		return ERR_AUTH;
	}
	else
		printf("ALInitEngine sucess: %d\n", (int)res);
	m_handle = handle;
	m_sucess = true;

	string strPath = buildFacePath();
	vector<string> vecFiles;
	get_all_files(strPath, vecFiles);
	if (vecFiles.empty())
		return ERR_NO_FACE_TEMPLATE;

	for (auto & filename : vecFiles) {
		auto faceFea = std::make_shared<CFaceFeat>();
		int ret = -1;
		if (faceFea->buildFeature(m_handle, filename, ret)) {
			m_vecFeatDb.push_back(faceFea);
			nResult = MOK;			
		}
		else {
			cout << "error : " << filename << "  error_no :" << ret << endl;
			nResult = ret;
		}
	}


	//testVerifyFace(vecFiles);
	std::thread threadHandleCamera(&CMngSdk::threadHandleCamera, this);
	threadHandleCamera.detach();
	
	return nResult;
}

string CMngSdk::get_face_result()
{
	return m_faceResult->getResult();
}

bool CMngSdk::check_person()
{
	return m_faceResult->checkResult();
}

void CMngSdk::inputImage(cv::Mat & image_mat)
{	
	m_matQueue->inputInfo(image_mat);
	return;
}

void CMngSdk::threadHandleCamera()
{
	std::ostringstream oss;
	oss << std::this_thread::get_id();
	std::string stid = oss.str();
	unsigned long long tid = std::stoull(stid);
	cout << "thread id " << tid << endl;

	for (;;) {
		cv::Mat mat;
		if (m_matQueue->getInfo(mat)) {
			if (!mat.empty()) {
				handleMat(mat);				
			}
		}
		else {
			cout << "exit the thread!!" << endl;
			break;
		}		
	}
}

bool CMngSdk::checkFaceDbEmpyt()
{
	bool bResult = false;
	std::lock_guard<std::mutex> lck(m_lockFeatDb);
	if (m_vecFeatDb.empty())
	{
		bResult = true;
	}
	return bResult;
}

void CMngSdk::handleMat(cv::Mat & mat)
{
	//cv::imwrite("utest.jpg", mat);
	
	auto faceFeat = std::make_shared<CFaceFeat>();
	int ret = -1;
	bool bFeat = faceFeat->getFeatureMulti(m_handle, mat, ret);
	if (!bFeat) {
		cout << "fault to getFeature error_no: " << ret << endl;
		return;
	}	
	if (!checkFaceDbEmpyt())
		handleVerify(faceFeat, mat);
	return;
}

void CMngSdk::handleVerify(std::shared_ptr<CFaceFeat> & faceFeat, cv::Mat & mat)
{
	bool bFind = false;
	std::vector<std::shared_ptr<CFaceItem> > & vecFaceItem = faceFeat->getAllFaceItem();
	for (auto & faceItem : vecFaceItem)
	{
		auto & refFeat = faceItem->faceFeat;
		std::lock_guard<std::mutex> lck(m_lockFeatDb);
		for (auto & faceDb : m_vecFeatDb) {
			if (faceDb->verifyFace(m_handle, refFeat)) {

				bFind = true;
				auto json_data = std::make_shared<JSON_DATA>();
				MRECT & rect = faceItem->faceInfo.faceRect;
				cout << "find name jpg: " << faceDb->getName() << endl;
				cout << "rect(" << rect.left << " " << rect.top << " " << rect.right << " " << rect.bottom << ")" << endl;

				json_data->rect = rect;
				json_data->face_id = faceDb->getName();
				faceFeat->getRect(json_data->full_width, json_data->full_heigh);
				cout << "positon(" << json_data->full_width << " x " << json_data->full_heigh <<  ")" << endl;
				m_faceResult->inputResult(*json_data);
				break;
			}
		}
		if (bFind)
			break;
	}

	if (bFind) {
		m_faceResult->buildResult();
	}
	return;
}
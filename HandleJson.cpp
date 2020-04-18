#include "common.h"
#include "HandleJson.h"


using namespace rapidjson;

CHandleJson::CHandleJson()
{
}


CHandleJson::~CHandleJson()
{

}

string CHandleJson::buildFaceResult(JSON_DATA & data)
{
	StringBuffer s;
	Writer<StringBuffer> writer(s);
	writer.StartObject();
	writer.Key(JSON_FACE_INFO);			
	writer.StartObject();
	writer.Key(JSON_FACE_ID);
	writer.String(data.face_id.c_str());
	writer.Key(JSON_POSITION);
	writer.StartArray();
	writer.Int(data.rect.left);
	writer.Int(data.rect.top);
	writer.Int(data.rect.right);
	writer.Int(data.rect.bottom);
	writer.EndArray();
	writer.Key(JSON_IAMGE_POSITION);
	writer.StartArray();
	writer.Int(0);
	writer.Int(0);
	writer.Int(data.full_width);
	writer.Int(data.full_heigh);
	writer.EndArray();
	writer.EndObject();
	return s.GetString();
}

/*
string CHandleJson::buildAdResp(std::vector<AD_INFO> &  vecInfo)
{
	StringBuffer s;
	Writer<StringBuffer> writer(s);
	writer.StartObject();
	writer.Key(JSON_CODE);
	writer.Int(200);
	writer.Key(JSON_MSG);
	writer.String("ok");
	writer.Key(JSON_DATA);	
	writer.StartArray();
	for (auto & item : vecInfo) {
		writer.StartObject();
		writer.Key(JSON_AD_IMAGE);
		writer.String(item.strImageFile.c_str());
		writer.Key(JSON_AD_URL);
		writer.String(item.strUrl.c_str());
		writer.EndObject();
	}
	writer.EndArray();	
	writer.EndObject();
	return s.GetString();
}

string CHandleJson::buildVerResp(DOWNLOAD_INFO & info)
{
	StringBuffer s;
	Writer<StringBuffer> writer(s);
	writer.StartObject();
	writer.Key(JSON_CODE);
	writer.Int(200);
	writer.Key(JSON_MSG);
	writer.String("ok");
	writer.Key(JSON_DATA);
	writer.StartObject();
	writer.Key(JSON_VER);
	writer.String(info.ver.c_str());
	writer.Key(JSON_DOWNLOAD);
	writer.StartArray();
	for (auto & item : info.mpFile) {
		writer.String(item.first.c_str());
	}
	writer.EndArray();
	writer.EndObject();	
	writer.EndObject();
	return s.GetString();
}

string CHandleJson::buildLogDeviceResp(DEVICE_INFO & deviceInfo)
{
	StringBuffer s;
	Writer<StringBuffer> writer(s);
	writer.StartObject();
	writer.Key(JSON_CODE);
	writer.Int(200);
	writer.Key(JSON_MSG);
	writer.String("ok");
	writer.Key(JSON_LOG_DEVICE);
	writer.String(deviceInfo.logDevice.c_str());
	writer.Key(JSON_QUERY_TICKET);
	writer.String(deviceInfo.queryTicket.c_str());
	writer.Key(JSON_STATION_NAME);
	writer.String(deviceInfo.stationNameV.c_str());
	writer.EndObject();
	return s.GetString();
}

bool  CHandleJson::getTest(string & jpg, string &strJson)
{
	auto pDoc(UTIL_SELF::make_unique<rapidjson::Document>());
	rapidjson::Document & doc = *pDoc;

	doc.Parse<0>(strJson.c_str());
	if (doc.HasParseError())
	{
		LOG_ERROR(" it's error to analysis");
		return false;
	}

	string base64 = json_check_string(doc, "base64_jpg");
	if (!base64.empty()) {

		UTIL_SELF::Base64Decode(jpg, base64.data(), base64.size());
	}

	return true;
}

bool  CHandleJson::getUserInfo(USER_INFO & info, string &strJson)
{
	auto pDoc(UTIL_SELF::make_unique<rapidjson::Document>());
	rapidjson::Document & doc = *pDoc;

	doc.Parse<0>(strJson.c_str());
	if (doc.HasParseError())
	{
		LOG_ERROR(" it's error to analysis");
		return false;
	}
	info.userID = json_check_string(doc, JSON_USER_ID);
	info.pwd = json_check_string(doc, JSON_USER_PWD);
	return true;
}

bool  CHandleJson::getConfigInfo(DOWNLOAD_INFO &  info, string &strJson)
{
	auto pDoc(UTIL_SELF::make_unique<rapidjson::Document>());
	rapidjson::Document & doc = *pDoc;

	doc.Parse<0>(strJson.c_str());
	if (doc.HasParseError())
	{
		LOG_ERROR(" it's error to analysis");
		return false;
	}
	info.ver = json_check_string(doc, JSON_VER);
	if (json_check_is_array(doc, JSON_DOWNLOAD)) {
		rapidjson::Value & jsonResult = doc[JSON_DOWNLOAD];
		if (jsonResult.Empty())
		{
			return true;
		}
		string strEmpty;
		for (SizeType i = 0; i < jsonResult.Size(); i++)
		{
			string key = jsonResult[i].GetString();
			(info.mpFile)[key] = strEmpty;
			//LOG_INFO("key %s", key.c_str());
		}
	}
	return true;
}

bool CHandleJson::getDelInfo(int & faceID, string &strJson)
{
	bool bResul = false;
	auto pDoc(UTIL_SELF::make_unique<rapidjson::Document>());
	rapidjson::Document & doc = *pDoc;

	doc.Parse<0>(strJson.c_str());
	if (doc.HasParseError())
	{
		LOG_ERROR(" it's error to analysis");
		return bResul;
	}
	faceID = json_check_int32(doc, JSON_FACE_ID);
	return true;
}

bool CHandleJson::getPicInfo(string & jpg, string &strJson)
{
	bool bResul = false;
	auto pDoc(UTIL_SELF::make_unique<rapidjson::Document>());
	rapidjson::Document & doc = *pDoc;

	doc.Parse<0>(strJson.c_str());
	if (doc.HasParseError())
	{
		LOG_ERROR(" it's error to analysis");
		return bResul;
	}
	string base64 = json_check_string(doc, JSON_IMAGE_DATA);
	if (!base64.empty()) {

		UTIL_SELF::Base64Decode(jpg, base64.data(), base64.size());
		bResul = true;
	}
	return bResul;
}


bool  CHandleJson::getAdInfo(std::vector<AD_INFO> &  vecInfo, string &strJson)
{
	
	auto pDoc(UTIL_SELF::make_unique<rapidjson::Document>());
	rapidjson::Document & doc = *pDoc;
	doc.Parse<0>(strJson.c_str());
	if (doc.HasParseError())
	{
		LOG_ERROR(" it's error to analysis");
		return false;
	}

	if (!json_check_is_array(doc, JSON_DATA)) {
		return false;
	}
	rapidjson::Value & jsonResult = doc[JSON_DATA];
	if (jsonResult.Empty()) {
		return false;
	}

	for (SizeType i = 0; i < jsonResult.Size(); i++) {
		AD_INFO adInfo;
		rapidjson::Value & adResul = jsonResult[i];		
		adInfo.strImageFile = json_check_string(adResul, JSON_AD_IMAGE);
		adInfo.strUrl = json_check_string(adResul, JSON_AD_URL);
		vecInfo.push_back(adInfo);
	}
	return true;
}

string CHandleJson::buildResp(int code, string msg)
{
	StringBuffer s;
	Writer<StringBuffer> writer(s);
	writer.StartObject();	
	writer.Key(JSON_CODE);
	writer.Int(code);
	writer.Key(JSON_MSG);
	writer.String(msg.c_str());
	writer.EndObject();	
	return s.GetString();
}


bool CHandleJson::getKeeplive(MSG_USER_INFO & info, string &strJson)
{
	auto pDoc(UTIL_SELF::make_unique<rapidjson::Document>());
	rapidjson::Document & doc = *pDoc;

	doc.Parse<0>(strJson.c_str());
	if (doc.HasParseError())
	{
		LOG_ERROR(" it's error to analysis");
		return false;
	}
	info.authID = json_check_string(doc, JSON_AUTH_ID);
	info.avID = json_check_string(doc, JSON_USER_ID);
	info.state = json_check_int32(doc, JSON_USER_STATE);
	return true;
}

void CHandleJson::buildCheckResp(string & resp, int code, string msg, FACE_ATTRI & attri)
{
	StringBuffer s;
	Writer<StringBuffer> writer(s);
	writer.StartObject();
	writer.Key(JSON_CODE);
	writer.Int(code);
	writer.Key(JSON_MSG);
	writer.String(msg.c_str());
	writer.Key(JSON_FACE_ID);
	writer.Int(attri.face_id);
	writer.Key(JSON_FACE_TYPE);
	writer.Int(attri.face_type);
	writer.Key(JSON_FACE_FRIST);
	writer.Bool(attri.face_first);
	writer.Key(JSON_FACE_SEX);
	writer.String(attri.sex.c_str());
	writer.Key(JSON_FACE_AGE);
	writer.Int(attri.age);
	writer.EndObject();
	resp = s.GetString();
	return;
}

void CHandleJson::buildResp(string & resp, int code, string msg, int id, int appType)
{
	StringBuffer s;
	Writer<StringBuffer> writer(s);
	writer.StartObject();
	writer.Key(JSON_CODE);
	writer.Int(code);
	writer.Key(JSON_MSG);
	writer.String(msg.c_str());
	writer.Key(JSON_FACE_ID);
	writer.Int(id);
	writer.Key(JSON_APP_TYPE);
	writer.Int(appType);
	writer.EndObject();
	resp = s.GetString();
	return;
}
*/


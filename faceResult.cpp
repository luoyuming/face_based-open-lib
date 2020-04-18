#include "faceResult.h"
#include "HandleJson.h"


CFaceResult::CFaceResult()
{
}


CFaceResult::~CFaceResult()
{
}

bool CFaceResult::checkResult()
{
	bool bResult = false;
	std::lock_guard<std::mutex> lck(m_lock);
	if (!m_strJson.empty()) {
		bResult = true;
	}

	return bResult;
}

void CFaceResult::inputResult(JSON_DATA & data)
{
	m_data = data;
}

void CFaceResult::buildResult()
{
	auto handleJson(UTIL_SELF::make_unique<CHandleJson>());
	string json = handleJson->buildFaceResult(m_data);
	std::lock_guard<std::mutex> lck(m_lock);
	m_strJson = json;
	return;
}

string CFaceResult::getResult()
{
	string strResult;
	std::lock_guard<std::mutex> lck(m_lock);
	strResult = m_strJson;
	m_strJson.clear();
	return strResult;
}

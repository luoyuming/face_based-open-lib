#pragma once
#include "common.h"
#include "arcsoft_face_sdk.h"
#include "amcomdef.h"
#include "asvloffscreen.h"
#include "merror.h"

struct JSON_DATA {
	MRECT rect;
	int full_width;
	int full_heigh;
	string face_id;

};

class CFaceResult
{
public:
	CFaceResult();
	~CFaceResult();
	bool checkResult();
	void inputResult(JSON_DATA & data);
	void buildResult();
	string getResult();
private:
	std::mutex	m_lock;
	string		m_strJson;
	JSON_DATA   m_data;
};


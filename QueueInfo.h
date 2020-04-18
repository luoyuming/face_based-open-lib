#pragma once
#include "common.h"

using namespace std;

class CQueue {

public:	
    CQueue();
    ~CQueue();
	void inputInfo(cv::Mat & mat);
	bool getInfo(cv::Mat & mat);
    void notifyExit();    
private:
    bool                                    m_bExit;
    std::condition_variable                 m_cv;
	std::mutex								m_lock;
	std::list<cv::Mat>						m_queue;
};



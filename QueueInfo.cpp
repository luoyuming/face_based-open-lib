#include "QueueInfo.h"


CQueue::CQueue() :m_bExit(false) 
{
  
}

CQueue::~CQueue()
{

}

void CQueue::notifyExit()
{    
    std::unique_lock <std::mutex> lck(m_lock);
    m_bExit = true;    
    m_cv.notify_all();
}

void CQueue::inputInfo(cv::Mat & mat)
{
    std::unique_lock <std::mutex> lck(m_lock);
	m_queue.push_back(mat);
    m_cv.notify_all();	
	return;
}

bool CQueue::getInfo(cv::Mat & mat)
{	
    std::unique_lock <std::mutex> lck(m_lock);
    while (m_queue.empty() && (!m_bExit) )
    {
        m_cv.wait(lck);        
    }
    if (m_bExit)
    {
        return false;
    }
	mat = std::move(m_queue.front());
	m_queue.pop_front();   
	return true;
    
}



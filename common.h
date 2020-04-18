#ifndef _COMMON_H
#define _COMMON_H

#include <unistd.h>
#include <stdio.h>
#include <cassert>
#include <mutex>
#include <chrono>
#include <thread>
#include <list>
#include <vector>
#include <map>
#include <memory>
#include <condition_variable>
#include <unordered_map>
#include <string>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <functional>
#include <fstream>
#include <random>
#include <iterator>
#include <cctype>
#include <locale>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <atomic>
#include <cstring>
#include <iomanip>
using namespace std;


#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <limits.h>  
#include <sys/resource.h>  
#include <signal.h>  
#include <sys/types.h>
#include <pthread.h>
#include <assert.h>
#include <time.h>
#include <linux/tcp.h>



typedef  int32_t		byte4;
typedef  uint32_t		ubyte4;
typedef  int32_t        SOCKET;
#define INVALID_SOCKET  (SOCKET)(~0)



#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


namespace UTIL_SELF {
	template<typename T, typename... Args>
	std::unique_ptr<T> make_unique(Args&&... args) {
		return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
	}
}
#endif
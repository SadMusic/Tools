#include <map>
#include <list>
#include <mutex>
#include <thread>

#ifdef WIN32
#include <windows.h>
#include <WinSock.h>
#pragma comment(lib, "WS2_32.lib")
#else

#endif

class ClientSocket {
protected:
	ClientSocket();

	static ClientSocket* getInstance();
	static void destoryInstance();

	void start();
	void stop();
	bool isStart();

public:
	virtual ~ClientSocket();
private:
	bool init();
	void uninit();
protected:
	static ClientSocket* _instance;

	bool _isInitialized;
	bool _isStart;
};
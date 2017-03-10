#include <map>
#include <list>
#include <mutex>
#include <thread>

#ifdef WIN32
	#include <windows.h>
	#include <WinSock.h>
	#pragma comment(lib, "WS2_32.lib")
#else
	#define SOCKET int;
	#define INVALID_SOCKET -1;
#endif

class ClientSocket {
protected:
	ClientSocket();

	static ClientSocket* getInstance();
	static void destoryInstance();

	void start();
	void stop();
	bool isStart();

	bool connectToServer(int guid, const char* ip, int port, long timeoutSec = 1, long timeoutUSec = 0);

public:
	virtual ~ClientSocket();
private:
	bool init();
	void uninit();

	bool connectToServer(SOCKET s, unsigned long addr, int port, long timeoutSec = 1, long timeoutUSec = 0);
	void closeSocket(SOCKET s);

	bool hasError();
	bool isError(int code);

protected:
	static ClientSocket* _instance;

	bool _isInitialized;
	bool _isStart;

	int _maxSocket;
protected:
	struct SockrtInfo{
		SOCKET socket;
		char ip[16];
		int port;
		int guid;

		SockrtInfo(){
			socket = 0;
			port = 0;
			guid = 0;

			memset(ip, 0, sizeof(ip));
		}
	};

};
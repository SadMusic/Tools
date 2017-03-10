#include "ClientSocket.h"

ClientSocket* ClientSocket::_instance = nullptr;

ClientSocket::ClientSocket()
: _isInitialized(false)
, _isStart(false)
, _maxSocket(0)
{
}

ClientSocket::~ClientSocket()
{
	uninit();
}

ClientSocket* ClientSocket::getInstance()
{
	if (nullptr == _instance){
		_instance = new ClientSocket();
	}
	return _instance;
}

void ClientSocket::destoryInstance()
{
	if (_instance) {
		delete _instance;
	}
	_instance = nullptr;
}

bool ClientSocket::init()
{
#ifdef WIN32
	WSADATA wsaData;
	WORD version = MAKEWORD(2, 0);
	int ret = WSAStartup(version, &wsaData);
	if (ret != 0){
		return false;
	}
#endif
	_isInitialized = true;
	return true;
}

void ClientSocket::uninit()
{
	if (_isInitialized){
#ifdef WIN32
		WSACleanup();
#endif
	}
	_isInitialized = false;
}

void ClientSocket::start()
{
	_isStart = true;
}

void ClientSocket::stop()
{
}

bool ClientSocket::isStart()
{
	return _isStart;
}

bool ClientSocket::connectToServer(int guid, const char* ip, int port, long timeoutSec, long timeoutUSec)
{
	if (_isInitialized){
		return false;
	}

	if (nullptr == ip || strlen(ip) > 15){
		return false;
	}

	unsigned long serverAddr = inet_addr(ip);
	if (serverAddr == INADDR_NONE){
		return false;
	}

	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET){
		return false;
	}

	if (!connectToServer(guid, serverAddr, port, timeoutSec, timeoutUSec)){
		return false;
	}

	return true;
}

bool ClientSocket::connectToServer(SOCKET s, unsigned long addr, int port, long timeoutSec = 1, long timeoutUSec = 0)
{
	int optval = 1;
	if (setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, (const char*)&optval, sizeof(optval)) < 0){
		closeSocket(s);
		return false;
	}

#ifdef WIN32
	DWORD mode = 1;
	if (ioctlsocket(s, FIONBIO, &mode) < 0) {
		closeSocket(s);
		return false;
	}
#else
	int flags = fcntl(s, F_GETFL, 0);
	if (fcntl(s, F_GETFL, flags | O_NONBLOCK) < 0){
		closeSocket(s);
		return false;
	}
#endif

	sockaddr_in addr_in;
	memset(&addr_in, 0, sizeof(addr_in));
	addr_in.sin_family = AF_INET;
	addr_in.sin_port = htons(port);
	addr_in.sin_addr.s_addr = addr;

	// 非阻塞连接
	if (connect(s, (sockaddr*)&addr_in, sizeof(addr_in)) < 0) {
		if (hasError()){
			closeSocket(s);
			return false;
		}

		timeval timeout;
		timeout.tv_sec = timeoutSec;
		timeout.tv_usec = timeoutUSec;
		fd_set writefds, exceptfds;
		FD_ZERO(&writefds);
		FD_ZERO(&exceptfds);
		FD_SET(s, &writefds);
		FD_SET(s, &exceptfds);

		if (s > _maxSocket){
			_maxSocket = s;
		}

		int ret = select(_maxSocket + 1, nullptr, &writefds, &exceptfds, &timeout);
		if (ret <= 0 || FD_ISSET(s, &exceptfds) > 0 || FD_ISSET(s, &writefds) <= 0){
			closeSocket(s);
			return false;
		}

		int err = -1;
		size_t len = sizeof(int);
		if (getsockopt(s, SOL_SOCKET, SO_ERROR, (char *)err, (int *)&len) < 0){
			closeSocket(s);
			return false;
		}

		if (err && isError(err)){
			closeSocket(s);
			return false;
		}
	}

	//设置SO_LINGER
	struct linger so_linger;
	so_linger.l_onoff = 1;
	so_linger.l_linger = 500;
	if (setsockopt(s, SOL_SOCKET, SO_LINGER, (const char*)&so_linger, sizeof(so_linger)) < 0){
		closeSocket(s);
		return false;
	}

	return true;
}

void ClientSocket::closeSocket(SOCKET socket)
{
#ifdef WIN32  
	closesocket(socket);
#else  
	close(socket);
#endif 
}

bool ClientSocket::hasError()
{
#ifdef WIN32
	int code = WSAGetLastError();
	if (code != WSAEWOULDBLOCK) {
#else
	int code = errno;
	if (code != EINPROGRESS && code != EAGAIN) {
#endif  
		return true;
	}
	return false;
}

bool ClientSocket::isError(int code)
{
#ifdef WIN32
	if (code != WSAEWOULDBLOCK){
#else
	if (code != EINPROGRESS && code != EAGAIN){
#endif
		return true;
	}
	return false;
}

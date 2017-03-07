#include "ClientSocket.h"

ClientSocket* ClientSocket::_instance = nullptr;

ClientSocket::ClientSocket()
: _isInitialized(false)
, _isStart(false)
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
#ifdef _WIN32
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
#ifdef _WIN32
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

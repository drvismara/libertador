#ifndef _CLIENT_CALLABLE_H_
#define _CLIENT_CALLABLE_H_

#include <map>

#include "KeyMaintainer.h"

class ClientThread
{
public:
	ClientThread(int client_sock, std::map<std::string, KeyMaintainer>* kmp);

	// aqui el procesamiento de cliente
	void operator()(const char* msg);

private:
	void processRequest(const char* req);

	void processRequest2(const char* req);

	int clientSocket;

	std::string* answer;

	std::map<std::string, KeyMaintainer>* keyMap;
};

#endif

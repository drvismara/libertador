#ifndef _SILVER_BULLET_H_
#define _SILVER_BULLET_H_

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <ConfigFile.h>
#include <ClientThread.h>
#include <KeyMaintainer.h>

//namespace Libertador {
class SilverBulletServer
{
public:
	SilverBulletServer(ConfigFile* cnf);

	int startServer();
	int stopServer();

	void startKeyMaintainerProcess();
	
private:
	int max_client;
	int listen_port;
	int max_file_process;

	int socket_desc;
    struct sockaddr_in server;

    ConfigFile* sConf;

    std::map<std::string, KeyMaintainer> keyMaint;
};

//}

#endif // _SILVER_BULLET_H_


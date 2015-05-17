#include <iostream>
#include <thread>

#include <ConfigFile.h>
#include <SilverBulletServer.h>

int main(int argc, char** argv)
{
	int ret=0;

	ConfigFile conf("silver_bullet.conf");

	SilverBulletServer* server = new SilverBulletServer(&conf);

	server->startKeyMaintainerProcess();
	server->startServer();

	return ret;
}


#include <iostream>

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <KeyMaintainer.h>

KeyMaintainer::KeyMaintainer(std::string mapName,  std::string mapMapper, std::string mapFrm, int mapKeyLoad)
{
	name = mapName;
	mapper = mapMapper;
	frm = mapFrm;
	keyLoad = mapKeyLoad;

	updFlag = FLAG_DEACTIVE;
	insFlag = FLAG_DEACTIVE;
	delFlag = FLAG_DEACTIVE;

	msgFlag = MSG_DEACTIVE;
}

void KeyMaintainer::operator()(const char* msg)
{
	std::cout << "KeyMaintainer: [ " << msg << " ] inicializado." << std::endl;
	std::string command(" ");

	clock_t t1 = clock();
	clock_t t2;
	clock_t t_delta;
	float sec_t;

	while(1)
	{
		std::cout << "KeyMaintainer: [ " << msg << " ] on_while." << std::endl;

		while ( (msgFlag == MSG_DEACTIVE) || (msgFlag == MSG_SENT) )
		{
			t2 = clock();
			t_delta = t2 - t1;
			sec_t = ((float)t_delta)/CLOCKS_PER_SEC;

			if (sec_t > (keyLoad*3600))
			{
				// aca hace un chequeo de estado general
				// y obligo al flush table en MySQL
				// forzando la recarga
			}

			//std::cout << "KeyMaintainer: [ " << msg << " ] on_while 2." << std::endl;
		}

		std::cout << "KeyMaintainer: [ " << msg << " ] process command." << std::endl;

		command.clear();
		command.assign(in_msg);

		if (command.compare("CHECK") == 0)
		{
			std::cout << "KeyMaintainer: [ " << msg << " ] on CHECK." << std::endl;

			if (checkNews() == OP_OK)
			{
				std::cout << "KeyMaintainer: [ " << msg << " ] checkNews() OK." << std::endl;
				msgFlag = MSG_SENT;
			} else{
				out_msg.clear();
				out_msg.assign("ERROR");
				msgFlag = MSG_SENT;
			}
		}

		if (command.compare("UPDATE") == 0)
		{
			if (doUpdate() == OP_OK)
			{
				msgFlag = MSG_SENT;
			} else{
				out_msg.clear();
				out_msg.assign("ERROR_UPDATE");
				msgFlag = MSG_SENT;
			}
		}

		if (command.compare("INSERT") == 0)
		{
			if (doInsert() == OP_OK)
			{
				msgFlag = MSG_SENT;
			} else{
				out_msg.clear();
				out_msg.assign("ERROR_INSERT");
				msgFlag = MSG_SENT;
			}
		}

		if (command.compare("DELETE") == 0)
		{
			if (doDelete() == OP_OK)
			{
				msgFlag = MSG_SENT;
			} else{
				out_msg.clear();
				out_msg.assign("ERROR_DELETE");
				msgFlag = MSG_SENT;
			}
		}

		if (command.compare("QUIT") == 0)
		{
			break;
		}

	}

	std::cout << "KeyMaintainer: [ " << msg << " ] detenido." << std::endl;
}

void KeyMaintainer::messenger(std::string cmd, std::string resp)
{
	in_msg = cmd;
	msgFlag = MSG_ACTIVE;

	std::cout << "messenger() | MSG_ACTIVE";

	while (msgFlag == MSG_ACTIVE)
	{
		//do nothing
	}

	resp.clear();
	resp.assign(out_msg);

	msgFlag = MSG_DEACTIVE;
}

int KeyMaintainer::checkNews()
{
	int ret = OP_OK;

	out_msg.clear();
	out_msg.assign("DO_UPDATE");

	return ret;
}

int KeyMaintainer::doUpdate()
{
	int ret = OP_OK;

	return ret;
}

int KeyMaintainer::doInsert()
{
	int ret = OP_OK;

	return ret;
}

int KeyMaintainer::doDelete()
{
	int ret = OP_OK;

	return ret;
}

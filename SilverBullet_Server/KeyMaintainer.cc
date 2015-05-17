#include <iostream>

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "KeyMaintainer.h"

KeyMaintainer::KeyMaintainer(std::string mapName,  std::string mapMapper, std::string mapFrm, int mapKeyLoad)
{
	name = mapName;
	mapper = mapMapper;
	frm = mapFrm;
	keyLoad = mapKeyLoad;

	updFlag = FLAG_DEACTIVE;
	insFlag = FLAG_DEACTIVE;
	delFlag = FLAG_DEACTIVE;
}

KeyMaintainer::KeyMaintainer(KeyMaintainer&& other)
{
    name = std::move(other.name);
	mapper = std::move(other.mapper);
	frm = std::move(other.frm);
	keyLoad = std::move(other.keyLoad);

    updFlag = std::move(other.updFlag);
	insFlag = std::move(other.insFlag);
	delFlag = std::move(other.delFlag);
}

KeyMaintainer::KeyMaintainer(const KeyMaintainer& other)
{
    name = other.name;
	mapper = other.mapper;
	frm = other.frm;
	keyLoad = other.keyLoad;

    updFlag = other.updFlag;
	insFlag = other.insFlag;
	delFlag = other.delFlag;
}

KeyMaintainer& KeyMaintainer::operator= (KeyMaintainer&& other)
{
    name = std::move(other.name);
	mapper = std::move(other.mapper);
	frm = std::move(other.frm);
	keyLoad = std::move(other.keyLoad);

    updFlag = std::move(other.updFlag);
	insFlag = std::move(other.insFlag);
	delFlag = std::move(other.delFlag);

	return *this;
}

KeyMaintainer& KeyMaintainer::operator= (const KeyMaintainer& other)
{
    name = other.name;
	mapper = other.mapper;
	frm = other.frm;
	keyLoad = other.keyLoad;

    updFlag = other.updFlag;
	insFlag = other.insFlag;
	delFlag = other.delFlag;

	return *this;
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

		while ( (getEnvMessage() == MSG_DEACTIVE) || (getEnvMessage() == MSG_SENT) )
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
		command = in_msg;

		if (command.compare("CHECK") == 0)
		{
			std::cout << "KeyMaintainer: [ " << msg << " ] on CHECK." << std::endl;

			if (checkNews() == OP_OK)
			{
				std::cout << "KeyMaintainer: [ " << msg << " ] checkNews() OK." << std::endl;
				putEnvMessage(MSG_SENT);
			} else{
				out_msg.clear();
				out_msg = "ERROR";
				putEnvMessage(MSG_SENT);
			}
		}

		if (command.compare("UPDATE") == 0)
		{
			if (doUpdate() == OP_OK)
			{
				putEnvMessage(MSG_SENT);
			} else{
				out_msg.clear();
				out_msg.assign("ERROR_UPDATE");
				putEnvMessage(MSG_SENT);
			}
		}

		if (command.compare("INSERT") == 0)
		{
			if (doInsert() == OP_OK)
			{
				putEnvMessage(MSG_SENT);
			} else{
				out_msg.clear();
				out_msg.assign("ERROR_INSERT");
				putEnvMessage(MSG_SENT);
			}
		}

		if (command.compare("DELETE") == 0)
		{
			if (doDelete() == OP_OK)
			{
				putEnvMessage(MSG_SENT);
			} else{
				out_msg.clear();
				out_msg.assign("ERROR_DELETE");
				putEnvMessage(MSG_SENT);
			}
		}

		if (command.compare("QUIT") == 0)
		{
			break;
		}
	}

	std::cout << "KeyMaintainer: [ " << msg << " ] detenido." << std::endl;
}

int KeyMaintainer::getEnvMessage()
{
    int ret = -1;

    varMtx.lock();

    std::string varName = "SB_KEY_MSG_" + name;
    std::string envData = "SB_KEY_DATA_" + name;

    std::string SB_KEY_MSG = getenv(varName.c_str());

    if (SB_KEY_MSG.compare("MSG_ACTIVE") == 0)
    {
        ret = MSG_ACTIVE;
        in_msg = getenv(envData.c_str());
    }

    if (SB_KEY_MSG.compare("MSG_DEACTIVE") == 0)
    {
        ret = MSG_DEACTIVE;
    }

    if (SB_KEY_MSG.compare("MSG_SENT") == 0)
    {
        ret = MSG_SENT;
    }

    varMtx.unlock();

    return ret;
}

int KeyMaintainer::putEnvMessage(int msg)
{
    int ret=0;
    std::string varName = "SB_KEY_MSG_" + name;
    std::string envData = "SB_KEY_DATA_" + name;

    varMtx.lock();

    switch(msg)
    {
        case MSG_ACTIVE:
                        setenv(varName.c_str(), "MSG_ACTIVE", 1);
                        break;
        case MSG_DEACTIVE:
                        setenv(varName.c_str(), "MSG_DEACTIVE", 1);
                        break;
        case MSG_SENT:
                        setenv(envData.c_str(), out_msg.c_str(), 1);
                        setenv(varName.c_str(), "MSG_SENT", 1);
                        break;
    };

    varMtx.unlock();
    return ret;
}

int KeyMaintainer::checkNews()
{
	int ret = OP_OK;

	out_msg.clear();
	out_msg = "DO_UPDATE";

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

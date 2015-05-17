

#include <iostream>
#include <thread>
#include <map>

#include <SilverBulletServer.h>
#include <KeyMaintainer.h>

SilverBulletServer::SilverBulletServer(ConfigFile* cnf)
{
	std::cout << "SilverBulletServer Constructor." << std::endl;

    sConf = cnf;

	max_client = sConf->Value("system", "max_client");
	listen_port = sConf->Value("system", "port");
	max_file_process = sConf->Value("system", "max_file_process");
}

int SilverBulletServer::startServer()
{
    int client_sock;
    int *new_sock;
    int c;
    struct sockaddr_in client;

    int th_idx = 0;

    std::thread* thr_cli[200];

    std::cout << "SilverBulletServer::startServer" << std::endl;

    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        throw std::runtime_error("SilverBulletServer::startServer: ERROR socket().");
    }
    
    std::cout << "Socket created" << std::endl;

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( listen_port );

    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        throw std::runtime_error("SilverBulletServer::startServer: ERROR bind().");
    }

    listen(socket_desc, max_client);

    std::cout << "SilverBulletServer inicializado correctamente. [ ESPERANDO ]" << std::endl;
    c = sizeof(struct sockaddr_in);

    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        new_sock = (int*) malloc(sizeof(int)*1);
        *new_sock = client_sock;

        ClientThread* cli = new ClientThread(*new_sock, &keyMaint);

		thr_cli[th_idx] = new std::thread((*cli), "CON_HILO");
		thr_cli[th_idx]->join();

		if (client_sock < 0)
    	{
        	throw std::runtime_error("SilverBulletServer::startServer: ERROR accept().");
    	}

        th_idx++;
    }
}

int SilverBulletServer::stopServer()
{
	std::cout << "SilverBulletServer::stopServer" << std::endl;
    std::thread* thrKeyMaintainer;

    thrKeyMaintainer->join();
}


void SilverBulletServer::startKeyMaintainerProcess()
{
    std::cout << "SilverBulletServer::startKeyMaintainerProcess" << std::endl;
    
    std::string* listSection = sConf->getSections();

    std::cout << "SilverBulletServer::startKeyMaintainerProcess | getSections() " << std::endl;
    int i=0;

    std::thread* thr_cli[200];
    //KeyMaintainer* km[200];
    /*
    std::string mapName;
    std::string mapMapper;
    std::string mapFrm;
    */

    while(listSection[i].compare("END") != 0)
    {
        std::cout << "SilverBulletServer::startKeyMaintainerProcess | listSection[i] " << listSection[i] << std::endl;

        std::string mapName = listSection[i];

        std::cout << "SilverBulletServer::startKeyMaintainerProcess | mapName " << mapName << std::endl;

        std::string mapMapper = sConf->Value(listSection[i], "mapper");

        std::cout << "SilverBulletServer::startKeyMaintainerProcess | mapMapper " << mapMapper << std::endl;

        std::string mapFrm = sConf->Value(listSection[i], "frm_check");

        std::cout << "SilverBulletServer::startKeyMaintainerProcess | mapFrm " << mapFrm << std::endl;

        int mapKeyLoad = sConf->Value(listSection[i], "key_check");

        std::cout << "SilverBulletServer::startKeyMaintainerProcess | mapKeyLoad " << mapKeyLoad << std::endl;

        KeyMaintainer* km = new KeyMaintainer(mapName, mapMapper, mapFrm, mapKeyLoad);

        std::cout << "SilverBulletServer::startKeyMaintainerProcess | KeyMaintainer creado... " << std::endl;

        keyMaint.insert( std::pair<std::string, KeyMaintainer>(mapName, (*km) ) );

        //keyMaint[mapName] = (*km);
        
        thr_cli[i] = new std::thread((*km), mapName.c_str());
        //thr_cli[i]->join();

        //mapName.clear();
        //mapMapper.clear();
        //mapFrm.clear();
        //mapKeyLoad = 0;

        i++;
    }
}


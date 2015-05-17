#include <iostream>
#include <map>
#include <string>

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <ClientThread.h>
#include <KeyMaintainer.h>

#include <silver_common.h>

ClientThread::ClientThread(int client_sock, std::map<std::string, KeyMaintainer>* kmp)
{
	std::cout << "ClientThread Constructor" << std::endl;
	clientSocket = client_sock;

	answer = new std::string("");

    keyMap = kmp;
}

void ClientThread::operator()(const char* msg)
{
	int read_size;
    char *message , client_message[2000];

    memset(client_message, '\0', 2000);
    
    std::cout << "Operator() del cliente: " << msg << std::endl;

    //Send some messages to the client
    message = "Greetings! I am your connection handler\n";
    write(clientSocket , message , strlen(message));
     
    message = "Now type something and i shall repeat what you type \n";
    write(clientSocket , message , strlen(message));
    
    while( (read_size = recv(clientSocket , client_message , 2000 , 0)) > 0 )
    {
		client_message[read_size-2] = '\0';
        
        std::cout << "DATA recv() " << client_message << std::endl;

        processRequest2(client_message);

        //processRequest(client_message);
        write(clientSocket , "RECEIVED...\n" , 12);
        write(clientSocket, answer->c_str(), answer->length());
		memset(client_message, 0, 2000);
		answer->erase();
    }
    
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    } else if(read_size == -1) {
        perror("recv failed");
    }
}

void ClientThread::processRequest2(const char* req)
{
    std::string request(req);
    int posEqual;
    std::string mName;
    std::string comd;
    std::string comd_resp("");

    std::map<std::string, KeyMaintainer>::iterator it;

    posEqual=request.find(';');

    mName = request.substr(0,posEqual);
    comd = request.substr(posEqual+1);

    std::cout << "MAPPER: " << mName << std::endl;
    std::cout << "COMMAND: " << comd << std::endl;

    it = keyMap->find(mName);
    (*it).second.messenger(comd, comd_resp);

    std::cout << "ANSWER: " << comd_resp << std::endl;

    //(*keyMap)[mName].messenger(comd, comd_resp);

    if (request.compare("CHECK\0") == 0)
    {
        std::cout << "Entro " << std::endl;
    } else{
        std::cout << "No entro #" << request << "#" << std::endl;
    }
}

void ClientThread::processRequest(const char* req)
{
	std::string PR;
	std::string FW;
	std::string PP;
	int posEqual;
	int posEqual2;

	std::string prog_path;
	std::string path_file;
	std::string params;

	std::string command;

	std::string request((char*)req);

	// mensajes que recv
	// PR:<path_program_to_run>;FW:<path_file_to_write>;PP:<program_params>
	// pero lo procesa el cliente.

	std::cout << "RECV: " << req << std::endl;

	posEqual=request.find(';');
    PR = request.substr(0,posEqual);

    posEqual2 = (request.substr(posEqual+1)).find(';');
    FW = (request.substr(posEqual+1)).substr(0,posEqual2);

    PP = (request.substr(posEqual+1)).substr(posEqual2+1);

    std::cout << "PR-> " << PR << std::endl;
    std::cout << "FW-> " << FW << std::endl;
    std::cout << "PP-> " << PP << std::endl;

    posEqual = PR.find(':');
    prog_path = PR.substr(posEqual+1);

    posEqual = FW.find(':');
    path_file = FW.substr(posEqual+1);

    posEqual = PP.find(':');
    params = PP.substr(posEqual+1);

    std::cout << "prog_path-> " << prog_path << std::endl;
    std::cout << "path_file-> " << path_file << std::endl;
    std::cout << "params-> " << params << std::endl;

    command.append(prog_path);
    command.append(" ");
    command.append(path_file);
    command.append(" ");
    command.append(params);

    FILE* pf;
    char* err=NULL;
    char data[512];

    std::cout << "COMANDO: " << command << std::endl;

    if (system(prog_path.c_str()) == -1)
    {
    	answer->append("ERROR COMMAND");
    } else {
    	answer->append("command ok.");
    }

}


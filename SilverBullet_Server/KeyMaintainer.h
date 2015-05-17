#ifndef _KEY_MAINTAINER_H_
#define _KEY_MAINTAINER_H_

#define FIELD_SEPARATOR ','

#define FLAG_ACTIVE 1
#define FLAG_DEACTIVE 0

#define MSG_ACTIVE 1
#define MSG_DEACTIVE 0
#define MSG_SENT 2

#define OP_OK 0
#define OP_NOK 1

class KeyMaintainer
{
	// cada 1Hs forzara el update completo de archivos.

public:
	KeyMaintainer(std::string mapName,  std::string mapMapper, std::string mapFrm, int mapKeyLoad);

	void operator()(const char* msg);

	void messenger(std::string cmd, std::string resp);

private:
	std::string name;
	std::string mapper;
	std::string frm;
	int keyLoad;

	int updFlag;
	int insFlag;
	int delFlag;

	std::string in_msg;
	std::string out_msg;
	int msgFlag;

	int checkNews();
	int doUpdate();
	int doInsert();
	int doDelete();
};


#endif // _KEY_MAINTAINER_H_

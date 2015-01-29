#ifndef NETWORK_H
#define NETWORK_H

#include "LinkedList.h"

struct GUID
{
	unsigned long Data1;
	unsigned short  Data2;
	unsigned short  Data3;
	unsigned char  Data4[8];
};

struct PlayerInfo
{
	GUID guid;
	char *name;
};

class Network
{
public:
	Network();
private:
	LinkedList<PlayerInfo> players;
};

#endif
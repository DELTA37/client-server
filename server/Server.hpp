#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <utility>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <fcntl.h>
#include <vector>
#include <string>
#include <stdint.h>
#include <stdlib.h>
#include <cmath>
#include "Header/DataBase.hpp"

#define PORT (8901)
#define NUMCLIENT (100)
//#define HTML
#define DEBUG

class CServer
{
private:
    int sockfd;             // socket descriptor
    int port;               // port
    sockaddr_in addr;       // c-struct which responsable for information about connection
    int client[NUMCLIENT];  // client descriptors
    int count;              // count of clients [ <= NUMCLIENT]

    fd_set s_client;        // structure responsable for select-usage

    CTable* Table[NUMCLIENT]; // local table for each client
    CTable CommonTable;       // common table
public:
    CServer(void);
    void AddClient(void);                   
    void RemoveClient(int i);            // Delete client
    std::string ReadFrom(int i);
    bool IsReadyRead(int i);
    bool IsReadyWrite(int i);
    void WriteTo(int i, std::string msg);
    bool NonBlock(int fd, bool blocking);
    virtual void MainLoop(void); 
    virtual void Do(int i, std::string msg); // what we do with client after request
};

struct Command
{
    std::string command;
   
    std::vector<std::string> fields;
    std::string prefix;
    std::vector<int> group;
    std::vector< std::pair<double, double> > rating;
    std::string sort;

    int correct;
    
    Command(std::string str);
    void print(void);
    static int StrToInt(std::string); // single to all
};

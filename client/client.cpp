#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <iostream>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <assert.h>
#include <fstream>

#define DEFAULTPORT (8901)
#define HOST "127.0.0.1"


void SendBuf(const char *buf, int sock)
{
    int n = strlen(buf);
	std::cout << n << std::endl;
    send(sock, &n, sizeof(int), 0);
  	send(sock, buf, n, 0);
}


bool IsReadyWrite(int sock)
{
    fd_set fds;
    struct timeval tv;

    FD_ZERO(&fds);

    FD_SET(sock, &fds);

    tv.tv_sec = tv.tv_usec = 10;

    int rc = select(sock + 1, NULL, &fds, NULL, &tv);

    if (rc < 0)
    {
        return -1;
	std::cout << "Select error";
    }

    return FD_ISSET(sock, &fds) ? 1 : 0;
}

bool IsReadyRead(int sock)
{
    fd_set fds;
    struct timeval tv;

    FD_ZERO(&fds);

    FD_SET(sock, &fds);

    tv.tv_sec = tv.tv_usec = 3;

    int rc = select(sock + 1, &fds, NULL, NULL, &tv);

    if (rc < 0)
    {
        return -1;
    }

    return FD_ISSET(sock, &fds) ? 1 : 0;
}



std::string ReadBuf(int sock)
{
    char *buf;
    int r = 0;
    int n = 0;

    while(r < (int)sizeof(int))
    {
        r += read(sock, (&n) + r, sizeof(int) - r);
    }

    r = 0; 
    buf = (char *)malloc((n + 1) * sizeof(char));
    if (buf == NULL)
    {
        std::cout << "Error in malloc" << std::endl;
    }

    while(r < n)
    {
        r += read(sock, buf + r, n - r);
    }
    buf[n] = '\0';
    return std::string(buf);
}

int main(void)
{
	int sockfd;
	
    char *buf = NULL;
	std::string str;
	int l = 0;
	l = l;

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
    addr.sin_port = htons(DEFAULTPORT);
	addr.sin_addr.s_addr = inet_addr(HOST);
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	assert(connect(sockfd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)) == 0);
	
    while(!std::cin.eof())
    {
		/* ONE WAY
        if ((l = getline(&buf, (size_t *)&l, stdin)) == -1)
		{
			std::cout << "Wil bve continued "<< std::endl;
			continue;
		} 
		buf[l - 1] = '\0';
		*/
			std::getline(std::cin, str);
			
       // if (buf.substr(0, 5) == std::string("print"))
       // {
	    	SendBuf(str.c_str(), sockfd);
            std::cout << "sent" << std::endl;
            std::cout << "Read: " << ReadBuf(sockfd) << std::endl;
			free(buf);
       /* }
        else
        {
			if (IsReadyRead(sockfd))
            SendBuf(buf.c_str(), sockfd);
            std::cout << "sent" << std::endl;
        }*/
    }
	close(sockfd);
	return 0;
}

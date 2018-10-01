#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char* argv[]){
    if( argc < 2)
    {
        fprintf(stderr, "Port No not provided. Program terminated\n");
        exit(1);
    }
    int sockfd, newsockfd, portno, n;
    char buffer[255];

    // sockadr_in gives us the internet address of server or client
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;

    //int sockfd = socket(int domain, int type, int protocol)
    //                    AF_INET,    SOCK_STREAM, 0
    //                    IPv4,       TCP,         Default TCP port
    // CREATE THE SOCKET
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        error("Error opening socket.");
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    //serv.addr.sin_addr.s_addr = inet_addr(ip_addr);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    // int bind(int sockfd, const struct sockaddr *addr, socklen_taddrlen)
    // assigns address specfied by addr to the socket record (sockfd)
    if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) error("Binding Failed.");

    //struc sockaddr{           --> return 0 if success and -1 if failure
    //  sa_family_t sa_family;
    //  char sa_data[14];
    //}

    // int listen(int sockfd, int backlog)
    // listens for the connection on a socket
    // backlog --> number of connections a system can handle at a time ( 4 = 4 connections)
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    //newsockfd = accept(sockfd, (struct sockaddr*) &addr, &addrlen);
    // all communications will be taken on newsockfd
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0) error("Error on accept");

    //int connection(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
    // return 0 for success and -1 for failure

    //BEGIN WRITING CODE FOR COMMUNICATION BETWEEN CLIENT AND SERVER
    while(1)
    {
        bzero(buffer, 255);
        n = read(newsockfd, buffer, 255);
        if(n < 0) error("Error on reading");
        printf("Client: %s\n", buffer);
        bzero(buffer, 255);
        fgets(buffer, 255, stdin);

        n = write(newsockfd, buffer, strlen(buffer));
        
        //int read(new sockfd, buffer, buffer_size)
        // int write(newsockfd, budder_size)
        // buffer --> message
        // buffer --> message_size

        if(n < 0) error("Error on writing");
        int i = strncmp("Bye", buffer, 3);
        if (i == 0) break;
    }


}
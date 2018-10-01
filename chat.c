#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <unistd.h> 
#include <errno.h> 
#include <arpa/inet.h> 

void error(const char *msg)
{
    perror(msg);
    exit(1);
}
// Returns hostname for the local computer 
void checkHostName(int hostname) 
{ 
    if (hostname == -1) 
    { 
        perror("gethostname"); 
        exit(1); 
    } 
} 
  
// Returns host information corresponding to host name 
void checkHostEntry(struct hostent * hostentry) 
{ 
    if (hostentry == NULL) 
    { 
        perror("gethostbyname"); 
        exit(1); 
    } 
} 
  
// Converts space-delimited IPv4 addresses 
// to dotted-decimal format 
void checkIPbuffer(char *IPbuffer) 
{ 
    if (NULL == IPbuffer) 
    { 
        perror("inet_ntoa"); 
        exit(1); 
    } 
} 
char* getIpAddress(){
    char hostbuffer[256]; 
    char *IPbuffer; 
    struct hostent *host_entry; 
    int hostname; 
  
    // To retrieve hostname 
    hostname = gethostname(hostbuffer, sizeof(hostbuffer)); 
    checkHostName(hostname); 
  
    // To retrieve host information 
    host_entry = gethostbyname(hostbuffer); 
    checkHostEntry(host_entry); 
  
    // To convert an Internet network 
    // address into ASCII string 
    IPbuffer = inet_ntoa(*((struct in_addr*) 
                           host_entry->h_addr_list[0])); 
    return IPbuffer; 
}

int connectToServer(int port, char *ip_addr){
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    // Used to store information about a given host (hostname and ipv4 address)
    struct hostent *server;

    char buffer[255];
    portno = port;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) error("Error opening socket.");

    server = gethostbyname(ip_addr);
    if(server == NULL) fprintf(stderr, "Error, no such host");
        
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) error("Connection failed.");
    while(1)
    {
        bzero(buffer, 255);
        fgets(buffer, 255, stdin);
        n = write(sockfd, buffer, strlen(buffer));
        if(n < 0) error("Error on writing.");
        bzero(buffer, 255);
        n = read(sockfd, buffer, 255);
        if(n < 0 ) error("Error on reading;");
        printf("Server: %s", buffer);

        int i = strncmp("Bye", buffer, 3);
        if (i == 0) break;
    }
    close(sockfd);
    return 0;
}

int main(int argc, char* argv[]){
    // GET PORT NUMBER FROM COMMAND LINE
    int port = 0;
    
    if(argc <  2 || argc > 2){
        printf("Please enter one port number");
        exit(0);
    } else{
        port = atoi(argv[1]);
    }

    // GET IP ADDRESS
    char* ip_addr = getIpAddress();
    printf("Waiting for client on %s port %s....\nEnter help for instructions.\n", ip_addr, argv[1]);

    // // START SERVER
    // int sockfd, newsockfd, portno, n;
    // char buffer[255];

    // // sockadr_in gives us the internet address of server or client
    // struct sockaddr_in serv_addr, cli_addr;
    // socklen_t clilen;

    // //int sockfd = socket(int domain, int type, int protocol)
    // //                    AF_INET,    SOCK_STREAM, 0
    // //                    IPv4,       TCP,         Default TCP port
    // // CREATE THE SOCKET
    // sockfd = socket(AF_INET, SOCK_STREAM, 0);
    // if(sockfd < 0)
    // {
    //     error("Error opening socket.");
    // }
    // bzero((char *) &serv_addr, sizeof(serv_addr));
    // portno = atoi(argv[1]);

    // serv_addr.sin_family = AF_INET;
    // serv_addr.sin_addr.s_addr = inet_addr(ip_addr);
    // //serv_addr.sin_addr.s_addr = INADDR_ANY;
    // serv_addr.sin_port = htons(portno);

    // // int bind(int sockfd, const struct sockaddr *addr, socklen_taddrlen)
    // // assigns address specfied by addr to the socket record (sockfd)
    // if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) error("Binding Failed.");

    // //struc sockaddr{           --> return 0 if success and -1 if failure
    // //  sa_family_t sa_family;
    // //  char sa_data[14];
    // //}

    // // int listen(int sockfd, int backlog)
    // // listens for the connection on a socket
    // // backlog --> number of connections a system can handle at a time ( 4 = 4 connections)
    // listen(sockfd, 5);
    // clilen = sizeof(cli_addr);

    // //newsockfd = accept(sockfd, (struct sockaddr*) &addr, &addrlen);
    // // all communications will be taken on newsockfd
    // newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    // if (newsockfd < 0) error("Error on accept");

    // //int connection(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
    // // return 0 for success and -1 for failure

    // //BEGIN WRITING CODE FOR COMMUNICATION BETWEEN CLIENT AND SERVER
    // // while(1)
    // // {
    // //     bzero(buffer, 255);
    // //     n = read(newsockfd, buffer, 255);
    // //     if(n < 0) error("Error on reading");
    // //     printf("Client: %s\n", buffer);
    // //     bzero(buffer, 255);
    // //     fgets(buffer, 255, stdin);

    // //     n = write(newsockfd, buffer, strlen(buffer));
        
    // //     //int read(new sockfd, buffer, buffer_size)
    // //     // int write(newsockfd, budder_size)
    // //     // buffer --> message
    // //     // buffer --> message_size

    // //     if(n < 0) error("Error on writing");
    // //     int i = strncmp("Bye", buffer, 3);
    // //     if (i == 0) break;
    // // }






    //
    char command[256];
    printf( ">> ");
    fgets(command, 256, stdin);
    char newString[50][50];
    int i,j,ctr;
    j=0; ctr=0;
    for(i=0;i<=(strlen(command));i++)
    {
        // if space or NULL found, assign NULL into newString[ctr]
        if(command[i]==' '||command[i]=='\0')
        {
            newString[ctr][j]='\0';
            ctr++;  //for next word
            j=0;    //for next word, init index to 0
        }
        else
        {
            newString[ctr][j]=command[i];
            j++;
        }
    }
    char* help = "help";
    char* word = (char *)newString[0];
    if(strcmp((char *)newString[0], help) == 0) printf("help");
    else if(strcmp(newString[0], "myip") == 0) printf("The ip address is %s", ip_addr);
    else if(strcmp(newString[0], "myport") == 0) printf("The program runs on port number %s", argv[1]);
    else if(strcmp(newString[0], "connect") == 0) connectToServer(atoi(newString[2]), newString[1]);
    else printf("Please try again");
}


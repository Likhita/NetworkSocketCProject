#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "structandhelperfunctions.h"
 
#define BUF_SIZE 4096

int main(int argc, char *argv[])
{
    printf("This is the client program\n");
	int sockfd,port;
	char buf[BUF_SIZE];
	struct hostent *host; 
    struct sockaddr_in address;
	struct information my_info;
	
	// check for 3 command line arguments
	if (argc < 3) { 
		fprintf(stderr,"Usage: %s hostname port\n", argv[0]); 
		exit(0);
    }
	
	//atoi() converts ASCII to integer and gets the port number
	port = atoi(argv[2]); 
	
   //Create socket for client.
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) { 
        perror("Socket create failed.\n") ; 
        return -1 ; 
    } 
	printf("Client socket created..\n");
	
	//gethostbyname() gets the IP address given the host name
	host=gethostbyname(argv[1]);
	if(!host){
        perror("gethostbyname failed !");
        exit(-1);
    }
	printf("Fetching the server's IP address..\n");
	memset(&address,0,sizeof(address)); 
    address.sin_family = AF_INET;
	memcpy(&address.sin_addr.s_addr,host->h_addr,host->h_length);
    address.sin_port = htons(port);
 
	//connecting to the server
	printf("Connecting to the server, %s on port, %d\n", argv[1], port); 
    if(connect(sockfd,(struct sockaddr *)&address,sizeof(address)) == -1)
    {
        perror(" Connect error has occurred");
        exit(-1);
    }
	printf("Client is now connected to the server \n");
	
	//getting the data from the user and converting the integer and float values 
	//from host to network format using htonl() and htonf()
	printf("Please enter any integer value:");
	scanf("%d", &my_info.number); 
	my_info.number = htonl(my_info.number);
	printf("\nPlease enter any char value:");
	scanf("%s", &my_info.letter);
	printf("\nPlease enter any float value:");
	scanf("%f", &my_info.f);
	my_info.f = convert_float(my_info.f);

	//writing data to the socket file descriptor sockfd
	if(write(sockfd, &my_info, sizeof(struct information)) == -1)
    {
        perror(" Write error has occurred");
        exit(-1);
    }
	printf("Data sent to the server!\n");  
	
	//read data sent back from server
	read(sockfd, buf, BUF_SIZE);
	printf("Server sent : %s\n", buf);
    
	//closing the socket connection
	close(sockfd);
	printf("Client closes the connection.\n");
    exit(0);
}
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include "structandhelperfunctions.h"

#define BUF_SIZE 4096
#define QUEUE_LENGTH 10
 
int main(int argc, char *argv[])
{
    int server_sockfd, client_sockfd,port;
    int server_len ; 
    int rc ; 
    unsigned client_len;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
	struct information received_info;
 
	// check for 2 command line arguments
	if (argc < 2) { 
		fprintf(stderr,"Usage: %s port\n", argv[0]); 
		exit(0);
    }
	
	//atoi() converts ASCII to integer and gets the port number
	port = atoi(argv[1]); 
	
	memset(&server_address,0,sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htons(INADDR_ANY);
    server_address.sin_port = htons(port) ; 

	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_sockfd == -1) { 
        perror("Server socket create failed.\n") ; 
        return -1 ; 
    } 
	
    if(bind(server_sockfd, (struct sockaddr *) &server_address, sizeof(server_address)) == -1){
        perror(" Bind error has occurred");
        exit(-1);
    } 
	printf("Server Bind successful..\n"); 
	
    //creates a connection queue and wait for clients
	if(listen(server_sockfd, QUEUE_LENGTH) == -1){
        perror(" Listen error has occurred");
        exit(-1);
    }
	printf("Server is listening..\n");
	
    client_sockfd = accept(server_sockfd, 0, 0);
    if (client_sockfd < 0) {
		perror(" Accept error has occurred");
        exit(-1);
	}	
	printf("Server accepted the client connection..\n");
	printf("Server waiting for data..\n");

	if(read(client_sockfd, &received_info, sizeof(struct information)) == -1)
    {
        perror(" Read error has occurred");
        exit(-1);
    }
	received_info.number = ntohl(received_info.number);
	received_info.f = convert_float(received_info.f);

	printf("------------------------------\n");
	printf("Data received from client:\n");
	printf("------------------------------\n");
	printf("Integer value: %d\n", received_info.number) ; 
	printf("Char value: %c\n", received_info.letter) ;
	printf("Float value: %f\n", received_info.f) ;
	printf("------------------------------\n");
	
	write(client_sockfd, "Thanks, received your data", 27);
	
	if (close(client_sockfd) == -1) { 
		perror("Error occurred while closing connection");
		exit(-1);
    }
	printf("Connection with client closed.\n");
	
	if (close(server_sockfd) == -1) { 
		perror("Error occurred while closing connection");
		exit(-1);
    }
	printf("Server exiting..\n");
    return 0;
}
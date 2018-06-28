#include "unp.h"

void
str_cli(FILE *f, int sockfd);

void
str_cli(FILE *f, int sockfd)
{
        char sendline[MAXLINE], recvline[MAXLINE];
        while(Fgets(sendline, MAXLINE, f) != NULL) {
                Write(sockfd, sendline, strlen(sendline));
                /* connection closed when read 0 */
                if(Readline(sockfd, recvline, MAXLINE) == 0)
                        err_quit("str_cli: server terminated prematurely");
                Fputs(recvline, stdout);
        }
}

int
main(int argc, char *argv[])
{
        if(argc != 2)
                err_quit("usage: tcpcli <IPaddress>");

        int sockfd;
        sockfd = Socket(AF_INET, SOCK_STREAM, 0);

        struct sockaddr_in sockaddr;
        bzero(&sockaddr, sizeof(sockaddr));

        sockaddr.sin_family = AF_INET;
        sockaddr.sin_port = htons(SERV_PORT);
        Inet_pton(AF_INET, argv[1], &sockaddr.sin_addr);

        Connect(sockfd, (SA *)&sockaddr, sizeof(sockaddr));

        str_cli(stdin, sockfd);

        return 0;
}

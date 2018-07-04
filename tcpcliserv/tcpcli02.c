#include "unp.h"

void
str_cli(FILE *f, int sockfd);

void
sigpipehandler(int signum)
{
        err_quit("sigpipe");
}

void
str_cli(FILE *f, int sockfd)
{
        int maxfdp1;
        fd_set rfd_set;
        char sendline[MAXLINE], recvline[MAXLINE];

        for(;;) {
                FD_ZERO(&rfd_set);
                FD_SET(fileno(f), &rfd_set);
                FD_SET(sockfd, &rfd_set);
                maxfdp1 = MAX(sockfd, fileno(f)) + 1;
                Select(maxfdp1, &rfd_set, NULL, NULL, NULL);

                if(FD_ISSET(sockfd, &rfd_set)) {
                        if(Readline(sockfd, recvline, sizeof(recvline)) == 0)
                                err_quit("str_cli: server terminated prematurely");
                        Fputs(recvline, stdout);
                }

                if(FD_ISSET(fileno(f), &rfd_set)) {
                        /* 这里有两个问题 */
                        /* 1. Fgets有cache但是cache住的，select检测不到，所以和select合作最好不要用cache版本的io */
                        /* 2. 如果read EOF 从 stdin, 我们直接退出程序，可能有一些tcp packets 没有接收的，会丢掉 */
                        if (Fgets(sendline, MAXLINE, f) == NULL)
                                return;
                        Writen(sockfd, sendline, strlen(sendline));
                }
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

        Signal(SIGPIPE, sigpipehandler);

        Connect(sockfd, (SA *)&sockaddr, sizeof(sockaddr));

        str_cli(stdin, sockfd);

        return 0;
}

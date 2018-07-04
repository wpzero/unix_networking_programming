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
        int maxfdp1, normalshutdown;
        size_t n;
        fd_set rrfd_set, rfd_set;
        char sendline[MAXLINE], recvline[MAXLINE];
        normalshutdown = 0;
        FD_ZERO(&rfd_set);
        FD_SET(fileno(f), &rfd_set);
        FD_SET(sockfd, &rfd_set);
        maxfdp1 = MAX(sockfd, fileno(f)) + 1;

        for(;;) {
                memcpy(&rrfd_set, &rfd_set, sizeof(rfd_set));
                Select(maxfdp1, &rrfd_set, NULL, NULL, NULL);

                if(FD_ISSET(sockfd, &rrfd_set)) {
                        if((n = Read(sockfd, recvline, sizeof(recvline))) == 0) {
                                if(normalshutdown == 1)
                                        return;
                                else
                                        err_quit("str_cli: server terminated prematurely");
                        }
                        Write(fileno(stdout), recvline, n);
                }

                if(FD_ISSET(fileno(f), &rrfd_set)) {
                        if((n = Read(fileno(f), sendline, sizeof(sendline))) == 0) {
                                normalshutdown = 1;
                                /* 关掉写 */
                                Shutdown(sockfd, SHUT_WR);
                                /* 不在监听 stdin */
                                /* 程序现在要做的是继续接收没有接收的packets */
                                FD_CLR(fileno(f), &rfd_set);
                                continue;
                        }
                        Writen(sockfd, sendline, n);
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

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
        char sendline[MAXLINE], recvline[MAXLINE];
        while(Fgets(sendline, MAXLINE, f) != NULL) {
                Write(sockfd, sendline, 1);
                sleep(1);
                /* 当给一个接受到过rst的socket在写东西的时候会出发SIGPIPE信号 */
                /* 如果这个signal默认是terminate the process */
                /* 如果要是handler改成不是terminate,就要处理write的返回值，errno会是EPIPE */
                /* unix networking programming 5.13 */
                Write(sockfd, sendline + 1, strlen(sendline) - 1);
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

        Signal(SIGPIPE, sigpipehandler);

        Connect(sockfd, (SA *)&sockaddr, sizeof(sockaddr));

        str_cli(stdin, sockfd);

        return 0;
}

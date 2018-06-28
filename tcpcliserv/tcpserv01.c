/* 这个实现是没有wait children的所以会有zombie的process产生 */

#include "unp.h"


/* 处理child process wait */
/* 这是一个比经典的处理方法 */
void
sigchld_handler(int signal)
{
        /* 非阻塞调用，利用WNOHANG */
        while (waitpid(-1, NULL, WNOHANG) > 0);
}


void
str_echo(int connfd)
{
        ssize_t n;
        char buff[MAXLINE];
again:
        while((n = Read(connfd, buff, MAXLINE)) > 0)
                Write(connfd, buff, n);
        if(n < 0 && errno == EINTR)
                goto again;
        else if(n < 0)
                err_sys("str_echo: read error");
}

int
main(int argc, char *argv[])
{
        int listenfd, connfd;
        socklen_t len;
        pid_t childpid;
        struct sockaddr_in servaddr, cliaddr;
        bzero(&servaddr, sizeof(servaddr));
        bzero(&cliaddr, sizeof(cliaddr));

        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(SERV_PORT);

        listenfd = Socket(AF_INET, SOCK_STREAM, 0);

        Bind(listenfd, (SA *)&servaddr, sizeof(servaddr));

        Listen(listenfd, LISTENQ);

        Signal(SIGCHLD, sigchld_handler);

        for(;;) {
                len = sizeof(cliaddr);
                connfd = Accept(listenfd, (SA *)&cliaddr, &len);
                if((childpid = Fork()) == 0) {
                        Close(listenfd);
                        str_echo(connfd);
                        Close(connfd);
                        exit(EXIT_SUCCESS);
                }
                Close(connfd);
        }

        return 0;
}

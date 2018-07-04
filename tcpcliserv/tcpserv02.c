#include "unp.h"

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
        long arg1, arg2;
again:
        while((n = Readline(connfd, buff, MAXLINE)) > 0) {
                printf("%s", buff);
                if(sscanf(buff, "%ld %ld", &arg1, &arg2) == 2)
                        snprintf(buff, sizeof(buff),"%ld\n", arg1 + arg2);
                else
                        snprintf(buff, sizeof(buff), "input error\n");
                n = strlen(buff);
                Writen(connfd, buff, n);
        }
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
                if((connfd = accept(listenfd, (SA *)&cliaddr, &len)) < 0) {
                        if(errno == EINTR)
                                continue;
                        else
                                err_sys("accept error");
                }
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

/* ========== */
/* 这是利用多进程的daytime server的实现 */
/* ========== */

#include <time.h>
#include "unp.h"

void doit(int);

void
doit(int fd)
{
        char buff[MAXLINE];
        time_t ticks;
        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        Write(fd, buff, strlen(buff));
}

int
main(int argc, char *argv[])
{
        int listenfd, connfd;
        socklen_t len;
        pid_t pid;
        char buff[180];
        struct sockaddr_in serveraddr, cliaddr;
        listenfd = Socket(AF_INET, SOCK_STREAM, 0);

        bzero(&serveraddr, sizeof(serveraddr));
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
        serveraddr.sin_port = htons(13);

        Bind(listenfd, (SA *)&serveraddr, sizeof(serveraddr));
        Listen(listenfd, LISTENQ);

        for(;;) {
                len = sizeof(cliaddr);
                connfd = Accept(listenfd, (SA *)&cliaddr, &len);
                len = sizeof(cliaddr);
                printf("%s : %d", Inet_ntop(AF_INET, &cliaddr, buff, len), ntohs(cliaddr.sin_port));
                if((pid = Fork()) == 0) {
                        Close(listenfd);
                        doit(connfd);
                        Close(connfd);
                        exit(EXIT_SUCCESS);
                }
                Close(connfd);
        }

        return 0;
}

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>

int sock;

void handler()
{
    send(sock, "shut", sizeof("shut"), 0);
    shutdown(sock, SHUT_WR);
    raise(SIGINT);
}

int main(int argc, char *argv[])
{
    int c_pid;
    int qq = 0;
    struct sockaddr_in srv;
    char buf[2000];
    char mmsg[100];
    sock = socket(AF_INET, SOCK_STREAM, 0);
    srv.sin_addr.s_addr = inet_addr("127.0.0.1");
    srv.sin_family = AF_INET;
    srv.sin_port = htons(13888);
    connect(sock, (struct sockaddr *)&srv, sizeof(srv));
    signal(SIGINT, handler);
    if ((c_pid = fork()) == -1) //fork出错
    {
        printf("子进程fork失败。\n");
        return 1;
    }
    else if (c_pid == 0) //子进程，执行命令
    {
        while (1)
        {
            fgets(mmsg, 100, stdin);
            send(sock, mmsg, sizeof(mmsg), 0);
        }
    }
    else
    {
        while (1)
        {
            recv(sock, buf, sizeof(buf), 0);
            if (strcmp(buf, "shut") == 0)
            {
                close(sock);
                printf("\n超时下线\n");
                return 0;
            }
            if (qq == 0)
            {
                printf("消息来自：%s\n", buf);
                qq++;
            }
            else
            {
                printf("收到消息：%s\n", buf);
                qq = 0;
            }
        }
    }
    return 0;
}
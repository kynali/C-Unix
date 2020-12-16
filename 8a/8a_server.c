#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

struct sset
{
    int socket;
    char name[50];
    int state;
} set[100];

int setnum = 0;
int maxfd;
//select准备
fd_set readfds;
fd_set readfds_backup; //因为select后会修改readfds，所以再定义一个readfds_backup保存accept的套接字

void handler()  //超时踢下线
{
    signal(SIGALRM, handler);
    for (int i = 0; i < setnum; i++)
    {
        if (set[i].socket > 0 && set[i].state == 1)
        {
            send(set[i].socket, "shut", sizeof("shut"), 0);
            FD_CLR(set[i].socket, &readfds_backup);
            close(set[i].socket);
            if (maxfd == set[i].socket)
                maxfd--;
            printf("socket= %d 超时下线\n", set[i].socket);
            set[i].socket = -1;
        }
        if (set[i].socket > 0)
            set[i].state = 1;
    }
    alarm(60);
}

int main(int argc, char *argv[])
{
    int srv_sock, clt_sock, c, recv_len;
    struct sockaddr_in srv, clt;
    char buf[2000];
    char tbuf[2000];
    srv_sock = socket(AF_INET, SOCK_STREAM, 0); // 创建套接字
    srv.sin_family = AF_INET;
    srv.sin_addr.s_addr = htonl(INADDR_ANY); // 在所有地址上监听
    srv.sin_port = htons(13888);             // 监听 13888 端口
    // 绑定套接字
    bind(srv_sock, (struct sockaddr *)&srv, sizeof(srv)); // 为什么要强制类型转换?
    listen(srv_sock, 5);

    maxfd = srv_sock;
    FD_ZERO(&readfds); //清空集合
    FD_ZERO(&readfds_backup);
    FD_SET(srv_sock, &readfds_backup); //套接字放到集合里
    int i = 0;
    int j = 0;
    int k = 0;
    int m = 0;
    int selectnum = 0;

    signal(SIGALRM, handler);
    alarm(60);

    while (1)
    {
        readfds = readfds_backup;

        selectnum = select(maxfd + 1, &readfds, NULL, NULL, NULL);
        //printf("%d\n", selectnum);
        if (selectnum <= 0)
        {
            printf("select failed\n");
            continue;
        }

        //遍历select后的readfds
        for (i = 0; i <= maxfd; i++)
        {
            if (FD_ISSET(i, &readfds))
            {
                //分两种情况，1.有新的客户端连接，就去accept 2.已连接的客户端发信息过来
                if (i == srv_sock)
                {
                    // 接受客户端连接
                    c = sizeof(struct sockaddr_in);
                    clt_sock = accept(srv_sock, (struct sockaddr *)&clt, (socklen_t *)&c);
                    printf("----------------------------------------------\n");
                    printf("socket= %d, client ip= %s, port= %d 连接上来啦\n", clt_sock, inet_ntoa(*((struct in_addr *)&clt.sin_addr.s_addr)), ntohs(clt.sin_port));
                    FD_SET(clt_sock, &readfds_backup);
                    set[setnum].socket = clt_sock;
                    strcpy(set[setnum].name, "wuming");
                    set[setnum].state = 3;
                    setnum++;
                    if (maxfd < clt_sock)
                        maxfd = clt_sock;
                }
                else
                {
                    //这里就是已连接的Client发送信息进来啦
                    for (m = 0; m < setnum; m++)
                    {
                        if (set[m].socket == i)
                            break;
                    }
                    set[m].state = 2;

                    memset(buf, 0, sizeof(buf));
                    recv_len = recv(i, buf, sizeof(buf), 0);
                    if (recv_len < 0)
                        printf("recv failed\n");
                    else
                    {
                        printf("socket= %d 对你说：%s\n", i, buf);
                        if (buf[0] == '*')  //改名*命令
                        {
                            for (m = 0; m < setnum; m++)
                            {
                                if (set[m].socket == i)
                                    break;
                            }
                            strcpy(set[m].name, buf + 1);
                            send(i, "服务器", sizeof("服务器"), 0);
                            usleep(1000);
                            send(i, "改名完成\n", sizeof("改名完成\n"), 0);
                        }
                        else if (buf[0] == '@') //@就是对某用户说话
                        {
                            for (m = 0; m < setnum; m++)
                            {
                                if (set[m].socket == i)
                                    break;
                            }
                            memset(tbuf, 0, sizeof(tbuf));
                            for (k = 0; k < strlen(buf); k++)
                            {
                                if (buf[k] == ' ')
                                {
                                    tbuf[k - 1] = '\n';
                                    break;
                                }
                                tbuf[k] = buf[k + 1];
                            }

                            for (j = 0; j < setnum; j++)
                            {
                                if (set[j].socket > 0 && strcmp(set[j].name, tbuf) == 0)
                                {
                                    for (m = 0; m < setnum; m++)
                                    {
                                        if (set[m].socket == i)
                                            break;
                                    }
                                    send(set[j].socket, set[m].name, sizeof(set[m].name), 0);
                                    usleep(1000);
                                    send(set[j].socket, buf, sizeof(buf), 0);
                                }
                            }
                        }
                    }
                    if (strcmp(buf, "shut") == 0)   //client下线操作
                    {
                        close(i);
                        FD_CLR(i, &readfds_backup);
                        if (maxfd == i)
                            maxfd--;
                        for (m = 0; m < setnum; m++)
                        {
                            if (set[m].socket == i)
                                break;
                        }
                        set[m].socket = -1;
                        printf("socket= %d 已下线\n", i);
                    }
                }
            }
        }
    }
    close(clt_sock);
    close(srv_sock);
    return 0;
}